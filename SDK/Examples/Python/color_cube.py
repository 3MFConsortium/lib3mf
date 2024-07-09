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

Abstract: Color cube example

Interface version: 2.4.0

'''


from lib3mf_common import *

# Get wrapper
wrapper = get_wrapper()

# Get version
get_version(wrapper)

# Create a model
model = wrapper.CreateModel()

# Initialize a mesh object
mesh_object = model.AddMeshObject()
mesh_object.SetName("Colored Box")

# Define cube size
fSizeX, fSizeY, fSizeZ = 100.0, 200.0, 300.0

# Create vertices
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

# Define triangles
triangles = [
    add_triangle(mesh_object, 2, 1, 0),
    add_triangle(mesh_object, 0, 3, 2),
    add_triangle(mesh_object, 4, 5, 6),
    add_triangle(mesh_object, 6, 7, 4),
    add_triangle(mesh_object, 0, 1, 5),
    add_triangle(mesh_object, 5, 4, 0),
    add_triangle(mesh_object, 2, 3, 7),
    add_triangle(mesh_object, 7, 6, 2),
    add_triangle(mesh_object, 1, 2, 6),
    add_triangle(mesh_object, 6, 5, 1),
    add_triangle(mesh_object, 3, 0, 4),
    add_triangle(mesh_object, 4, 7, 3)
]

# Set geometry
mesh_object.SetGeometry(vertices, triangles)

# Define colors
color_group = model.AddColorGroup()
id_red = color_group.AddColor(wrapper.RGBAToColor(255, 0, 0, 255))
id_green = color_group.AddColor(wrapper.RGBAToColor(0, 255, 0, 255))
id_blue = color_group.AddColor(wrapper.RGBAToColor(0, 0, 255, 255))
id_orange = color_group.AddColor(wrapper.RGBAToColor(255, 128, 0, 255))
id_yellow = color_group.AddColor(wrapper.RGBAToColor(255, 255, 0, 255))

# Set triangle colors
sTriangleColorRed = create_triangle_color(color_group, id_red, id_red, id_red)
sTriangleColorGreen = create_triangle_color(color_group, id_green, id_green, id_green)
sTriangleColorBlue = create_triangle_color(color_group, id_blue, id_blue, id_blue)
sTriangleColor1 = create_triangle_color(color_group, id_orange, id_red, id_yellow)
sTriangleColor2 = create_triangle_color(color_group, id_yellow, id_green, id_orange)


# One-colored Triangles
mesh_object.SetTriangleProperties(0, sTriangleColorRed)
mesh_object.SetTriangleProperties(1, sTriangleColorRed)
mesh_object.SetTriangleProperties(2, sTriangleColorGreen)
mesh_object.SetTriangleProperties(3, sTriangleColorGreen)
mesh_object.SetTriangleProperties(4, sTriangleColorBlue)
mesh_object.SetTriangleProperties(5, sTriangleColorBlue)

# Gradient-colored Triangles
mesh_object.SetTriangleProperties(6, sTriangleColor1)
mesh_object.SetTriangleProperties(7, sTriangleColor2)
mesh_object.SetTriangleProperties(8, sTriangleColor1)
mesh_object.SetTriangleProperties(9, sTriangleColor2)
mesh_object.SetTriangleProperties(10, sTriangleColor1)
mesh_object.SetTriangleProperties(11, sTriangleColor2)

# Set object level property
mesh_object.SetObjectLevelProperty(sTriangleColorRed.ResourceID, sTriangleColorRed.PropertyIDs[0])

# Add build item and write to file
model.AddBuildItem(mesh_object, wrapper.GetIdentityTransform())
writer = model.QueryWriter("3mf")
writer.WriteToFile("colorcube.3mf")
