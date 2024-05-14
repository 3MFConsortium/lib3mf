# Beam Lattice example
from lib3mf_common import *

# Get a wrapper object
wrapper = get_wrapper()

# Check version always
get_version(wrapper)

# Create a model and set name
model = wrapper.CreateModel()
mesh_object = model.AddMeshObject()
mesh_object.SetName("Beamlattice")

# Modifiable size
fSizeX = 100.0
fSizeY = 200.0
fSizeZ = 300.0

# Define vertices (creates an array of lib3mf position objects)
vertices = [
    create_vertex(mesh_object, 0.0, 0.0, 0.0),
    create_vertex(mesh_object, fSizeX, 0.0, 0.0),
    create_vertex(mesh_object, fSizeX, fSizeY, 0.0),
    create_vertex(mesh_object, 0.0, fSizeY, 0.0),
    create_vertex(mesh_object, 0.0, 0.0, fSizeZ),
    create_vertex(mesh_object, fSizeX, 0.0, fSizeZ),
    create_vertex(mesh_object, fSizeX, fSizeY, fSizeZ),
    create_vertex(mesh_object, 0.0, fSizeY, fSizeZ)
]

# Define beam variables
r0 = 1.0
r1 = 1.5
r2 = 2.0
r3 = 2.5

# Create a list of beams (strings are automatically converted to enums)
beams = [
    create_beam(2, 1, r0, r0, 'Butt', 'Butt'),
    create_beam(0, 3, r0, r1, 'Sphere', 'Butt'),
    create_beam(4, 5, r0, r2, 'Sphere', 'Butt'),
    create_beam(6, 7, r0, r3, 'HemiSphere', 'Butt'),
    create_beam(0, 1, r1, r0, 'HemiSphere', 'Butt'),
    create_beam(5, 4, r1, r1, 'Sphere', 'HemiSphere'),
    create_beam(2, 3, r1, r2, 'Sphere', 'Sphere'),
    create_beam(7, 6, r1, r3, 'Butt', 'Butt'),
    create_beam(1, 2, r2, r2, 'Butt', 'Butt'),
    create_beam(6, 5, r2, r3, 'HemiSphere', 'Butt'),
    create_beam(3, 0, r3, r0, 'Butt', 'Sphere'),
    create_beam(4, 7, r3, r1, 'HemiSphere', 'HemiSphere')
]

# Set geometry and beams
mesh_object.SetGeometry(vertices, [])
beam_lattice = mesh_object.BeamLattice()
beam_lattice.SetBeams(beams)
beam_lattice.SetMinLength(0.005)

# Add mesh object to the model
model.AddBuildItem(mesh_object, wrapper.GetIdentityTransform())

# Write it out
writer = model.QueryWriter("3mf")
writer.WriteToFile("beamlattice.3mf")
