package internal

func GotoStartLine() {
	cursor := EDITOR.GetCursor()

	cursor.From(0, cursor.y)
}

func GotoEndLine() {
	cursor := EDITOR.GetCursor()
	line := EDITOR.GetCurrentLine()

	if line != nil {
		cursor.From(int64(len(line.data)), cursor.y)
	}
}

func GotoStartFile() {
	cursor := EDITOR.GetCursor()
	cursor.From(0, 0)

	EDITOR.buffer.curline = EDITOR.buffer.headline
}

func GotoEndFile() {
	cursor := EDITOR.GetCursor()
	cursor.From(0, EDITOR.buffer.lines-1)

	EDITOR.buffer.curline = EDITOR.buffer.tailline
}
