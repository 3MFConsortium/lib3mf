#!/usr/bin/env python3

"""Example helper for assigning object materials and carrying slicer metadata.

This script handles the standard 3MF part of the workflow:
- read an input 3MF
- assign one base material per mesh object
- optionally collapse all mesh objects into one ComponentsObject build item
- optionally copy `/Metadata/...` attachments from a slicer-produced template 3MF
- optionally inject a raw `project_settings.config` attachment
- optionally transplant slicer-specific package entries from a template 3MF after writing

Important:
- Material assignment is standard 3MF and supported by lib3mf.
- Bambu/Orca "extruder" semantics are slicer-specific and not modeled by lib3mf.
  For those, this script either copies attachment bytes or transplants package entries
  from a known-good slicer-generated template.

This variant assumes the PyPI package is installed:
    import lib3mf
    from lib3mf import get_wrapper
"""

from __future__ import annotations

import argparse
import io
import zipfile

import lib3mf
from lib3mf import get_wrapper


DEFAULT_RELATIONSHIP = "http://schemas.3mf.io/package/2015/relationships/metadata/attachment"


def parse_hex_color(text: str) -> lib3mf.Color:
    value = text.strip()
    if value.startswith("#"):
        value = value[1:]
    if len(value) != 6:
        raise ValueError(f"Color must be RRGGBB, got: {text}")

    color = lib3mf.Color()
    color.Red = int(value[0:2], 16)
    color.Green = int(value[2:4], 16)
    color.Blue = int(value[4:6], 16)
    color.Alpha = 255
    return color


def identity_transform(wrapper):
    return wrapper.GetIdentityTransform()


def iter_objects(model):
    it = model.GetObjects()
    while it.MoveNext():
        yield it.GetCurrentObject()


def iter_mesh_objects(model):
    for obj in iter_objects(model):
        if obj.IsMeshObject():
            yield obj


def iter_build_items(model):
    it = model.GetBuildItems()
    while it.MoveNext():
        yield it.GetCurrent()


def assign_materials(model: Lib3MF.Model, mesh_objects, material_specs):
    if not material_specs:
        return None

    base_materials = model.AddBaseMaterialGroup()
    property_ids = []

    for idx, spec in enumerate(material_specs, start=1):
        if "=" in spec:
            name, color_text = spec.split("=", 1)
        elif ":" in spec:
            name, color_text = spec.split(":", 1)
        else:
            name = f"Material {idx}"
            color_text = spec
        property_id = base_materials.AddMaterial(name.strip(), parse_hex_color(color_text.strip()))
        property_ids.append(property_id)

    mesh_objects = list(mesh_objects)
    if len(mesh_objects) > len(property_ids):
        raise ValueError(
            f"Need at least {len(mesh_objects)} material specs, got {len(property_ids)}."
        )

    resource_id = base_materials.GetResourceID()
    for mesh_object, property_id in zip(mesh_objects, property_ids):
        mesh_object.SetObjectLevelProperty(resource_id, property_id)

    return base_materials


def collapse_meshes_to_components(model, wrapper, mesh_objects):
    mesh_objects = list(mesh_objects)
    if not mesh_objects:
        raise ValueError("No mesh objects found to collapse.")

    components = model.AddComponentsObject()
    components.SetName("Combined Part")
    transform = identity_transform(wrapper)

    for mesh_object in mesh_objects:
        components.AddComponent(mesh_object, transform)

    existing_build_items = list(iter_build_items(model))
    for build_item in existing_build_items:
        model.RemoveBuildItem(build_item)

    model.AddBuildItem(components, transform)
    return components


def copy_metadata_attachments_from_template(wrapper, target_model, template_3mf: str):
    template_model = wrapper.CreateModel()
    template_reader = template_model.QueryReader("3mf")
    template_reader.ReadFromFile(template_3mf)

    copied = []
    for index in range(template_model.GetAttachmentCount()):
        attachment = template_model.GetAttachment(index)
        path = attachment.GetPath()
        if not path.startswith("/Metadata/"):
            continue

        target_attachment = target_model.AddAttachment(path, attachment.GetRelationShipType())
        target_attachment.ReadFromBuffer(attachment.WriteToBuffer())
        copied.append(path)

    return copied


def add_project_settings_attachment(model, config_path: str, uri: str, relationship_type: str):
    attachment = model.AddAttachment(uri, relationship_type)
    attachment.ReadFromFile(config_path)
    return attachment


def normalize_zip_name(name: str) -> str:
    return name[1:] if name.startswith("/") else name


def should_copy_zip_entry(entry_name: str, prefixes):
    normalized = normalize_zip_name(entry_name)
    return any(normalized.startswith(normalize_zip_name(prefix)) for prefix in prefixes)


def transplant_package_entries(template_3mf: str, output_3mf: str, prefixes):
    if not prefixes:
        return []

    copied = []
    with zipfile.ZipFile(output_3mf, "r") as output_zip:
        existing_entries = {
            info.filename: output_zip.read(info.filename)
            for info in output_zip.infolist()
        }

    with zipfile.ZipFile(template_3mf, "r") as template_zip:
        for info in template_zip.infolist():
            if should_copy_zip_entry(info.filename, prefixes):
                existing_entries[info.filename] = template_zip.read(info.filename)
                copied.append(info.filename)

    buffer = io.BytesIO()
    with zipfile.ZipFile(buffer, "w", compression=zipfile.ZIP_DEFLATED) as merged_zip:
        for filename in sorted(existing_entries.keys()):
            merged_zip.writestr(filename, existing_entries[filename])

    with open(output_3mf, "wb") as handle:
        handle.write(buffer.getvalue())

    return copied


def main() -> int:
    parser = argparse.ArgumentParser(description="Assign object materials and carry slicer-specific package metadata.")
    parser.add_argument("input_3mf", help="Input 3MF file, e.g. from OpenSCAD")
    parser.add_argument("output_3mf", help="Output 3MF file")
    parser.add_argument(
        "--material",
        action="append",
        default=[],
        help="Material spec in the form Name=#RRGGBB or Name:RRGGBB. Repeat once per mesh object.",
    )
    parser.add_argument(
        "--collapse-to-components",
        action="store_true",
        help="Replace current build items with a single ComponentsObject referencing all mesh objects.",
    )
    parser.add_argument(
        "--copy-metadata-from",
        help="Copy every attachment under /Metadata/ from a slicer-produced template 3MF.",
    )
    parser.add_argument(
        "--project-settings-config",
        help="Attach a raw project_settings.config file into the package.",
    )
    parser.add_argument(
        "--project-settings-uri",
        default="/Metadata/project_settings.config",
        help="Package URI for the injected config attachment.",
    )
    parser.add_argument(
        "--project-settings-relationship",
        default=DEFAULT_RELATIONSHIP,
        help="Relationship type for the injected config attachment.",
    )
    parser.add_argument(
        "--transplant-from-template",
        help="After writing the output 3MF, copy selected raw package entries from this slicer-authored template.",
    )
    parser.add_argument(
        "--transplant-prefix",
        action="append",
        default=[],
        help="Package prefix to copy from the template zip, e.g. /Metadata/ or /3D/Objects/. Repeat as needed.",
    )
    args = parser.parse_args()

    wrapper = get_wrapper()
    model = wrapper.CreateModel()
    reader = model.QueryReader("3mf")
    reader.SetStrictModeActive(False)
    reader.ReadFromFile(args.input_3mf)

    mesh_objects = list(iter_mesh_objects(model))
    if not mesh_objects:
        raise RuntimeError("Input model does not contain any mesh objects.")

    assign_materials(model, mesh_objects, args.material)

    if args.collapse_to_components:
        collapse_meshes_to_components(model, wrapper, mesh_objects)

    copied_paths = []
    if args.copy_metadata_from:
        copied_paths = copy_metadata_attachments_from_template(wrapper, model, args.copy_metadata_from)

    if args.project_settings_config:
        add_project_settings_attachment(
            model,
            args.project_settings_config,
            args.project_settings_uri,
            args.project_settings_relationship,
        )

    model_metadata = model.GetMetaDataGroup()

    writer = model.QueryWriter("3mf")
    writer.WriteToFile(args.output_3mf)

    transplanted_entries = []
    if args.transplant_from_template and args.transplant_prefix:
        transplanted_entries = transplant_package_entries(
            args.transplant_from_template,
            args.output_3mf,
            args.transplant_prefix,
        )

    print(f"Read {len(mesh_objects)} mesh object(s) from {args.input_3mf}")
    if args.material:
        print(f"Assigned {min(len(args.material), len(mesh_objects))} material mapping(s)")
    if args.collapse_to_components:
        print("Collapsed mesh build items into one ComponentsObject build item")
    if copied_paths:
        print("Copied metadata attachments:")
        for path in copied_paths:
            print(f"  {path}")
    if args.project_settings_config:
        print(f"Attached raw project settings file as {args.project_settings_uri}")
    if transplanted_entries:
        print("Transplanted package entries from template:")
        for path in transplanted_entries:
            print(f"  {path}")
    print(f"Wrote {args.output_3mf}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
