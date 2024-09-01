package main

import (
	"fmt"
	lib3mf "github.com/3MFConsortium/lib3mf_go/v2"
	"log"
	"os"
	"strings"
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
