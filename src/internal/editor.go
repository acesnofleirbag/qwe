// Copyright (c) The Quickware Editor, 2026

package internal

import (
	"os"
	"strings"

	"github.com/gdamore/tcell/v2"
)

type Mode int

const (
	Mode__Normal = iota
	Mode__Insert
	Mode__Visual
	Mode__Command
	Mode__Goto
	Mode__Tree
)

type Movement int

const (
	Movement__Up = iota
	Movement__Right
	Movement__Down
	Movement__Left
)

type LineMode int

const (
	LineMode__Normal = iota
	LineMode__Relative
)

type Editor struct {
	config   map[string]any
	mode     Mode
	cmdline  []rune
	buffer   *Buffer
	finder   *Finder
	timeline *Timeline
	filetree *FileTree
	tui      *TUI
	exit     bool
}

var EDITOR = Editor{
	config:   CONFIG,
	mode:     Mode__Normal,
	cmdline:  make([]rune, 0),
	buffer:   NewBuffer(),
	finder:   NewFinder(),
	timeline: NewTimeline(),
	exit:     false,
}

func (self *Editor) OpenPath(fname string) {
	info, err := os.Stat(fname)

	if err != nil {
		Log("[ERROR] editor.OpenPath: cannot stat path")
	}

	if info.IsDir() {
		self.filetree = NewFileTree(fname)
		self.toggleFileTree()
	} else {
		self.buffer = NewBufferFromPath(fname)
	}
}

func (self *Editor) GetFileTreeWidth() int {
	if self.filetree == nil || !self.filetree.show {
		return 0
	}

	return self.filetree.width + 1
}

func (self *Editor) toggleFileTree() {
	if self.filetree == nil {
		self.filetree = NewFileTree(".")
	}

	self.filetree.show = !self.filetree.show

	if self.filetree.show {
		self.setMode(Mode__Tree)
	} else {
		self.setMode(Mode__Normal)
	}
}

func (self *Editor) ExpandNode() {
	node := self.filetree.Selected()

	if node == nil {
		return
	}

	if node.isDir {
		self.filetree.Toggle()
		return
	}

	self.buffer = NewBufferFromPath(node.path)
	self.setMode(Mode__Normal)
}

func (self *Editor) Run() {
	S := self.tui.screen
	defer S.Fini()

	go func() {
		for {
			Events <- S.PollEvent()
		}
	}()

	for !self.exit {
		event := <-Events

		self.compute(event)
		self.tui.Rehydrate()
	}
}

func (self *Editor) AttachIface(tui *TUI) {
	self.tui = tui
}

func (self *Editor) GetModeAsStr() string {
	modes := map[Mode]string{
		Mode__Normal:  "NORMAL",
		Mode__Insert:  "INSERT",
		Mode__Visual:  "VISUAL",
		Mode__Command: "COMMAND",
		Mode__Goto:    "GOTO",
		Mode__Tree:    "TREE",
	}

	return modes[self.mode]
}

func (self *Editor) GetCursor() *Cursor {
	return self.buffer.cursor
}

func (self *Editor) GetCurline() *Line {
	cursor := self.GetCursor()

	if self.buffer.lines <= cursor.y {
		self.addNewline()
	}

	return self.buffer.curline
}

func (self *Editor) computeNormalMode(event *tcell.EventKey) {
	switch event.Key() {
	case tcell.KeyCtrlB:
		self.toggleFileTree()
	case tcell.KeyRune:
		switch event.Rune() {
		case 'i':
			EDITOR.setMode(Mode__Insert)
		case 'v':
			EDITOR.setMode(Mode__Visual)
		case ':':
			self.cmdline = append(self.cmdline, ':')
			EDITOR.setMode(Mode__Command)
		case 'g':
			EDITOR.setMode(Mode__Goto)
		case 'k':
			EDITOR.mvCursor(Movement__Up)
		case 'l':
			EDITOR.mvCursor(Movement__Right)
		case 'j':
			EDITOR.mvCursor(Movement__Down)
		case 'h':
			EDITOR.mvCursor(Movement__Left)
		case '$':
			GotoEndLine()
		case '0':
			GotoStartLine()
		}
	}
}

func (self *Editor) computeInsertMode(event *tcell.EventKey) {
	switch event.Key() {
	case tcell.KeyEscape:
		self.setMode(Mode__Normal)
	case tcell.KeyEnter:
		cursor := self.GetCursor()
		cursor.From(0, cursor.y+1)
		self.addNewline()
	case tcell.KeyBackspace, tcell.KeyBackspace2:
		self.rmChar("backward")
		self.mvCursor(Movement__Left)
	case tcell.KeyDelete:
		self.rmChar("forward")
	case tcell.KeyRune:
		self.addChar(event.Rune())
	}
}

func (self *Editor) computeVisualMode(event *tcell.EventKey) {
	switch event.Key() {
	case tcell.KeyCtrlB:
		self.toggleFileTree()
	case tcell.KeyEscape:
		self.setMode(Mode__Normal)
	case tcell.KeyRune:
		switch event.Rune() {
		case 'k':
			EDITOR.mvCursor(Movement__Up)
		case 'l':
			EDITOR.mvCursor(Movement__Right)
		case 'j':
			EDITOR.mvCursor(Movement__Down)
		case 'h':
			EDITOR.mvCursor(Movement__Left)
		case '$':
			GotoEndLine()
		case '0':
			GotoStartLine()
		}
	}
}

func (self *Editor) computeGotoMode(event *tcell.EventKey) {
	switch event.Key() {
	case tcell.KeyEscape:
		self.setMode(Mode__Normal)
	case tcell.KeyRune:
		switch event.Rune() {
		case 'g':
			GotoStartFile()
		case 'G':
			GotoEndFile()
		}
		self.setMode(Mode__Normal)
	}
}

func (self *Editor) computeCommandMode(event *tcell.EventKey) {
	switch event.Key() {
	case tcell.KeyEscape:
		self.setMode(Mode__Normal)
	case tcell.KeyEnter:
		self.exeCmd()
	case tcell.KeyBackspace, tcell.KeyBackspace2:
		self.cmdline = self.cmdline[:len(self.cmdline)-1]
	case tcell.KeyRune:
		ch := event.Rune()
		self.cmdline = append(self.cmdline, ch)
	}
}

func (self *Editor) computeTreeMode(event *tcell.EventKey) {
	switch event.Key() {
	case tcell.KeyCtrlB:
		self.toggleFileTree()
	case tcell.KeyEscape:
		self.toggleFileTree()
		self.setMode(Mode__Normal)
	case tcell.KeyUp:
		self.filetree.Up()
	case tcell.KeyDown:
		self.filetree.Down()
	case tcell.KeyLeft:
		self.filetree.CollapseNode()
	case tcell.KeyRight, tcell.KeyEnter:
		self.ExpandNode()
	case tcell.KeyRune:
		switch event.Rune() {
		case 'k':
			self.filetree.Up()
		case 'j':
			self.filetree.Down()
		case 'h':
			self.filetree.CollapseNode()
		case 'l':
			fallthrough
		case 'o':
			self.ExpandNode()
		case ':':
			self.toggleFileTree()
			self.cmdline = append(self.cmdline, ':')
			self.setMode(Mode__Command)
		}
	}
}

func (self *Editor) compute(event tcell.Event) {
	S := self.tui.screen

	switch event := event.(type) {
	case *tcell.EventResize:
		S.Sync()
	case *tcell.EventMouse:
		switch event.Buttons() {
		case tcell.WheelUp:
			// self.scrollUp()
		case tcell.WheelDown:
			// self.scrollDown()
		}
	case *tcell.EventKey:
		switch self.mode {
		case Mode__Normal:
			self.computeNormalMode(event)
		case Mode__Insert:
			self.computeInsertMode(event)
		case Mode__Visual:
			self.computeVisualMode(event)
		case Mode__Goto:
			self.computeGotoMode(event)
		case Mode__Command:
			self.computeCommandMode(event)
		case Mode__Tree:
			self.computeTreeMode(event)
		}
	}
}

func (self *Editor) mvCursor(move Movement) {
	cursor := self.GetCursor()

	switch move {
	case Movement__Up:
		if self.buffer.curline.prev != nil {
			cursor.Up()
			self.buffer.curline = self.buffer.curline.prev
		}
	case Movement__Right:
		cursor.Right()
	case Movement__Down:
		if self.buffer.curline.next != nil {
			self.buffer.curline = self.buffer.curline.next
			cursor.Down()
		}
	case Movement__Left:
		cursor.Left()
	}
}

func (self *Editor) setMode(mode Mode) {
	self.mode = mode
}

func (self *Editor) addNewline() {
	cursor := self.GetCursor()
	line := NewLine(cursor.y)

	if cursor.y == 0 {
		// TODO: self.buffer.head.next = self.buffer.head typing 'O'
		self.buffer.headline = &line
		self.buffer.tailline = &line
	} else if cursor.y == self.buffer.lines {
		self.buffer.curline.next = &line
		line.prev = self.buffer.curline
		self.buffer.tailline = &line
	} else {
		line.prev = self.buffer.curline
		line.next = self.buffer.curline.next
		self.buffer.curline.next = &line
		self.buffer.tailline = &line
	}

	self.buffer.curline = &line
	self.buffer.lines += 1
}

func (self *Editor) addChar(ch rune) {
	line := self.GetCurline()
	line.data = append(line.data, ch)
	self.buffer.dirty = true
	self.mvCursor(Movement__Right)
}

func (self *Editor) rmChar(mode string) {
	cursor := self.GetCursor()
	line := self.GetCurline()

	if strings.Compare(mode, "backward") == 0 && cursor.x > 0 {
		pre := line.data[:cursor.x-1]
		su := line.data[cursor.x:]

		line.data = append(pre, su...)
	} else if strings.Compare(mode, "forward") == 0 && cursor.x < int64(len(line.data)) {
		pre := line.data[:cursor.x]
		su := line.data[cursor.x+1:]

		line.data = append(pre, su...)
	}
}

func (self *Editor) ComputeNumLineOffset() int64 {
	nline := self.buffer.lines

	x := int64(0)

	for nline != 0 {
		nline /= 10
		x += 1
	}

	return x + 1
}

func (self *Editor) exeCmd() {
	cmd := string(self.cmdline[1:])

	if strings.Compare("q", cmd) == 0 || strings.Compare("Q", cmd) == 0 {
		if self.buffer.dirty {
			// TODO: show a warning
		}

		self.exit = true
	} else if strings.Compare("q!", cmd) == 0 || strings.Compare("Q!", cmd) == 0 {
		self.exit = true
	}

	self.cmdline = self.cmdline[:0]
	self.setMode(Mode__Normal)
}
