// Copyright (c) The Quickware Editor, 2026

package filetree

import (
	"os"
	"path/filepath"
	"qwe/internal"
	"slices"
	"sort"
)

const DEFAULT_FILETREE_WIDTH = 30

type FileNode struct {
	name     string
	path     string
	parent   *FileNode
	loaded   bool
	depth    int
	isDir    bool
	expanded bool
	children []*FileNode
}

type FileTree struct {
	root    *FileNode
	visible []*FileNode
	cursor  int
	scroll  int
	width   int
	show    bool
}

func NewFileTree(path string) *FileTree {
	abs, err := filepath.Abs(path)
	if err != nil {
		abs = path
	}

	info, err := os.Stat(path)
	if err != nil {
		internal.Log("[ERROR] filetree.NewFileTree: cannot stat path")
	}

	root := &FileNode{
		name:     filepath.Base(abs),
		path:     abs,
		isDir:    info.IsDir(),
		depth:    -1,
		expanded: info.IsDir(),
	}
	root.load()

	tree := &FileTree{
		root:  root,
		width: DEFAULT_FILETREE_WIDTH,
		show:  false,
	}
	tree.reload()

	return tree
}

func (self *FileNode) GetName() string {
	return self.name
}

func (self *FileNode) GetPath() string {
	return self.path
}

func (self *FileNode) GetDepth() int {
	return self.depth
}

func (self *FileNode) IsDir() bool {
	return self.isDir
}

func (self *FileNode) IsExpanded() bool {
	return self.expanded
}

func (self *FileTree) IsVisible() []*FileNode {
	return self.visible
}

func (self *FileTree) GetCursor() int {
	return self.cursor
}

func (self *FileTree) GetScroll() int {
	return self.scroll
}

func (self *FileTree) GetWidth() int {
	return self.width
}

func (self *FileTree) IsShown() bool {
	return self.show
}

func (self *FileTree) SetShown(show bool) {
	self.show = show
}

func (self *FileNode) load() {
	if self.loaded {
		return
	}
	self.loaded = true

	entries, err := os.ReadDir(self.path)

	if err != nil {
		internal.Log("[ERROR] filetree.load: cannot read dir")
		return
	}

	for _, entry := range entries {
		if slices.Contains(internal.CONFIG["fileTreeIgnore"].([]string), entry.Name()) {
			continue
		}

		node := &FileNode{
			name:   entry.Name(),
			path:   filepath.Join(self.path, entry.Name()),
			isDir:  entry.IsDir(),
			depth:  self.depth + 1,
			parent: self,
		}

		self.children = append(self.children, node)
	}

	sort.SliceStable(self.children, func(i, j int) bool {
		a, b := self.children[i], self.children[j]

		if a.isDir != b.isDir {
			return a.isDir
		}

		return a.name < b.name
	})
}

func (self *FileTree) walk(node *FileNode) {
	for _, child := range node.children {
		self.visible = append(self.visible, child)

		if child.isDir && child.expanded {
			self.walk(child)
		}
	}
}

func (self *FileTree) reload() {
	// GC: optimization to not realloc the array
	clear(self.visible)
	self.visible = self.visible[:0]

	self.walk(self.root)

	if self.cursor >= len(self.visible) {
		self.cursor = len(self.visible) - 1
	}
}

func (self *FileTree) MoveCursorUp() {
	if self.cursor > 0 {
		self.cursor -= 1
	}
}

func (self *FileTree) MoveCursorDown() {
	if self.cursor < len(self.visible)-1 {
		self.cursor += 1
	}
}

func (self *FileTree) Selected() *FileNode {
	if len(self.visible) == 0 {
		return nil
	}

	return self.visible[self.cursor]
}

func (self *FileTree) Toggle() {
	node := self.Selected()

	if node == nil || !node.isDir {
		return
	}

	node.load()
	node.expanded = !node.expanded
	self.reload()
}

func (self *FileTree) CollapseNode() {
	node := self.Selected()
	if node == nil {
		return
	}

	if node.isDir && node.expanded {
		node.expanded = false
		self.reload()
		return
	}

	if node.parent != nil && node.parent != self.root {
		for i, n := range self.visible {
			if n == node.parent {
				self.cursor = i
				break
			}
		}
	}
}

func (self *FileTree) ComputeScrollOffset(height int) {
	if height <= 0 {
		return
	}

	if self.cursor < self.scroll {
		self.scroll = self.cursor
	}

	if self.cursor >= self.scroll+height {
		self.scroll = self.cursor - height + 1
	}
}
