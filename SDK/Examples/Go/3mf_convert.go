/*
+++

Copyright (C) 2019 3MF Consortium (Vijai Kumar Suriyababu)

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions, and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions, and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Abstract: An example to convert between 3MF and STL

Interface version: 2.4.1-alpha
+++
*/

package main

import (
	"fmt"
	"log"
	"os"
	"strings"

	lib3mf "github.com/3MFConsortium/lib3mf.go/v2"
)

// findExtension returns the file extension from a given filename.
func findExtension(filename string) string {
	idx := strings.LastIndex(filename, ".")
	if idx != -1 {
		return filename[idx:]
	}
	return ""
}

// convert performs the conversion between 3MF and STL formats.
func convert(filename string) int {
	// Get a wrapper object
	wrapper, err := lib3mf.GetWrapper()
	if err != nil {
		log.Fatal("Error loading 3MF library:", err)
		return -1
	}

	// Check the library version (optional, similar to get_version in Python)
	nMajor, nMinor, nMicro, err := wrapper.GetLibraryVersion()
	if err != nil {
		log.Fatal("Error fetching lib3mf version:", err)
		return -1
	}
	fmt.Printf("lib3mf version: %d.%d.%d\n", nMajor, nMinor, nMicro)

	extension := strings.ToLower(findExtension(filename))
	var readerName, writerName, newExtension string

	switch extension {
	case ".stl":
		readerName = "stl"
		writerName = "3mf"
		newExtension = ".3mf"
	case ".3mf":
		readerName = "3mf"
		writerName = "stl"
		newExtension = ".stl"
	default:
		fmt.Printf("Unknown input file extension: %s\n", extension)
		return -1
	}

	outputFilename := filename[:len(filename)-len(extension)] + newExtension

	// Create a new 3MF model
	model, err := wrapper.CreateModel()
	if err != nil {
		log.Fatal("Error creating 3MF model:", err)
		return -1
	}

	// Read from the input file
	reader, err := model.QueryReader(readerName)
	if err != nil {
		log.Fatal("Error querying reader for format:", readerName)
		return -1
	}
	fmt.Printf("Reading %s...\n", filename)
	err = reader.ReadFromFile(filename)
	if err != nil {
		log.Fatal("Error reading from file:", err)
		return -1
	}

	// Write to the output file
	writer, err := model.QueryWriter(writerName)
	if err != nil {
		log.Fatal("Error querying writer for format:", writerName)
		return -1
	}
	fmt.Printf("Writing %s...\n", outputFilename)
	err = writer.WriteToFile(outputFilename)
	if err != nil {
		log.Fatal("Error writing to file:", err)
		return -1
	}

	fmt.Println("Done")
	return 0
}

func main() {
	if len(os.Args) != 2 {
		fmt.Println("Usage:")
		fmt.Println("Convert 3MF to STL: go run 3mf_convert.go model.3mf")
		fmt.Println("Convert STL to 3MF: go run 3mf_convert.go model.stl")
		os.Exit(1)
	} else {
		if err := convert(os.Args[1]); err != 0 {
			os.Exit(err)
		}
	}
}
