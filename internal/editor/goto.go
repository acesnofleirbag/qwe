// Copyright (c) The Quickware Editor, 2026

package editor

func (self *Editor) GotoStartLine() {
	cursor := self.GetCursor()
	cursor.From(0, cursor.GetY())
}

func (self *Editor) GotoEndLine() {
	cursor := self.GetCursor()
	line := self.GetCurline()

	if line != nil {
		cursor.From(int64(len(line.GetData())), cursor.GetY())
	}
}

func (self *Editor) GotoStartFile() {
	self.buffer.GotoHeadLine()
}

func (self *Editor) GotoEndFile() {
	self.buffer.GotoTailLine()
}
