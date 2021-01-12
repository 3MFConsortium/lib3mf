console.log("Loading Lib3MF");

var wrapper = require('../../Bindings/NodeJS/build/Release/lib3mf_nodeaddon')('../../Bin/lib3mf.dll');

console.log("creating Model");
model = wrapper.CreateModel();

console.log("creating Reader");
reader = model.QueryReader("3mf");

console.log("load 3MF file");
reader.ReadFromFile("../Files/Helix.3mf");

console.log("creating Writer");
writer = model.QueryWriter("stl");

console.log("write stl file");
writer.WriteToFile("Helix.stl");

console.log("done");
