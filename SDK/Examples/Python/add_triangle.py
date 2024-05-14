from lib3mf_common import *

# Get wrapper
wrapper = get_wrapper()

# Get version
get_version(wrapper)

# Create a model
model = wrapper.CreateModel()

# Initialize a mesh object
meshObject = model.AddMeshObject()

# Now create 3 vertices
p1 = create_vertex_and_return_index(meshObject, 0, 0, 0)
p2 = create_vertex_and_return_index(meshObject, 0, 1, 0)
p3 = create_vertex_and_return_index(meshObject, 0, 0, 1)

# Create a triangle with 3 positions
add_triangle(meshObject, p1, p2, p3)

# Get a 3MF writer and write the single triangle
writer = model.QueryWriter("3mf")
writer.WriteToFile("triangle.3mf")
