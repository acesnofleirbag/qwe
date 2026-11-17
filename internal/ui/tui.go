// Copyright (c) The Quickware Editor, 2026

package ui

import (
	"fmt"
	"qwe/internal/editor"
	"strings"

	"github.com/gdamore/tcell/v2"
)

var EDITOR = editor.EDITOR

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

	tui := &TUI{
		screen: S,
	}

	return tui
}

func (self *TUI) GetScreen() tcell.Screen {
	return self.screen
}

func (self *TUI) drawText(S tcell.Screen, x int, y int, text string, style tcell.Style) {
	col := 0

	for _, ch := range text {
		S.SetContent(x+col, y, ch, nil, style)
		col++
	}
}

func (self *TUI) rehydrateFileTree() {
	tree := EDITOR.GetFileTree()

	if tree == nil || !tree.IsShown() {
		return
	}

	S := self.screen
	_, y_max := S.Size()
	height := y_max - 1 // NOTE: keep the status line free

	tree.ComputeScrollOffset(height)

	focused := EDITOR.GetMode() == editor.Mode__Tree
	border := tcell.StyleDefault.Foreground(tcell.ColorGray)

	for line := range height {
		idx := tree.GetScroll() + line

		style := tcell.StyleDefault
		text := ""

		if idx < len(tree.IsVisible()) {
			node := tree.IsVisible()[idx]
			icon := "  "

			if node.IsDir() {
				icon = "> "

				if node.IsExpanded() {
					icon = "/ "
				}

				style = style.Foreground(tcell.ColorLightGray).Bold(true)
			}

			text = strings.Repeat("  ", node.GetDepth()) + icon + node.GetName()

			if idx == tree.GetCursor() {
				if focused {
					style = style.Reverse(true)
				} else {
					style = style.Underline(true)
				}
			}
		}

		runes := []rune(text)

		width := tree.GetWidth()

		if len(runes) > width {
			runes = append(runes[:width-1], '…')
		}

		for len(runes) < width {
			runes = append(runes, ' ')
		}

		self.drawText(S, 0, line, string(runes), style)
		S.SetContent(width, line, '|', nil, border)
	}
}

func (self *TUI) rehydrateEditor() {
	S := self.screen

	cursor := EDITOR.GetCursor()
	fileTreeWidth := EDITOR.GetFileTreeWidth()

	if buf := EDITOR.GetBuffer(); !buf.IsEmpty() {
		line := buf.GetCurLine()

		// render current line
		render := fmt.Sprintf("%v %v", line.GetNum()+1, string(line.GetData()))

		self.drawText(S, fileTreeWidth, int(line.GetNum()), render, tcell.StyleDefault)

		// render prev lines visible on render area
		for prev := line.GetPrev(); prev != nil && line.GetNum() >= cursor.GetOffsetY(); prev = prev.GetPrev() {
			render := fmt.Sprintf("%v %v", prev.GetNum()+1, string(prev.GetData()))

			self.drawText(S, fileTreeWidth, int(prev.GetNum()), render, tcell.StyleDefault)
		}

		// render next lines visible on render area
		for next := line.GetNext(); next != nil && line.GetNum() <= cursor.GetY()+cursor.GetOffsetY(); next = next.GetNext() {
			render := fmt.Sprintf("%v %v", next.GetNum()+1, string(next.GetData()))

			self.drawText(S, fileTreeWidth, int(next.GetNum()), render, tcell.StyleDefault)
		}
	}
}

func (self *TUI) rehydrateStatusline() {
	S := self.screen

	x_max, y_max := S.Size()
	cursor := EDITOR.GetCursor()

	mode := fmt.Sprintf("[%v]", EDITOR.GetModeAsStr())
	icursor := fmt.Sprintf("%v:%v", cursor.GetX(), cursor.GetY())

	gap := x_max - len(mode) - len(icursor)

	if EDITOR.GetMode() == editor.Mode__Command {
		// render cmdline
		for col, ch := range EDITOR.GetCmdline() {
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

	if EDITOR.GetMode() == editor.Mode__Tree {
		S.HideCursor()
		return
	}

	nlineOff := EDITOR.ComputeNumLineOffset()
	cursor := EDITOR.GetCursor()
	fileTreeWidth := EDITOR.GetFileTreeWidth()

	S.ShowCursor(fileTreeWidth+int(cursor.GetX()-cursor.GetOffsetX())+int(nlineOff), int(cursor.GetY()-cursor.GetOffsetY()))
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
