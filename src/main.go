package main

import (
	"fmt"
	"os"
)

// global editor state
var EDITOR = NewEditor()

func usage() {
	fmt.Print(`The Quickware Editor: 

    -h, --help: This help message
    -v, --version: Print the quickware editor version
`)
}

func evalArgs(args []string) {
	for i := 1; i < len(args); i++ {
		switch args[i] {
		case "-h", "--help":
			usage()
			os.Exit(0)
		case "-v", "--version":
			fmt.Println("The Quickware Editor 0.1.0")
			os.Exit(0)
		}
	}
}

func main() {
	evalArgs(os.Args)

	iface := NewScreen()

	EDITOR.AttachIface(iface)
	EDITOR.SetConfig(Config{lineMode: LineMode__Normal, useArrows: true})
	EDITOR.Run()
}
