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

Abstract: An example to convert between 3MF and STL

Interface version: 2.4.1

'''


import sys
from lib3mf_common import *


def find_extension(filename):
    idx = filename.rfind('.')
    if idx != -1:
        return filename[idx:]
    return ""


def convert(filename):
    # Get a wrapper object
    wrapper = get_wrapper()

    # Check version always
    get_version(wrapper)

    extension = find_extension(filename).lower()
    reader_name, writer_name, new_extension = "", "", ""

    if extension == ".stl":
        reader_name = "stl"
        writer_name = "3mf"
        new_extension = ".3mf"
    elif extension == ".3mf":
        reader_name = "3mf"
        writer_name = "stl"
        new_extension = ".stl"

    if not reader_name:
        print(f"Unknown input file extension: {extension}")
        return -1

    output_filename = filename[:-len(extension)] + new_extension

    model = wrapper.CreateModel()
    reader = model.QueryReader(reader_name)
    print(f"Reading {filename}...")
    reader.ReadFromFile(filename)

    writer = model.QueryWriter(writer_name)
    print(f"Writing {output_filename}...")
    writer.WriteToFile(output_filename)
    print("Done")
    return 0


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage:")
        print("Convert 3MF to STL: python3 3mf_convert.py model.3mf")
        print("Convert STL to 3MF: python3 3mf_convert.py model.stl")
    else:
        try:
            result = convert(sys.argv[1])
            sys.exit(result)
        except Exception as e:
            print(str(e))
            sys.exit(1)