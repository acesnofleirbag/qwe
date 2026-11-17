// Copyright (c) The Quickware Editor, 2026

package text

import (
	"bufio"
	"os"
	"qwe/internal"
)

type Line struct {
	num  int64
	data []rune
	prev *Line
	next *Line
}

func NewLine(numline int64) Line {
	return Line{
		num:  numline,
		data: make([]rune, 0),
		prev: nil,
		next: nil,
	}
}

func (self *Line) GetNum() int64 {
	return self.num
}

func (self *Line) GetData() []rune {
	return self.data
}

func (self *Line) GetPrev() *Line {
	return self.prev
}

func (self *Line) GetNext() *Line {
	return self.next
}

type Buffer struct {
	dirty    bool
	lines    int64
	cursor   *Cursor
	headline *Line
	curline  *Line
	tailline *Line
	prev     *Buffer
	next     *Buffer
}

func NewBuffer() *Buffer {
	line := NewLine(0)

	buf := &Buffer{
		lines:    1,
		dirty:    false,
		cursor:   NewCursor(),
		headline: &line,
		curline:  &line,
		tailline: &line,
		prev:     nil,
		next:     nil,
	}

	return buf
}

func NewBufferFromPath(path string) *Buffer {
	buf := &Buffer{
		dirty:    false,
		headline: nil,
		curline:  nil,
		tailline: nil,
		cursor:   NewCursor(),
		prev:     nil,
		next:     nil,
	}

	f, err := os.Open(path)
	if err != nil {
		internal.Log("[ERROR] buffer.NewBufferFromPath: cannot open path")
	}

	defer f.Close()

	scan := bufio.NewScanner(f)

	var i int64 = 0
	for scan.Scan() {
		text := []rune(scan.Text())

		line := NewLine(i)
		line.data = text

		if i == 0 {
			buf.headline = &line
			buf.curline = &line
		} else {
			line.prev = buf.tailline
			buf.tailline.next = &line
		}

		buf.tailline = &line
		i++
	}

	buf.curline = buf.headline
	buf.lines = i

	if err := scan.Err(); err != nil {
		panic("[ERROR] buffer.NewBufferFromPath: cannot scan file")
	}

	return buf
}

func (self *Buffer) IsEmpty() bool {
	return self.lines == 0
}

func (self *Buffer) IsDirty() bool {
	return self.dirty
}

func (self *Buffer) GetLines() int64 {
	return self.lines
}

func (self *Buffer) GetCursor() *Cursor {
	return self.cursor
}

func (self *Buffer) GetCurLine() *Line {
	return self.curline
}

func (self *Buffer) GotoPrevLine() bool {
	if self.curline.prev == nil {
		return false
	}

	self.curline = self.curline.prev
	return true
}

func (self *Buffer) GotoNextLine() bool {
	if self.curline.next == nil {
		return false
	}

	self.curline = self.curline.next
	return true
}

func (self *Buffer) GotoHeadLine() {
	self.cursor.From(0, 0)
	self.curline = self.headline
}

func (self *Buffer) GotoTailLine() {
	self.cursor.From(0, self.lines-1)
	self.curline = self.tailline
}

func (self *Buffer) AddNewline() {
	line := NewLine(self.cursor.y)

	if self.cursor.y == 0 {
		self.headline = &line
		self.tailline = &line
	} else if self.cursor.y == self.lines {
		self.curline.next = &line
		line.prev = self.curline
		self.tailline = &line
	} else {
		line.prev = self.curline
		line.next = self.curline.next
		self.curline.next = &line
		self.tailline = &line
	}

	self.curline = &line
	self.lines += 1
}

func (self *Buffer) InsertRune(ch rune) {
	self.curline.data = append(self.curline.data, ch)
	self.dirty = true
}

// In case where forward is true compute a Delete key otherwise compute a
// Backspace key
func (self *Buffer) DeleteRune(forward bool) {
	line := self.curline
	x := self.cursor.x

	if !forward && x > 0 {
		pre := line.data[:x-1]
		su := line.data[x:]

		line.data = append(pre, su...)
	} else if forward && x < int64(len(line.data)) {
		pre := line.data[:x]
		su := line.data[x+1:]

		line.data = append(pre, su...)
	}
}
