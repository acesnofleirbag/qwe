package internal

import (
	"fmt"
	"os"
	"path/filepath"
)

const FILEPATH = "qwe.debug"

func Debug(format string, args ...any) {
	pwd, err := os.Getwd()

	if err != nil {
		panic("qwe: error getting pwd")
	}

	f, err := os.OpenFile(filepath.Join(pwd, FILEPATH), os.O_WRONLY|os.O_CREATE|os.O_APPEND, 0644)
	defer f.Close()

	if err != nil {
		panic("qwe: error opening debug file")
	}

	buf := fmt.Appendf(make([]byte, 0), format, args...)

	_, err = f.Write(append(buf, '\n'))

	if err != nil {
		panic("qwe: error writing debug file")
	}
}
