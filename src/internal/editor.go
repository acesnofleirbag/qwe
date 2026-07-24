package internal

import (
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
	exit     bool
	mode     Mode
	cmdline  []rune
	finder   Finder
	buffer   Buffer
	timeline Timeline
	tui      TUI
	config   map[string]any
}

var EDITOR = Editor{
	exit:     false,
	mode:     Mode__Normal,
	cmdline:  make([]rune, 0),
	finder:   NewFinder(),
	buffer:   NewBuffer(),
	timeline: NewTimeline(),

	config: CONFIG,
}

func (self *Editor) OpenPath(fname string) {
	self.buffer = NewBufferFromPath(fname)
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

func (self *Editor) AttachIface(tui TUI) {
	self.tui = tui
}

func (self *Editor) GetModeAsStr() string {
	modes := map[Mode]string{
		Mode__Normal:  "NORMAL",
		Mode__Insert:  "INSERT",
		Mode__Visual:  "VISUAL",
		Mode__Command: "COMMAND",
		Mode__Goto:    "GOTO",
	}

	return modes[self.mode]
}

func (self *Editor) GetCursor() *Cursor {
	return &self.buffer.cursor
}

func (self *Editor) GetCurrentLine() *Line {
	cursor := self.GetCursor()

	if self.buffer.lines <= cursor.y {
		self.addNewLine()
	}

	return self.buffer.curline
}

func (self *Editor) compute(event tcell.Event) {
	S := self.tui.screen

	switch event := event.(type) {
	case *tcell.EventResize:
		S.Sync()
		break
	case *tcell.EventMouse:
		switch event.Buttons() {
		case tcell.WheelUp:
			// self.scrollUp()
			break
		case tcell.WheelDown:
			// self.scrollDown()
			break
		}
		break
	case *tcell.EventKey:
		switch event.Key() {
		case tcell.KeyUp:
			if self.config["useArrows"] == true {
				self.mvCursor(Movement__Up)
			}
			break
		case tcell.KeyRight:
			if self.config["useArrows"] == true {
				self.mvCursor(Movement__Right)
			}
			break
		case tcell.KeyDown:
			if self.config["useArrows"] == true {
				self.mvCursor(Movement__Down)
			}
			break
		case tcell.KeyLeft:
			if self.config["useArrows"] == true {
				self.mvCursor(Movement__Left)
			}
			break
		case tcell.KeyEscape:
			if self.mode != Mode__Normal {
				self.setMode(Mode__Normal)
			}
			break
		case tcell.KeyEnter:
			switch self.mode {
			case Mode__Insert:
				cursor := self.GetCursor()
				cursor.From(0, cursor.y+1)
				self.addNewLine()
				break
			case Mode__Command:
				self.exeCmd()
				break
			}
			break
		case tcell.KeyBackspace, tcell.KeyBackspace2:
			if self.mode == Mode__Insert {
				self.rmChar("backward")
				self.mvCursor(Movement__Left)
			} else if self.mode == Mode__Command {
				self.cmdline = self.cmdline[:len(self.cmdline)-1]
			}
			break
		case tcell.KeyDelete:
			if self.mode == Mode__Insert {
				self.rmChar("forward")
			}
			break
		case tcell.KeyRune:
			ch := event.Rune()

			switch self.mode {
			case Mode__Goto:
				switch ch {
				case 'g':
					GotoStartFile()
					break
				case 'G':
					GotoEndFile()
					break
				}
				self.setMode(Mode__Normal)
				break
			case Mode__Normal, Mode__Visual:
				switch ch {
				case 'i':
					EDITOR.setMode(Mode__Insert)
					break
				case 'v':
					EDITOR.setMode(Mode__Visual)
					break
				case ':':
					self.cmdline = append(self.cmdline, ':')
					EDITOR.setMode(Mode__Command)
					break
				case 'g':
					EDITOR.setMode(Mode__Goto)
					break
				case 'k':
					EDITOR.mvCursor(Movement__Up)
					break
				case 'l':
					EDITOR.mvCursor(Movement__Right)
					break
				case 'j':
					EDITOR.mvCursor(Movement__Down)
					break
				case 'h':
					EDITOR.mvCursor(Movement__Left)
					break
				case '$':
					GotoEndLine()
					break
				case '0':
					GotoStartLine()
					break
				}
				break
			case Mode__Insert:
				self.addChar(ch)
				break
			case Mode__Command:
				self.cmdline = append(self.cmdline, ch)
				break
			}
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
		break
	case Movement__Right:
		cursor.Right()
		break
	case Movement__Down:
		if self.buffer.curline.next != nil {
			self.buffer.curline = self.buffer.curline.next
			cursor.Down()
		}
		break
	case Movement__Left:
		cursor.Left()
		break
	}
}

func (self *Editor) setMode(mode Mode) {
	self.mode = mode
}

func (self *Editor) addNewLine() {
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
	line := self.GetCurrentLine()
	line.data = append(line.data, ch)
	self.buffer.dirty = true
	self.mvCursor(Movement__Right)
}

func (self *Editor) rmChar(mode string) {
	cursor := self.GetCursor()
	line := self.GetCurrentLine()

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

func (self *Editor) ComputeNlineOffset() int64 {
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
			self.exit = true
		} else {
			self.exit = true
		}
	}

	self.cmdline = self.cmdline[:0]
	self.setMode(Mode__Normal)
}
