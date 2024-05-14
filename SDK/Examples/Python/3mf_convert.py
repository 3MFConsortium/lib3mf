# An example to convert between 3MF and STL
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