'''++

Copyright (C) 2019 3MF Consortium (Vijai Kumar Suriyababu)

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
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

'''


from lib3mf_common import *

# Get wrapper
wrapper = get_wrapper()

# Get version
get_version(wrapper)

# Create a model
model = wrapper.CreateModel()
mesh_object = model.AddMeshObject()
mesh_object.SetName("Box")

# Define the size of the box
fSizeX, fSizeY, fSizeZ = 10.0, 20.0, 30.0

# Create vertices
vertices = [
    create_vertex(mesh_object, 0, 0, 0),
    create_vertex(mesh_object, fSizeX, 0, 0),
    create_vertex(mesh_object, fSizeX, fSizeY, 0),
    create_vertex(mesh_object, 0, fSizeY, 0),
    create_vertex(mesh_object, 0, 0, fSizeZ),
    create_vertex(mesh_object, fSizeX, 0, fSizeZ),
    create_vertex(mesh_object, fSizeX, fSizeY, fSizeZ),
    create_vertex(mesh_object, 0, fSizeY, fSizeZ)
]

# Define triangles by vertices indices
triangle_indices = [
    (2, 1, 0), (0, 3, 2), (4, 5, 6), (6, 7, 4),
    (0, 1, 5), (5, 4, 0), (2, 3, 7), (7, 6, 2),
    (1, 2, 6), (6, 5, 1), (3, 0, 4), (4, 7, 3)
]

# Create a list of triangles
triangles = []
for v0, v1, v2 in triangle_indices:
    triangles.append(add_triangle(mesh_object, v0, v1, v2))

# Set geometry to the mesh object after creating vertices and triangles
mesh_object.SetGeometry(vertices, triangles)

# Adding components with different transformations
components_object = model.AddComponentsObject()
components_object.AddComponent(mesh_object, create_translation_matrix(0.0, 0.0, 0.0))
components_object.AddComponent(mesh_object, create_translation_matrix(40.0, 60.0, 80.0))
components_object.AddComponent(mesh_object, create_translation_matrix(120.0, 30.0, 70.0))

# Add the components object to the model as a build item
model.AddBuildItem(components_object, create_translation_matrix(0.0, 0.0, 0.0))

# Writing to files (3MF)
writer_3mf = model.QueryWriter("3mf")
writer_3mf.WriteToFile("components.3mf")

# Dump to a STL file
writer_stl = model.QueryWriter("stl")
writer_stl.WriteToFile("components.stl")