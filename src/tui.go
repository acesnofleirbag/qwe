package main

import (
	"fmt"
	"qwe/src/guard"

	"github.com/gdamore/tcell/v2"
)

var Events chan tcell.Event

type TUI struct {
	screen tcell.Screen
}

func NewScreen() TUI {
	S, err := tcell.NewScreen()
	guard.Err(err)

	err = S.Init()
	guard.Err(err)

	S.SetStyle(tcell.StyleDefault.Background(tcell.ColorReset).Foreground(tcell.ColorReset))
	S.SetCursorStyle(tcell.CursorStyleBlinkingBlock)
	S.EnableMouse()
	S.Clear()
	S.Sync()

	Events = make(chan tcell.Event)

	return TUI{
		screen: S,
	}
}

func (self *TUI) Rehydrate() {
	S := self.screen

	S.Clear()

	cursor := EDITOR.GetCursor()
	S.ShowCursor(int(cursor.x-cursor.offset.x), int(cursor.y-cursor.offset.y))

	// FIXME
	if !EDITOR.buffer.IsEmpty() {
		for nline, line := range EDITOR.buffer.data {
			line := fmt.Sprintf("%v %v", nline, string(line))

			for col, ch := range line {
				S.SetContent(col+int(EDITOR.GetNlineOffset())+1, nline, ch, nil, tcell.StyleDefault)
			}
		}
	}

	self.rehydrateStatusline()

	S.Show()
}

func (self *TUI) rehydrateStatusline() {
	S := self.screen

	maxx, maxy := S.Size()
	cursor := EDITOR.GetCursor()

	mode := fmt.Sprintf("[%v]", EDITOR.GetModeAsStr())
	icursor := fmt.Sprintf("%v:%v", cursor.x, cursor.y)

	gap := maxx - len(mode) - len(icursor)

	if EDITOR.mode == Mode__Command {
		// render cmdline
		for col, ch := range EDITOR.cmdline {
			S.SetContent(col, maxy-1, ch, nil, tcell.StyleDefault)
		}
	} else {
		// render mode
		for col, ch := range mode {
			S.SetContent(col, maxy-1, ch, nil, tcell.StyleDefault)
		}
	}

	// render gap
	for col := 0; col < gap; col++ {
		S.SetContent(col+len(mode), maxy-1, ' ', nil, tcell.StyleDefault)
	}

	// render cursor info
	for col, ch := range icursor {
		S.SetContent(col+len(mode)+gap, maxy-1, ch, nil, tcell.StyleDefault)
	}
}
