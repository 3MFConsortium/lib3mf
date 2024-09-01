package main

import (
	"fmt"
	lib3mf "github.com/3MFConsortium/lib3mf_go/v2"
	"log"
)

// createVertexAndReturnIndex creates a vertex on the mesh object and returns its index.
func createVertexAndReturnIndex(mesh lib3mf.MeshObject, x, y, z float32) uint32 {
	position := lib3mf.Position{
		Coordinates: [3]float32{x, y, z},
	}
	vertexIndex, err := mesh.AddVertex(position)
	if err != nil {
		log.Fatalf("Error adding vertex at (%f, %f, %f): %v", x, y, z, err)
	}
	return vertexIndex
}

// addTriangle adds a triangle to the mesh object using three vertex indices.
func addTriangle(mesh lib3mf.MeshObject, p1, p2, p3 uint32) {
	triangle := lib3mf.Triangle{
		Indices: [3]uint32{p1, p2, p3},
	}
	_, err := mesh.AddTriangle(triangle)
	if err != nil {
		log.Fatalf("Error adding triangle with vertices %d, %d, %d: %v", p1, p2, p3, err)
	}
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

	// Initialize a mesh object
	meshObject, err := model.AddMeshObject()
	if err != nil {
		log.Fatalf("Error adding mesh object: %v", err)
	}

	// Create 3 vertices
	p1 := createVertexAndReturnIndex(meshObject, 0, 0, 0)
	p2 := createVertexAndReturnIndex(meshObject, 0, 1, 0)
	p3 := createVertexAndReturnIndex(meshObject, 0, 0, 1)

	// Create a triangle with 3 positions
	addTriangle(meshObject, p1, p2, p3)

	// Get a 3MF writer and write the single triangle
	writer, err := model.QueryWriter("3mf")
	if err != nil {
		log.Fatalf("Error querying writer for 3MF format: %v", err)
	}
	err = writer.WriteToFile("triangle.3mf")
	if err != nil {
		log.Fatalf("Error writing to file 'triangle.3mf': %v", err)
	}

	fmt.Println("3MF file with a single triangle written successfully to 'triangle.3mf'")
}
