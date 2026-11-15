// Copyright (c) The Quickware Editor, 2026

package internal

import (
	"bufio"
	"os"
)

type Line struct {
	nline int64
	data  []rune
	prev  *Line
	next  *Line
}

func NewLine(nline int64) Line {
	return Line{
		nline: nline,
		data:  make([]rune, 0),
		prev:  nil,
		next:  nil,
	}
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
		Log("[ERROR] buffer.NewBufferFromPath: cannot open path")
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
