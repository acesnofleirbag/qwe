// Copyright (c) The Quickware Editor, 2026

package internal

type Timeline struct {
	data []string
}

func NewTimeline() *Timeline {
	timeline := &Timeline{
		data: make([]string, 0),
	}

	return timeline
}

func (self *Timeline) Push(data string) {
	self.data = append(self.data, data)
}

func (self *Timeline) Pop() string {
	data := self.data[len(self.data)]

	self.data = self.data[:len(self.data)-1]

	return data
}
