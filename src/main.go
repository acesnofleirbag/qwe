package main

import (
	"fmt"
	"os"
	"qwe/src/internal"
)

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
			fmt.Println("The Quickware Editor 1.0.0")
			os.Exit(0)
		default:
			internal.EDITOR.OpenPath(args[i])
		}
	}
}

func main() {
	evalArgs(os.Args)

	iface := internal.NewScreen()

	internal.EDITOR.AttachIface(iface)
	internal.EDITOR.Run()
}
