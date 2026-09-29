import assert from "node:assert/strict";
import path from "node:path";
import { pathToFileURL } from "node:url";

const modulePath = path.resolve(import.meta.dirname, "../build/wasm/lib3mf.js");
const { default: createLib3mf } = await import(pathToFileURL(modulePath));
const lib = await createLib3mf();
const handles = [];
const own = (value) => { handles.push(value); return value; };

try {
  const wrapper = own(new lib.CWrapper());
  const model = own(wrapper.CreateModel());
  const attachment = own(model.AddAttachment("/3D/Textures/displacement.png",
    "http://schemas.microsoft.com/3dmanufacturing/2013/01/3dtexture"));
  lib.FS.writeFile("/displacement.png", new Uint8Array([137, 80, 78, 71, 13, 10, 26, 10]));
  attachment.ReadFromFile("/displacement.png");
  const texture = own(model.AddDisplacement2D(attachment));
  const normals = own(model.AddNormVectorGroup());
  const normal = own(new lib.sVector());
  normal.set_Coordinates0(1);
  normal.set_Coordinates1(1);
  normal.set_Coordinates2(1);
  normals.AddVector(normal);
  const group = own(model.AddDisp2DGroup(texture, normals, 1, 0));
  for (const factor of [0, 0.5, 1]) {
    const coordinate = own(new lib.sDisplacement2DCoordinate());
    coordinate.set_U(factor);
    coordinate.set_V(1 - factor);
    coordinate.set_NormalVectorIndex(0);
    coordinate.set_DisplacementFactor(factor);
    group.AddCoordinate(coordinate);
  }

  const mesh = own(model.AddDisplacementMeshObject());
  for (const xyz of [[0, 0, 0], [1, 0, 0], [0, 1, 0], [0, 0, 1]]) {
    const vertex = own(new lib.sPosition());
    vertex.set_Coordinates0(xyz[0]);
    vertex.set_Coordinates1(xyz[1]);
    vertex.set_Coordinates2(xyz[2]);
    mesh.AddVertex(vertex);
  }
  for (const indices of [[0, 2, 1], [0, 1, 3], [0, 3, 2], [1, 2, 3]]) {
    const triangle = own(new lib.sTriangle());
    triangle.set_Indices0(indices[0]);
    triangle.set_Indices1(indices[1]);
    triangle.set_Indices2(indices[2]);
    mesh.AddTriangle(triangle);
  }
  const displacement = own(new lib.sTriangleDisplacement());
  displacement.set_DisplacementIndices0(2);
  displacement.set_DisplacementIndices1(0);
  displacement.set_DisplacementIndices2(1);
  mesh.SetTriangleDisplacement(3, group, displacement);

  const result = mesh.GetTriangleDisplacement(3);
  const returned = own(result.return);
  const returnedGroup = own(result.Disp2DGroup);
  assert.ok(returned instanceof lib.sTriangleDisplacement);
  assert.deepEqual([returned.get_DisplacementIndices0(), returned.get_DisplacementIndices1(),
    returned.get_DisplacementIndices2()], [2, 0, 1]);
  assert.equal(returnedGroup.GetResourceID(), group.GetResourceID());
  assert.equal(returnedGroup.GetCount(), 3);
  console.log("WASM triangle displacement: struct return and group handle passed");
} finally {
  for (const handle of handles.reverse()) handle.delete();
  lib.FS.unlink("/displacement.png");
}
