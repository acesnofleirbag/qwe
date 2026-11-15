// Copyright (c) The Quickware Editor, 2026

package internal

import (
	"fmt"
	"os"
	"path/filepath"
)

func Log(format string, args ...any) {
	pwd, err := os.Getwd()

	if err != nil {
		return // don't crash the exec on err
	}

	f, err := os.OpenFile(filepath.Join(pwd, CONFIG["logfile"].(string)), os.O_WRONLY|os.O_CREATE|os.O_APPEND, 0644)
	defer f.Close()

	if err != nil {
		return // don't crash the exec on err
	}

	buf := fmt.Appendf(make([]byte, 0), format, args...)

	_, err = f.Write(append(buf, '\n'))

	if err != nil {
		return // don't crash the exec on err
	}
}
