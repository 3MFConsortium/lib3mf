package main

import (
	"fmt"
	lib3mf "github.com/3MFConsortium/lib3mf_go/v2"
	"log"
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

// createTranslationMatrix creates a translation matrix.
func createTranslationMatrix(x, y, z float32) lib3mf.Transform {
	matrix := lib3mf.Transform{}

	// Set up an identity matrix with translation
	identityMatrix := [4][3]float32{
		{1.0, 0.0, 0.0}, // Row for X axis
		{0.0, 1.0, 0.0}, // Row for Y axis
		{0.0, 0.0, 1.0}, // Row for Z axis
		{x, y, z},       // Translation
	}

	// Fill the matrix fields with values
	for i := 0; i < 4; i++ {
		for j := 0; j < 3; j++ {
			matrix.Fields[i][j] = identityMatrix[i][j]
		}
	}

	return matrix
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

	// Define the size of the box
	fSizeX, fSizeY, fSizeZ := float32(10.0), float32(20.0), float32(30.0)

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

	// Create a components object
	componentsObject, err := model.AddComponentsObject()
	if err != nil {
		log.Fatalf("Error adding components object: %v", err)
	}

	// Add components with different transformations
	_, err = componentsObject.AddComponent(meshObject.Object, createTranslationMatrix(0.0, 0.0, 0.0))
	if err != nil {
		log.Fatalf("Error adding component with transformation: %v", err)
	}
	_, err = componentsObject.AddComponent(meshObject.Object, createTranslationMatrix(40.0, 60.0, 80.0))
	if err != nil {
		log.Fatalf("Error adding component with transformation: %v", err)
	}
	_, err = componentsObject.AddComponent(meshObject.Object, createTranslationMatrix(120.0, 30.0, 70.0))
	if err != nil {
		log.Fatalf("Error adding component with transformation: %v", err)
	}

	// Add the components object to the model as a build item
	_, err = model.AddBuildItem(componentsObject.Object, createTranslationMatrix(0.0, 0.0, 0.0))
	if err != nil {
		log.Fatalf("Error adding build item with transformation: %v", err)
	}

	// Writing to files (3MF)
	writer3MF, err := model.QueryWriter("3mf")
	if err != nil {
		log.Fatalf("Error querying writer for 3MF format: %v", err)
	}
	err = writer3MF.WriteToFile("components.3mf")
	if err != nil {
		log.Fatalf("Error writing to file 'components.3mf': %v", err)
	}

	// Dump to a STL file
	writerSTL, err := model.QueryWriter("stl")
	if err != nil {
		log.Fatalf("Error querying writer for STL format: %v", err)
	}
	err = writerSTL.WriteToFile("components.stl")
	if err != nil {
		log.Fatalf("Error writing to file 'components.stl': %v", err)
	}

	fmt.Println("3MF and STL files with components written successfully to 'components.3mf' and 'components.stl'")
}
