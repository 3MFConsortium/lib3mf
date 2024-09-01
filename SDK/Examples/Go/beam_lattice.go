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

// convertBeamStringToEnum converts a string to a lib3mf.BeamLatticeCapMode enum.
func convertBeamStringToEnum(beamMode string) lib3mf.BeamLatticeCapMode {
	switch beamMode {
	case "Butt":
		return lib3mf.BeamLatticeCapMode_Butt
	case "Sphere":
		return lib3mf.BeamLatticeCapMode_Sphere
	case "HemiSphere":
		return lib3mf.BeamLatticeCapMode_HemiSphere
	default:
		log.Fatalf("Invalid beam cap mode: %s", beamMode)
		return lib3mf.BeamLatticeCapMode_Butt // Default value, won't be reached
	}
}

// createBeam creates a beam with given indices, radii, and cap modes.
func createBeam(v0, v1 uint32, r0, r1 float64, c0, c1 string) lib3mf.Beam {
	beam := lib3mf.Beam{
		Indices: [2]uint32{v0, v1},
		Radii:   [2]float64{r0, r1},
		CapModes: [2]lib3mf.BeamLatticeCapMode{
			convertBeamStringToEnum(c0),
			convertBeamStringToEnum(c1),
		},
	}
	return beam
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
	err = meshObject.SetName("Beamlattice")
	if err != nil {
		log.Fatalf("Error setting mesh object name: %v", err)
	}

	// Define the size of the box
	fSizeX, fSizeY, fSizeZ := float32(100.0), float32(200.0), float32(300.0)

	// Create vertices
	vertices := []lib3mf.Position{
		createVertex(meshObject, 0.0, 0.0, 0.0),
		createVertex(meshObject, fSizeX, 0.0, 0.0),
		createVertex(meshObject, fSizeX, fSizeY, 0.0),
		createVertex(meshObject, 0.0, fSizeY, 0.0),
		createVertex(meshObject, 0.0, 0.0, fSizeZ),
		createVertex(meshObject, fSizeX, 0.0, fSizeZ),
		createVertex(meshObject, fSizeX, fSizeY, fSizeZ),
		createVertex(meshObject, 0.0, fSizeY, fSizeZ),
	}

	// Define beam variables
	r0 := 1.0
	r1 := 1.5
	r2 := 2.0
	r3 := 2.5

	// Create beams
	beams := []lib3mf.Beam{
		createBeam(2, 1, r0, r0, "Butt", "Butt"),
		createBeam(0, 3, r0, r1, "Sphere", "Butt"),
		createBeam(4, 5, r0, r2, "Sphere", "Butt"),
		createBeam(6, 7, r0, r3, "HemiSphere", "Butt"),
		createBeam(0, 1, r1, r0, "HemiSphere", "Butt"),
		createBeam(5, 4, r1, r1, "Sphere", "HemiSphere"),
		createBeam(2, 3, r1, r2, "Sphere", "Sphere"),
		createBeam(7, 6, r1, r3, "Butt", "Butt"),
		createBeam(1, 2, r2, r2, "Butt", "Butt"),
		createBeam(6, 5, r2, r3, "HemiSphere", "Butt"),
		createBeam(3, 0, r3, r0, "Butt", "Sphere"),
		createBeam(4, 7, r3, r1, "HemiSphere", "HemiSphere"),
	}

	// Add vertices to mesh object using SetVertex
	for i, vertex := range vertices {
		err := meshObject.SetVertex(uint32(i), vertex)
		if err != nil {
			log.Fatalf("Error setting vertex at index %d: %v", i, err)
		}
	}

	// Set beam lattice
	beamLattice, err := meshObject.BeamLattice()
	if err != nil {
		log.Fatalf("Error getting beam lattice: %v", err)
	}

	// Add beams one by one
	for i, beam := range beams {
		_, err := beamLattice.AddBeam(beam)
		if err != nil {
			log.Fatalf("Error adding a beam lattice %d, %v", i, err)
		}
	}

	// Set beam min length
	err = beamLattice.SetMinLength(0.005)
	if err != nil {
		log.Fatalf("Error setting minimum length in beam lattice: %v", err)
	}

	// Get identity transform
	transform, _ := wrapper.GetIdentityTransform()

	// Add mesh object to the model as a build item
	_, err = model.AddBuildItem(meshObject.Object, transform)
	if err != nil {
		log.Fatalf("Error adding build item with identity transform: %v", err)
	}

	// Write the model to a 3MF file
	writer, err := model.QueryWriter("3mf")
	if err != nil {
		log.Fatalf("Error querying writer for 3MF format: %v", err)
	}
	err = writer.WriteToFile("beamlattice.3mf")
	if err != nil {
		log.Fatalf("Error writing to file 'beamlattice.3mf': %v", err)
	}

	fmt.Println("3MF file with beam lattice written successfully to 'beamlattice.3mf'")
}
