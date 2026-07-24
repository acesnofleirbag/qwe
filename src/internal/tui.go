package internal

import (
	"fmt"

	"github.com/gdamore/tcell/v2"
)

var Events chan tcell.Event

type TUI struct {
	screen tcell.Screen
}

func NewScreen() TUI {
	S, err := tcell.NewScreen()
	if err != nil {
		panic("qwe: error to intantiate a new screen")
	}

	err = S.Init()
	if err != nil {
		panic("qwe: error initing screen")
	}

	S.SetStyle(tcell.StyleDefault.Background(tcell.ColorReset).Foreground(tcell.ColorReset))
	S.SetCursorStyle(tcell.CursorStyleBlinkingBlock)
	S.EnableMouse()
	S.Clear()

	nlineOff := EDITOR.ComputeNlineOffset()

	cursor := EDITOR.GetCursor()
	S.ShowCursor(int(cursor.x-cursor.offset.x)+int(nlineOff), int(cursor.y-cursor.offset.y))

	S.Sync()

	Events = make(chan tcell.Event)

	return TUI{
		screen: S,
	}
}

func (self *TUI) Rehydrate() {
	S := self.screen

	S.Clear()

	nlineOff := EDITOR.ComputeNlineOffset()

	cursor := EDITOR.GetCursor()
	S.ShowCursor(int(cursor.x-cursor.offset.x)+int(nlineOff), int(cursor.y-cursor.offset.y))

	if !EDITOR.buffer.IsEmpty() {
		line := EDITOR.buffer.curline

		// render current line
		render := fmt.Sprintf("%v %v", line.nline+1, string(line.data))

		for col, ch := range render {
			S.SetContent(col, int(line.nline), ch, nil, tcell.StyleDefault)
		}

		// render prev lines visible on render area
		for prev := line.prev; prev != nil && line.nline >= cursor.offset.y; prev = prev.prev {
			render := fmt.Sprintf("%v %v", prev.nline+1, string(prev.data))

			for col, ch := range render {
				S.SetContent(col, int(prev.nline), ch, nil, tcell.StyleDefault)
			}
		}

		// render next lines visible on render area
		for next := line.next; next != nil && line.nline <= cursor.y+cursor.offset.y; next = next.next {
			render := fmt.Sprintf("%v %v", next.nline+1, string(next.data))

			for col, ch := range render {
				S.SetContent(col, int(next.nline), ch, nil, tcell.StyleDefault)
			}
		}
	}

	self.rehydrateStatusline()

	S.Show()
}

func (self *TUI) rehydrateStatusline() {
	S := self.screen

	x_max, y_max := S.Size()
	cursor := EDITOR.GetCursor()

	mode := fmt.Sprintf("[%v]", EDITOR.GetModeAsStr())
	icursor := fmt.Sprintf("%v:%v", cursor.x, cursor.y)

	gap := x_max - len(mode) - len(icursor)

	if EDITOR.mode == Mode__Command {
		// render cmdline
		for col, ch := range EDITOR.cmdline {
			S.SetContent(col, y_max-1, ch, nil, tcell.StyleDefault)
		}
	} else {
		// render mode
		for col, ch := range mode {
			S.SetContent(col, y_max-1, ch, nil, tcell.StyleDefault)
		}
	}

	// render gap
	for col := range gap {
		S.SetContent(col+len(mode), y_max-1, ' ', nil, tcell.StyleDefault)
	}

	// render cursor info
	for col, ch := range icursor {
		S.SetContent(col+len(mode)+gap, y_max-1, ch, nil, tcell.StyleDefault)
	}
}
