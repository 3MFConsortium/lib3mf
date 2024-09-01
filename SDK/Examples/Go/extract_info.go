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

Abstract: An example that creates multiple components using transformations

Interface version: 2.3.2
+++
*/

package main

import (
	"fmt"
	lib3mf "github.com/3MFConsortium/lib3mf_go/v2"
	"log"
	"os"
)

// read3MFFileToModel reads a 3MF file into the provided model.
func read3MFFileToModel(model lib3mf.Model, filePath string) {
	reader, err := model.QueryReader("3mf")
	if err != nil {
		log.Fatalf("Error querying 3MF reader: %v", err)
	}
	err = reader.SetStrictModeActive(false)
	if err != nil {
		log.Fatalf("Error setting strict mode: %v", err)
	}
	err = reader.ReadFromFile(filePath)
	if err != nil {
		log.Fatalf("Error reading from file %s: %v", filePath, err)
	}
}

// getVersion prints the lib3mf library version information.
func getVersion(wrapper lib3mf.Wrapper) {
	nMajor, nMinor, nMicro, err := wrapper.GetLibraryVersion()
	if err != nil {
		log.Fatalf("Error fetching lib3mf version: %v", err)
	}
	versionString := fmt.Sprintf("lib3mf version: %d.%d.%d", nMajor, nMinor, nMicro)

	hasInfo, preReleaseInfo, err := wrapper.GetPrereleaseInformation()
	if err != nil {
		log.Fatalf("Error fetching prerelease information: %v", err)
	}
	if hasInfo {
		versionString += "-" + preReleaseInfo
	}

	hasInfo, buildInfo, err := wrapper.GetBuildInformation()
	if err != nil {
		log.Fatalf("Error fetching build information: %v", err)
	}
	if hasInfo {
		versionString += "+" + buildInfo
	}

	fmt.Println(versionString)
}

// showMetadataInformation prints metadata information from the model.
func showMetadataInformation(metadataGroup lib3mf.MetaDataGroup) {
	count, err := metadataGroup.GetMetaDataCount()
	if err != nil {
		log.Fatalf("Error getting metadata count: %v", err)
	}
	for i := uint32(0); i < count; i++ {
		metadata, err := metadataGroup.GetMetaData(i)
		if err != nil {
			log.Fatalf("Error getting metadata: %v", err)
		}
		name, err := metadata.GetName()
		if err != nil {
			log.Fatalf("Error getting metadata name: %v", err)
		}
		value, err := metadata.GetValue()
		if err != nil {
			log.Fatalf("Error getting metadata value: %v", err)
		}
		fmt.Printf("Metadata: %s = %s\n", name, value)
	}
}

// showSliceStackInformation prints information about slice stacks in the model.
func showSliceStackInformation(model lib3mf.Model) {
	sliceStacks, err := model.GetSliceStacks()
	if err != nil {
		log.Fatalf("Error getting slice stacks: %v", err)
	}
	for {
		hasNext, err := sliceStacks.MoveNext()
		if err != nil {
			log.Fatalf("Error moving to next slice stack: %v", err)
		}
		if !hasNext {
			break
		}

		sliceStack, err := sliceStacks.GetCurrentSliceStack()
		if err != nil {
			log.Fatalf("Error getting current slice stack: %v", err)
		}
		resourceID, err := sliceStack.GetResourceID()
		if err != nil {
			log.Fatalf("Error getting slice stack resource ID: %v", err)
		}
		fmt.Printf("Slice Stack: %d\n", resourceID)
	}
}

// showObjectInformation prints information about objects in the model.
func showObjectInformation(model lib3mf.Model) {
	objectIterator, err := model.GetObjects()
	if err != nil {
		log.Fatalf("Error getting objects iterator: %v", err)
	}

	for {
		hasNext, err := objectIterator.MoveNext()
		if err != nil {
			log.Fatalf("Error moving to next object: %v", err)
		}
		if !hasNext {
			break
		}

		obj, err := objectIterator.GetCurrentObject()
		if err != nil {
			log.Fatalf("Error getting current object: %v", err)
		}
		resourceID, err := obj.GetResourceID()
		if err != nil {
			log.Fatalf("Error getting object resource ID: %v", err)
		}
		isMesh, err := obj.IsMeshObject()
		if err != nil {
			log.Fatalf("Error checking if object is mesh: %v", err)
		}
		isComponent, err := obj.IsComponentsObject()
		if err != nil {
			log.Fatalf("Error checking if object is component: %v", err)
		}
		if isMesh {
			fmt.Printf("Mesh Object: %d\n", resourceID)
		} else if isComponent {
			fmt.Printf("Components Object: %d\n", resourceID)
		} else {
			fmt.Printf("Unknown Object: %d\n", resourceID)
		}
	}
}

func extractInfo(filePath string) {
	// Get a wrapper object
	wrapper, err := lib3mf.GetWrapper()
	if err != nil {
		log.Fatalf("Error loading 3MF library: %v", err)
	}

	// Create a new 3MF model
	model, err := wrapper.CreateModel()
	if err != nil {
		log.Fatalf("Error creating 3MF model: %v", err)
	}

	// Read the 3MF file into the model
	read3MFFileToModel(model, filePath)

	// Print library version
	getVersion(wrapper)

	// Show metadata information
	metaDataGroup, _ := model.GetMetaDataGroup()
	showMetadataInformation(metaDataGroup)

	// Show slice stack information
	showSliceStackInformation(model)

	// Show object information
	showObjectInformation(model)
}

func main() {
	if len(os.Args) != 2 {
		fmt.Println("Usage: go run extract_info.go model.3mf")
		os.Exit(1)
	}
	extractInfo(os.Args[1])
}
