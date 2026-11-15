// Copyright (c) The Quickware Editor, 2026

package internal

import (
	"fmt"
	"strings"

	"github.com/gdamore/tcell/v2"
)

var Events chan tcell.Event

type TUI struct {
	screen tcell.Screen
}

func NewScreen() *TUI {
	S, err := tcell.NewScreen()
	if err != nil {
		panic("qwe: error to instantiate a tcell screen")
	}

	err = S.Init()
	if err != nil {
		panic("qwe: error initing tcell screen")
	}

	S.SetStyle(tcell.StyleDefault.Background(tcell.ColorReset).Foreground(tcell.ColorReset))
	S.SetCursorStyle(tcell.CursorStyleBlinkingBlock)
	S.EnableMouse()
	S.Clear()
	S.Sync()

	Events = make(chan tcell.Event)

	tui := &TUI{
		screen: S,
	}

	return tui
}

func drawText(S tcell.Screen, x int, y int, text string, style tcell.Style) {
	col := 0

	for _, ch := range text {
		S.SetContent(x+col, y, ch, nil, style)
		col++
	}
}

func (self *TUI) rehydrateFileTree() {
	tree := EDITOR.filetree

	if tree == nil || !tree.show {
		return
	}

	S := self.screen
	_, y_max := S.Size()
	height := y_max - 1 // NOTE: keep the status line free

	tree.ComputeScrollOffset(height)

	focused := EDITOR.mode == Mode__Tree
	border := tcell.StyleDefault.Foreground(tcell.ColorGray)

	for line := range height {
		idx := tree.scroll + line

		style := tcell.StyleDefault
		text := ""

		if idx < len(tree.visible) {
			node := tree.visible[idx]
			icon := "  "

			if node.isDir {
				icon = "> "

				if node.expanded {
					icon = "/ "
				}

				style = style.Foreground(tcell.ColorLightGray).Bold(true)
			}

			text = strings.Repeat("  ", node.depth) + icon + node.name

			if idx == tree.cursor {
				if focused {
					style = style.Reverse(true)
				} else {
					style = style.Underline(true)
				}
			}
		}

		runes := []rune(text)

		if len(runes) > tree.width {
			runes = append(runes[:tree.width-1], '…')
		}

		for len(runes) < tree.width {
			runes = append(runes, ' ')
		}

		drawText(S, 0, line, string(runes), style)
		S.SetContent(tree.width, line, '|', nil, border)
	}
}

func (self *TUI) rehydrateEditor() {
	S := self.screen

	cursor := EDITOR.GetCursor()
	fileTreeWidth := EDITOR.GetFileTreeWidth()

	if !EDITOR.buffer.IsEmpty() {
		line := EDITOR.buffer.curline

		// render current line
		render := fmt.Sprintf("%v %v", line.nline+1, string(line.data))

		drawText(S, fileTreeWidth, int(line.nline), render, tcell.StyleDefault)

		// render prev lines visible on render area
		for prev := line.prev; prev != nil && line.nline >= cursor.offset.y; prev = prev.prev {
			render := fmt.Sprintf("%v %v", prev.nline+1, string(prev.data))

			drawText(S, fileTreeWidth, int(prev.nline), render, tcell.StyleDefault)
		}

		// render next lines visible on render area
		for next := line.next; next != nil && line.nline <= cursor.y+cursor.offset.y; next = next.next {
			render := fmt.Sprintf("%v %v", next.nline+1, string(next.data))

			drawText(S, fileTreeWidth, int(next.nline), render, tcell.StyleDefault)
		}
	}
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

func (self *TUI) rehydrateCursor() {
	S := self.screen

	if EDITOR.mode == Mode__Tree {
		S.HideCursor()
		return
	}

	nlineOff := EDITOR.ComputeNumLineOffset()
	cursor := EDITOR.GetCursor()
	fileTreeWidth := EDITOR.GetFileTreeWidth()

	S.ShowCursor(fileTreeWidth+int(cursor.x-cursor.offset.x)+int(nlineOff), int(cursor.y-cursor.offset.y))
}

func (self *TUI) Rehydrate() {
	S := self.screen

	S.Clear()

	self.rehydrateFileTree()
	self.rehydrateEditor()
	self.rehydrateStatusline()
	self.rehydrateCursor()

	S.Show()
}
