// Copyright (c) The Quickware Editor, 2026

package internal

type LineMode int

const (
	LineMode__Normal = iota
	LineMode__Relative
)

// Sorted in alpha numerical mode
var CONFIG = map[string]any{
	"fileTreeIgnore": []string{".git"},
	"lineMode":       LineMode__Normal,
	"logfile":        "qwe.logfile",
}
