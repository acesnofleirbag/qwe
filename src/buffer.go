package main

type Buffer struct {
	dirty  bool
	lines  int64
	data   [][]rune
	cursor Cursor
	prev   *Buffer
	next   *Buffer
}

func NewBuffer() Buffer {
	data := make([][]rune, 0)

	return Buffer{
		dirty:  false,
		lines:  0,
		data:   data,
		cursor: NewCursor(),
		prev:   nil,
		next:   nil,
	}
}

func (self *Buffer) BufferFromFile(fname string) Buffer {
	return Buffer{
		dirty:  false,
		lines:  0,
		data:   make([][]rune, 1),
		cursor: NewCursor(),
		next:   nil,
	}
}

func (self *Buffer) IsEmpty() bool {
	return self.lines == 0
}
