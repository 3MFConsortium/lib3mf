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

Abstract: Common set of functions that are used across all examples

Interface version: 2.4.1

'''

import os
import sys

try:
    import lib3mf
    from lib3mf import get_wrapper
except ImportError:
    sys.path.append(os.path.join(os.path.dirname(os.path.realpath(__file__)), "..", "..", "Bindings", "Python"))
    import Lib3MF as lib3mf
    import Lib3MF


    def get_wrapper():
        libpath = os.path.join(os.path.dirname(os.path.realpath(__file__)), "..", "..", "Bin")
        wrapper = Lib3MF.Wrapper(os.path.join(libpath, "lib3mf"))
        return wrapper


def create_vertex(_mesh, x, y, z):
    position = lib3mf.Position()
    position.Coordinates[0] = float(x)
    position.Coordinates[1] = float(y)
    position.Coordinates[2] = float(z)
    _mesh.AddVertex(position)
    return position


def create_vertex_and_return_index(_mesh, x, y, z):
    position = lib3mf.Position()
    position.Coordinates[0] = float(x)
    position.Coordinates[1] = float(y)
    position.Coordinates[2] = float(z)
    vertex_index = _mesh.AddVertex(position)
    return vertex_index


def add_triangle(_mesh, p1, p2, p3):
    triangle = lib3mf.Triangle()
    triangle.Indices[0] = p1
    triangle.Indices[1] = p2
    triangle.Indices[2] = p3
    _mesh.AddTriangle(triangle)
    return triangle


def get_version(wrapper):
    major, minor, micro = wrapper.GetLibraryVersion()
    print("Lib3MF version: {:d}.{:d}.{:d}".format(major, minor, micro), end="")
    hasInfo, prereleaseinfo = wrapper.GetPrereleaseInformation()
    if hasInfo:
        print("-" + prereleaseinfo, end="")
    hasInfo, buildinfo = wrapper.GetBuildInformation()
    if hasInfo:
        print("+" + buildinfo, end="")
    print("")


# Show metadata information
def show_metadata_information(metadata_group):
    count = metadata_group.GetMetaDataCount()
    for i in range(count):
        metadata = metadata_group.GetMetaData(i)
        print(f"Metadata: {metadata.GetName()} = {metadata.GetValue()}")


# Show slice stack information
def show_slice_stack_information(model):
    slice_stacks = model.GetSliceStacks()
    while slice_stacks.MoveNext():
        slice_stack = slice_stacks.GetCurrentSliceStack()
        print(f"Slice Stack: {slice_stack.GetResourceID()}")


# Show object information
def show_object_information(model):
    object_iterator = model.GetObjects()
    while object_iterator.MoveNext():
        obj = object_iterator.GetCurrentObject()
        if obj.IsMeshObject():
            print(f"Mesh Object: {obj.GetResourceID()}")
        elif obj.IsComponentsObject():
            print(f"Components Object: {obj.GetResourceID()}")
        else:
            print(f"Unknown Object: {obj.GetResourceID()}")


def read_3mf_file_to_model(model, file_path):
    reader = model.QueryReader("3mf")
    reader.SetStrictModeActive(False)
    reader.ReadFromFile(file_path)


def create_translation_matrix(x, y, z):
    matrix = lib3mf.Transform()

    # Default to identity matrix with translation
    identity_matrix = [
        (1.0, 0.0, 0.0),  # Row for X axis
        (0.0, 1.0, 0.0),  # Row for Y axis
        (0.0, 0.0, 1.0),  # Row for Z axis
        (x, y, z)  # Translation
    ]

    # Fill the Fields with identity matrix values
    for i in range(4):
        for j in range(3):
            matrix.Fields[i][j] = identity_matrix[i][j]

    return matrix


def convert_beam_string_to_enum(beam_mode):
    beam_mode = str(beam_mode).lower()
    if beam_mode == "butt":
        return lib3mf.BeamLatticeCapMode.Butt
    if beam_mode == "sphere":
        return lib3mf.BeamLatticeCapMode.Sphere
    if beam_mode == "hemisphere":
        return lib3mf.BeamLatticeCapMode.HemiSphere


def create_beam(v0, v1, r0, r1, c0, c1):
    beam = lib3mf.Beam()
    beam.Indices[0] = v0
    beam.Indices[1] = v1
    beam.Radii[0] = r0
    beam.Radii[1] = r1
    beam.CapModes[0] = convert_beam_string_to_enum(c0)
    beam.CapModes[1] = convert_beam_string_to_enum(c1)
    return beam


def create_triangle_color(color_group, color_id1, color_id2, color_id3):
    triangle_properties = lib3mf.TriangleProperties()
    triangle_properties.ResourceID = color_group.GetResourceID()
    triangle_properties.PropertyIDs[0] = color_id1
    triangle_properties.PropertyIDs[1] = color_id2
    triangle_properties.PropertyIDs[2] = color_id3
    return triangle_properties


def convert_list_to_array(_list, _datatype):
    _array_type = _datatype * len(_list)  # Define the type of the array
    _array = _array_type()  # Create an instance of the array

    # Populate the array
    for i, _list_entry in enumerate(_list):
        _array[i] = _list_entry

    return _array


def vertex_array(_position_list):
    return convert_list_to_array(_position_list, lib3mf.Position)


def triangle_array(_triangle_list):
    return convert_list_to_array(_triangle_list, lib3mf.Triangle)


def beam_array(_beam_list):
    return convert_list_to_array(_beam_list, lib3mf.Beam)
