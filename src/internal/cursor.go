// Copyright (c) The Quickware Editor, 2026

package internal

type Offset struct {
	x int64
	y int64
}

type Cursor struct {
	x      int64
	y      int64
	offset Offset
}

func NewCursor() *Cursor {
	cursor := &Cursor{
		x: 0,
		y: 0,
		offset: Offset{
			x: 0,
			y: 0,
		},
	}

	return cursor
}

func (self *Cursor) From(x int64, y int64) {
	self.x = x
	self.y = y
}

func (self *Cursor) Up() {
	if self.y > 0 {
		self.y -= 1
	}
}

func (self *Cursor) Right() {
	line := EDITOR.GetCurline()

	if int64(len(line.data)) > self.x {
		self.x += 1
	}
}

func (self *Cursor) Down() {
	self.y += 1
}

func (self *Cursor) Left() {
	if self.x > 0 {
		self.x -= 1
	}
}
