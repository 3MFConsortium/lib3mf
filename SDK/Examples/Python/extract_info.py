# Extract info from a 3MF model
import sys
from lib3mf_common import *


def extract_info(file_path):
    wrapper = get_wrapper()
    model = wrapper.CreateModel()

    # Initialize a 3MF reader
    read_3mf_file_to_model(model, file_path)

    # Print library version
    get_version(wrapper)

    # Show meta data info
    show_metadata_information(model.GetMetaDataGroup())

    # Show slice stack info
    show_slice_stack_information(model)

    # Show object info
    show_object_information(model)


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python extract_info.py model.3mf")
        sys.exit()
    extract_info(sys.argv[1])
