// Copyright (c) The Quickware Editor, 2026

package internal

const (
	FindMode__Forward = iota
	FindMode__Backward
)

type Finder struct {
	mode  int
	query string
}

func NewFinder() *Finder {
	finder := &Finder{
		mode:  FindMode__Forward,
		query: "",
	}

	return finder
}

func (self *Finder) FindForward() {}

func (self *Finder) FindBackward() {}
