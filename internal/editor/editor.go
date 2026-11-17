// Copyright (c) The Quickware Editor, 2026

package editor

import (
	"os"
	"qwe/internal"
	"qwe/internal/filetree"
	"qwe/internal/text"
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

type View interface {
	GetScreen() tcell.Screen
	Rehydrate()
}

type Editor struct {
	config   map[string]any
	mode     Mode
	cmdline  []rune
	buffer   *text.Buffer
	finder   *Finder
	timeline *text.Timeline
	filetree *filetree.FileTree
	view     View
	exit     bool
}

// Global editor state
var EDITOR = &Editor{
	config:   internal.CONFIG,
	mode:     Mode__Normal,
	cmdline:  make([]rune, 0),
	buffer:   text.NewBuffer(),
	finder:   NewFinder(),
	timeline: text.NewTimeline(),
	exit:     false,
}

func (self *Editor) Run() {
	S := self.view.GetScreen()
	defer S.Fini()

	events := make(chan tcell.Event)

	go func() {
		for {
			events <- S.PollEvent()
		}
	}()

	for !self.exit {
		event := <-events

		self.compute(event)
		self.view.Rehydrate()
	}
}

func (self *Editor) GetMode() Mode {
	return self.mode
}

func (self *Editor) GetCmdline() []rune {
	return self.cmdline
}

func (self *Editor) GetBuffer() *text.Buffer {
	return self.buffer
}

func (self *Editor) GetFileTree() *filetree.FileTree {
	return self.filetree
}

func (self *Editor) AttachView(view View) {
	self.view = view
}

func (self *Editor) OpenPath(fname string) {
	info, err := os.Stat(fname)

	if err != nil {
		internal.Log("[ERROR] editor.OpenPath: cannot stat path")
	}

	if info.IsDir() {
		self.filetree = filetree.NewFileTree(fname)
		self.toggleFileTree()
	} else {
		self.buffer = text.NewBufferFromPath(fname)
	}
}

func (self *Editor) GetFileTreeWidth() int {
	if self.filetree == nil || !self.filetree.IsShown() {
		return 0
	}

	return self.filetree.GetWidth() + 1
}

func (self *Editor) toggleFileTree() {
	if self.filetree == nil {
		self.filetree = filetree.NewFileTree(".")
	}

	self.filetree.SetShown(!self.filetree.IsShown())

	if self.filetree.IsShown() {
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

	if node.IsDir() {
		self.filetree.Toggle()
		return
	}

	self.buffer = text.NewBufferFromPath(node.GetPath())
	self.setMode(Mode__Normal)
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

func (self *Editor) GetCursor() *text.Cursor {
	return self.buffer.GetCursor()
}

func (self *Editor) GetCurline() *text.Line {
	cursor := self.GetCursor()

	if self.buffer.GetLines() <= cursor.GetY() {
		self.addNewline()
	}

	return self.buffer.GetCurLine()
}

func (self *Editor) computeNormalMode(event *tcell.EventKey) {
	switch event.Key() {
	case tcell.KeyCtrlB:
		self.toggleFileTree()
	case tcell.KeyRune:
		switch event.Rune() {
		case 'i':
			self.setMode(Mode__Insert)
		case 'v':
			self.setMode(Mode__Visual)
		case ':':
			self.cmdline = append(self.cmdline, ':')
			self.setMode(Mode__Command)
		case 'g':
			self.setMode(Mode__Goto)
		case 'k':
			self.mvCursor(Movement__Up)
		case 'l':
			self.mvCursor(Movement__Right)
		case 'j':
			self.mvCursor(Movement__Down)
		case 'h':
			self.mvCursor(Movement__Left)
		case '$':
			self.GotoEndLine()
		case '0':
			self.GotoStartLine()
		}
	}
}

func (self *Editor) computeInsertMode(event *tcell.EventKey) {
	switch event.Key() {
	case tcell.KeyEscape:
		self.setMode(Mode__Normal)
	case tcell.KeyEnter:
		cursor := self.GetCursor()
		cursor.From(0, cursor.GetY()+1)
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
			self.mvCursor(Movement__Up)
		case 'l':
			self.mvCursor(Movement__Right)
		case 'j':
			self.mvCursor(Movement__Down)
		case 'h':
			self.mvCursor(Movement__Left)
		case '$':
			self.GotoEndLine()
		case '0':
			self.GotoStartLine()
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
			self.GotoStartFile()
		case 'G':
			self.GotoEndFile()
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
		self.filetree.MoveCursorUp()
	case tcell.KeyDown:
		self.filetree.MoveCursorDown()
	case tcell.KeyLeft:
		self.filetree.CollapseNode()
	case tcell.KeyRight, tcell.KeyEnter:
		self.ExpandNode()
	case tcell.KeyRune:
		switch event.Rune() {
		case 'k':
			self.filetree.MoveCursorUp()
		case 'j':
			self.filetree.MoveCursorDown()
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
	S := self.view.GetScreen()

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
		if self.buffer.GotoPrevLine() {
			cursor.Up()
		}
	case Movement__Right:
		line := self.GetCurline()
		cursor.Right(int64(len(line.GetData())))
	case Movement__Down:
		if self.buffer.GotoNextLine() {
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
	self.buffer.AddNewline()
}

func (self *Editor) addChar(ch rune) {
	self.GetCurline()
	self.buffer.InsertRune(ch)
	self.mvCursor(Movement__Right)
}

func (self *Editor) rmChar(mode string) {
	self.GetCurline()
	self.buffer.DeleteRune(mode == "forward")
}

func (self *Editor) ComputeNumLineOffset() int64 {
	nline := self.buffer.GetLines()

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
		if self.buffer.IsDirty() {
			// TODO: show a warning
		}

		self.exit = true
	} else if strings.Compare("q!", cmd) == 0 || strings.Compare("Q!", cmd) == 0 {
		self.exit = true
	}

	// GC: optimization to not realloc the array
	clear(self.cmdline)
	self.cmdline = self.cmdline[:0]
	self.setMode(Mode__Normal)
}
