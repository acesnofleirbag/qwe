package main

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

type Config struct {
	lineMode  LineMode
	useArrows bool
}

type Editor struct {
	exit     bool
	mode     Mode
	cmdline  []rune
	finder   Finder
	buffer   Buffer
	timeline Timeline
	tui      TUI
	config   Config
}

func NewEditor() Editor {
	return Editor{
		exit:     false,
		mode:     Mode__Normal,
		cmdline:  make([]rune, 0),
		finder:   NewFinder(),
		buffer:   NewBuffer(),
		timeline: NewTimeline(),
	}
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

func (self *Editor) SetConfig(config Config) {
	self.config = config
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

func (self *Editor) GetCurrentLine() []rune {
	cursor := self.GetCursor()

	if self.buffer.lines <= cursor.y {
		self.addNewLine()
	}

	return self.buffer.data[cursor.y]
}

func (self *Editor) GetNlineByMode(mode int) {

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
			self.exit = true
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
			if self.config.useArrows {
				self.mvCursor(Movement__Up)
			}
			break
		case tcell.KeyRight:
			if self.config.useArrows {
				self.mvCursor(Movement__Right)
			}
			break
		case tcell.KeyDown:
			if self.config.useArrows {
				self.mvCursor(Movement__Down)
			}
			break
		case tcell.KeyLeft:
			if self.config.useArrows {
				self.mvCursor(Movement__Left)
			}
			break
		case tcell.KeyEscape:
			if self.mode != Mode__Normal {
				self.setMode(Mode__Normal)
			} else {
				self.exit = true
			}
			break
		case tcell.KeyEnter:
			switch self.mode {
			case Mode__Insert:
				cursor := self.GetCursor()

				cursor.From(0, cursor.y+1)
				break
			case Mode__Command:
				self.exeCmd()
				break
			}
			break
		case tcell.KeyBackspace, tcell.KeyBackspace2:
			if self.mode == Mode__Insert {
				self.rmChar()
			} else if self.mode == Mode__Command {
                self.cmdline = self.cmdline[:len(self.cmdline) - 1]
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
		cursor.Up()
		break
	case Movement__Right:
		cursor.Right()
		break
	case Movement__Down:
		cursor.Down()
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
	self.buffer.data = append(self.buffer.data, make([]rune, 0))
	self.buffer.lines += 1
}

func (self *Editor) addChar(ch rune) {
	cursor := self.GetCursor()
	line := self.GetCurrentLine()

	line = append(line, ch)
	self.buffer.data[cursor.y] = line

	self.buffer.dirty = true

	self.mvCursor(Movement__Right)
}

// FIXME
func (self *Editor) rmChar() {
	cursor := self.GetCursor()
	line := self.GetCurrentLine()

	line = append(line[cursor.x-1:], line[:cursor.x]...)
}

func (self *Editor) GetNlineOffset() int64 {
	nline := self.buffer.lines

	if nline == 0 {
		return 1
	}

	x := int64(0)

	for nline != 0 {
		nline /= 10
		x += 1
	}

	return x
}

func (self *Editor) exeCmd() {
	cmd := string(self.cmdline[1:])

	if strings.Compare("q", cmd) == 0 || strings.Compare("Q", cmd) == 0 {
		self.exit = true
	}

	self.cmdline = self.cmdline[:0]
	self.setMode(Mode__Normal)
}

func (self *Editor) checkByDirtyBuffer() {
	for i := &self.buffer; i.next != nil; i = i.next {
		// TODO
	}
}
