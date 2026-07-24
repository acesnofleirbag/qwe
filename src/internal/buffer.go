package internal

import (
	"bufio"
	"os"
)

type Line struct {
	nline int64
	data  []rune

	prev *Line
	next *Line
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
	headline *Line
	curline  *Line
	tailline *Line
	cursor   Cursor
	prev     *Buffer
	next     *Buffer
}

func NewBuffer() Buffer {
	line := NewLine(0)

	return Buffer{
		lines:    1,
		dirty:    false,
		cursor:   NewCursor(),
		headline: &line,
		curline:  &line,
		tailline: &line,
		prev:     nil,
		next:     nil,
	}
}

func NewBufferFromPath(path string) Buffer {
	buf := Buffer{
		dirty:    false,
		headline: nil,
		curline:  nil,
		tailline: nil,
		cursor:   NewCursor(),
		prev:     nil,
		next:     nil,
	}

	info, err := os.Stat(path)

	if err != nil {
		panic("qwe: error opening path")
	}

	if info.IsDir() {
		// TODO: treeview
	} else {
		f, err := os.Open(path)

		if err != nil {
			panic("qwe: error opening file")
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
			panic("qwe: error scanning file")
		}
	}

	return buf
}

func (self *Buffer) IsEmpty() bool {
	return self.lines == 0
}
