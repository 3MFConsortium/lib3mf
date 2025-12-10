# Plan: Extend lib3mf for New Implicit Nodes

## Overview
- Goal: add support for three implicit node types (beamlattice, functiongradient, normalizedistance) in lib3mf while preserving backward compatibility and generated bindings.
- Scope: API surface (AutomaticComponentToolkit), XML parsing/writing, schema updates, documentation, samples, and tests across all supported bindings.
- Deliverable: fully tested additive feature ready for release in lib3mf 2.5.0.

## Node Semantics and Contracts
### Beamlattice Node
- **Inputs**: `lattice` (ResourceID, required), `pos` (Vector, required).
- **Output**: `distance` (Scalar, unsigned distance to the referenced BeamLattice).
- **Validation**: resource must resolve to a MeshObject with BeamLattice; emit `BEAMLATTICE_INVALID_OBJECTTYPE` or `RESOURCENOTFOUND` otherwise. Ensure both inputs are wired (`INPUTNOTSET`).

### FunctionGradient Node
- **Inputs**: `function` (ResourceID, required), `pos` (Vector, required).
- **Attributes**: optional `channelname` (string) when the function outputs multiple channels.
- **Output**: `gradient` (Vector).
- **Validation**: referenced function must exist; require `channelname` for multi-channel outputs; record `INVALIDPARAM` when missing or unresolved.

### NormalizeDistance Node
- **Inputs**: `distance` (Scalar, required), `scale` (Scalar, optional).
- **Output**: `result` (Scalar) computed as `clamp(distance / max(scale, epsilon), -1, 1)`.
- **Validation**: ensure `distance` is wired; treat zero/negative `scale` as invalid in strict mode (`INVALIDPARAM`) and fall back to epsilon otherwise.

## AutomaticComponentToolkit Updates
- Increment `ImplicitNodeType` enum with: `BeamLattice = 51`, `FunctionGradient = 52`, `NormalizeDistance = 53`.
- Add node classes mirroring existing patterns:
  - `BeamLatticeNode` (inputs: lattice, pos; output: distance).
  - `FunctionGradientNode` (inputs: function ID, pos; property: channel name; output: gradient).
  - `NormalizeDistanceNode` (inputs: distance, scale; output: result).
- Extend `ImplicitFunction` helper methods (`AddBeamLatticeNode`, `AddFunctionGradientNode`, `AddNormalizeDistanceNode`).
- Bump library/component version to 2.5.0 (minor increase for additive API).

## XML Schema, Parsing, and Writing
- Update `implicit.xsd` to define `i:beamlattice`, `i:functiongradient`, and `i:normalizedistance` elements with proper attributes (e.g., `channelname`).
- Revise namespace version (e.g., `http://schemas.3mf.io/3dmanufacturing/implicit/2025/10`) while accepting older versions for reading.
- Reader changes:
  - Map new node names to the added enum values.
  - Resolve resource references (BeamLattice -> mesh; FunctionGradient -> function).
  - Persist channel names and optional scale links.
- Writer changes:
  - Emit new elements with correct namespace.
  - Serialize attributes/port wiring consistently with existing nodes.

## Runtime Evaluation Scaffolding
- Maintain evaluator-agnostic core; round-trip support must not depend on runtime evaluation.
- Optional evaluator guidance:
  - Beam lattice distance: integrate with existing beam geometry distance calculators when available.
  - Function gradient: finite differences for `FunctionFromImage3D`; leave analytic gradients for future work.
  - Normalize distance: simple scalar computation; clamp to avoid infinities.

## Backward Compatibility and Feature Gating
- Activate new node parsing when implicit namespace version >= new version; treat unsupported nodes as opaque or warn in strict mode if namespace is older.
- Preserve binary compatibility (additive enums/classes) and update semantic version (minor bump to 2.5.0).
- Continue to honor relaxed resource ordering as defined by volumetric extension requirements.

## Testing Strategy
- **Reader/Writer Unit Tests**: ensure nodes round-trip with and without optional attributes; cover error cases (invalid resources, missing channel name, bad scale).
- **Schema Validation**: validate new sample 3MF files against updated `implicit.xsd`.
- **Bindings Smoke Tests**: create nodes via C++, C#, Python, Go, Node, Pascal bindings; serialize and read back.
- **Negative Tests**: verify error handling for invalid references, incorrect resource types, and missing ports.

## Documentation and Samples
- Update volumetric/implicit documentation to describe new nodes (port tables, attribute descriptions, usage notes).
- Provide example 3MF files for each node type:
  - Beam lattice structural sample.
  - Function gradient on `FunctionFromImage3D`.
  - Normalize distance pipeline example.
- Add release notes/changelog entry for version 2.5.0.

## Build and CI
- Regenerate bindings with AutomaticComponentToolkit after XML edits.
- Update CMake or preset files only if new sources are emitted.
- Run existing build presets (`cmake: build` for Windows/Linux) and full CTest suites.
- Ensure new tests are integrated with coverage reporting.

## Risks and Mitigations
- **Distance Sign Ambiguity**: start with unsigned beamlattice distance; consider future signed variant if spec clarifies.
- **Function Gradient for Multi-Channel Functions**: enforce `channelname` requirement; document behavior.
- **Normalize Distance Stability**: clamp outputs; define behavior for absent/invalid scale.

## Milestones and Acceptance
1. **Phase 1 – API & XML Round-Trip**
   - Deliver schema updates, ACT changes, reader/writer support, unit tests.
2. **Phase 2 – Docs & Samples**
   - Publish documentation and sample 3MF files; validate schemas.
3. **Phase 3 – Optional Evaluator Enhancements**
   - Provide reference evaluator snippets (optional).

Success criteria: All phases complete, CI green, documentation published, and consumers informed of new minor version release.
