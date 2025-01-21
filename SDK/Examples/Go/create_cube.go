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

Abstract: Create a simple cube

Interface version: 2.4.0
+++
*/

package main

import (
	"fmt"
	"log"

	lib3mf "github.com/3MFConsortium/lib3mf.go/v2"
)

// createVertex creates a vertex on the mesh object and returns it.
func createVertex(mesh lib3mf.MeshObject, x, y, z float32) lib3mf.Position {
	position := lib3mf.Position{
		Coordinates: [3]float32{x, y, z},
	}
	_, err := mesh.AddVertex(position)
	if err != nil {
		log.Fatalf("Error adding vertex at (%f, %f, %f): %v", x, y, z, err)
	}
	return position
}

// addTriangle adds a triangle to the mesh object using three vertex indices.
func addTriangle(mesh lib3mf.MeshObject, p1, p2, p3 uint32) lib3mf.Triangle {
	triangle := lib3mf.Triangle{
		Indices: [3]uint32{p1, p2, p3},
	}
	_, err := mesh.AddTriangle(triangle)
	if err != nil {
		log.Fatalf("Error adding triangle with vertices %d, %d, %d: %v", p1, p2, p3, err)
	}
	return triangle
}

func main() {
	// Get a wrapper object
	wrapper, err := lib3mf.GetWrapper()
	if err != nil {
		log.Fatalf("Error loading 3MF library: %v", err)
	}

	// Check the lib3mf version
	nMajor, nMinor, nMicro, err := wrapper.GetLibraryVersion()
	if err != nil {
		log.Fatalf("Error fetching lib3mf version: %v", err)
	}
	fmt.Printf("lib3mf version: %d.%d.%d\n", nMajor, nMinor, nMicro)

	// Create a new 3MF model
	model, err := wrapper.CreateModel()
	if err != nil {
		log.Fatalf("Error creating 3MF model: %v", err)
	}

	// Initialize a mesh object and set its name
	meshObject, err := model.AddMeshObject()
	if err != nil {
		log.Fatalf("Error adding mesh object: %v", err)
	}
	err = meshObject.SetName("Box")
	if err != nil {
		log.Fatalf("Error setting mesh object name: %v", err)
	}

	// Define the size of the cube
	fSizeX, fSizeY, fSizeZ := float32(100.0), float32(200.0), float32(300.0)

	// Create vertices
	vertices := []lib3mf.Position{
		createVertex(meshObject, 0, 0, 0),
		createVertex(meshObject, fSizeX, 0, 0),
		createVertex(meshObject, fSizeX, fSizeY, 0),
		createVertex(meshObject, 0, fSizeY, 0),
		createVertex(meshObject, 0, 0, fSizeZ),
		createVertex(meshObject, fSizeX, 0, fSizeZ),
		createVertex(meshObject, fSizeX, fSizeY, fSizeZ),
		createVertex(meshObject, 0, fSizeY, fSizeZ),
	}

	// Define triangles by vertices indices
	triangleIndices := [][3]uint32{
		{2, 1, 0}, {0, 3, 2}, {4, 5, 6}, {6, 7, 4},
		{0, 1, 5}, {5, 4, 0}, {2, 3, 7}, {7, 6, 2},
		{1, 2, 6}, {6, 5, 1}, {3, 0, 4}, {4, 7, 3},
	}

	// Create triangles
	triangles := make([]lib3mf.Triangle, 0, len(triangleIndices))
	for _, indices := range triangleIndices {
		triangle := addTriangle(meshObject, indices[0], indices[1], indices[2])
		triangles = append(triangles, triangle)
	}

	// Set geometry to the mesh object after creating vertices and triangles
	err = meshObject.SetGeometry(vertices, triangles)
	if err != nil {
		log.Fatalf("Error setting geometry for mesh object: %v", err)
	}

	// Get identity transform
	transform, _ := wrapper.GetIdentityTransform()

	// Add build item with an identity transform
	_, err = model.AddBuildItem(meshObject.Object, transform)
	if err != nil {
		log.Fatalf("Error adding build item with identity transform: %v", err)
	}

	// Save the model to a 3MF file
	writer, err := model.QueryWriter("3mf")
	if err != nil {
		log.Fatalf("Error querying writer for 3MF format: %v", err)
	}
	err = writer.WriteToFile("cube.3mf")
	if err != nil {
		log.Fatalf("Error writing to file 'cube.3mf': %v", err)
	}

	fmt.Println("3MF file with a cube written successfully to 'cube.3mf'")
}
