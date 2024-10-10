package main

const (
	FindMode__Forward = iota
	FindMode__Backward
)

type Finder struct {
	mode  int
	query string
}

func NewFinder() Finder {
	return Finder{
		mode:  FindMode__Forward,
		query: "",
	}
}

func (self *Finder) FindForward() {}

func (self *Finder) FindBackward() {}
