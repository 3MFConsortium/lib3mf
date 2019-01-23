/*++

Copyright (C) 2018 3MF Consortium

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

Abstract:

NMR_ModelConstants.h defines the 3MF Model Constants.
These are given by the 3MF Standard

--*/

#ifndef __NMR_MODELCONSTANTS
#define __NMR_MODELCONSTANTS

#define PACKAGE_3D_RELS_CONTENT_TYPE "application/vnd.openxmlformats-package.relationships+xml"
#define PACKAGE_3D_MODEL_CONTENT_TYPE "application/vnd.ms-package.3dmanufacturing-3dmodel+xml"
#define PACKAGE_TEXTURE_CONTENT_TYPE "application/vnd.ms-package.3dmanufacturing-3dmodeltexture"
#define PACKAGE_PRINT_TICKET_CONTENT_TYPE "application/vnd.ms-printing.printticket+xml"
#define PACKAGE_CORE_PROPERTIES_CONTENT_TYPE "application/vnd.openxmlformats-package.core-properties+xml"
#define PACKAGE_GIF_CONTENT_TYPE "image/gif"
#define PACKAGE_JPG_CONTENT_TYPE "image/jpeg"
#define PACKAGE_PNG_CONTENT_TYPE "image/png"

#define PACKAGE_3D_MODEL_EXTENSION "model"
#define PACKAGE_3D_TEXTURE_EXTENSION "texture"
#define PACKAGE_3D_RELS_EXTENSION "rels"
#define PACKAGE_3D_JPG_EXTENSION "jpg"
#define PACKAGE_3D_JPEG_EXTENSION "jpeg"
#define PACKAGE_3D_PNG_EXTENSION "png"

#define PACKAGE_3D_MODEL_URI "/3D/3dmodel.model"
#define PACKAGE_TEXTURE_URI_BASE "/3D/Texture"
#define PACKAGE_PRINT_TICKET_URI "/3D/Metadata/Model_PT.xml"
#define PACKAGE_CORE_PROPERTIES_URI "/Metadata/CoreProperties.prop"
#define PACKAGE_THUMBNAIL_URI_BASE "/Metadata"

#define NMR_MAXHANDLE 0xfffffffe

#define PACKAGE_XMLNS_093 "http://schemas.microsoft.com/3dmanufacturing/2013/01"
#define PACKAGE_XMLNS_100 "http://schemas.microsoft.com/3dmanufacturing/core/2015/02"

#define PACKAGE_START_PART_RELATIONSHIP_TYPE "http://schemas.microsoft.com/3dmanufacturing/2013/01/3dmodel"
#define PACKAGE_PRINT_TICKET_RELATIONSHIP_TYPE "http://schemas.microsoft.com/3dmanufacturing/2013/01/printticket"
#define PACKAGE_TEXTURE_RELATIONSHIP_TYPE "http://schemas.microsoft.com/3dmanufacturing/2013/01/3dtexture"
#define PACKAGE_CORE_PROPERTIES_RELATIONSHIP_TYPE "http://schemas.openxmlformats.org/package/2006/relationships/metadata/core-properties"
#define PACKAGE_THUMBNAIL_RELATIONSHIP_TYPE "http://schemas.openxmlformats.org/package/2006/relationships/metadata/thumbnail"

#define XML_3MF_NAMESPACE_XML "http://www.w3.org/XML/1998/namespace"
#define XML_3MF_NAMESPACE_XMLNS "http://www.w3.org/2000/xmlns/"
#define XML_3MF_NAMESPACE_CORESPEC093 PACKAGE_XMLNS_093
#define XML_3MF_NAMESPACE_CORESPEC100 PACKAGE_XMLNS_100
#define XML_3MF_NAMESPACE_MATERIALSPEC "http://schemas.microsoft.com/3dmanufacturing/material/2015/02"
#define XML_3MF_NAMESPACE_PRODUCTIONSPEC "http://schemas.microsoft.com/3dmanufacturing/production/2015/06"
#define XML_3MF_NAMESPACE_BEAMLATTICESPEC "http://schemas.microsoft.com/3dmanufacturing/beamlattice/2017/02"
#define XML_3MF_NAMESPACE_SLICESPEC "http://schemas.microsoft.com/3dmanufacturing/slice/2015/07"

#define XML_3MF_NAMESPACEPREFIX_MATERIAL "m"
#define XML_3MF_NAMESPACEPREFIX_PRODUCTION "p"
#define XML_3MF_NAMESPACEPREFIX_BEAMLATTICE "b"
#define XML_3MF_NAMESPACEPREFIX_SLICE "s"

#define XML_3MF_ATTRIBUTE_XMLNS                     "xmlns"
#define XML_3MF_ATTRIBUTE_PREFIX_XML                "xml"
#define XML_3MF_ATTRIBUTE_MUST_UNDERSTAND           "MustUnderstand"

#define XML_3MF_ELEMENT_METADATA                    "metadata"
#define XML_3MF_ELEMENT_METADATA_ENRTY              "entry"
#define XML_3MF_ATTRIBUTE_METADATA_NAME         "name"
#define XML_3MF_ATTRIBUTE_METADATA_PRESERVE     "preserve"
#define XML_3MF_ATTRIBUTE_METADATA_TYPE         "type"
#define XML_3MF_METADATA_VALUE_1                "Title"
#define XML_3MF_METADATA_VALUE_2                "Designer"
#define XML_3MF_METADATA_VALUE_3                "Description"
#define XML_3MF_METADATA_VALUE_4                "Copyright"
#define XML_3MF_METADATA_VALUE_5                "LicenseTerms"
#define XML_3MF_METADATA_VALUE_6                "Rating"
#define XML_3MF_METADATA_VALUE_7                "CreationDate"
#define XML_3MF_METADATA_VALUE_8                "ModificationDate"
#define XML_3MF_METADATA_VALUE_9                "Application"


// Resources element.
#define XML_3MF_ELEMENT_RESOURCES                   "resources"

// Mesh element.
#define XML_3MF_ELEMENT_MESH                        "mesh"

// Slices element.
#define XML_3MF_ELEMENT_SLICES                      "slices"
#define XML_3MF_ATTRIBUTE_SLICES_SLICEHEIGHT        "sliceheight"

// Components element.
#define XML_3MF_ELEMENT_COMPONENTS                  "components"

// Vertices element.
#define XML_3MF_ELEMENT_VERTICES                    "vertices"

// Texturevertices element.
#define XML_3MF_ELEMENT_TEXTUREVERTICES             "texturevertices"

// Triangles element.
#define XML_3MF_ELEMENT_TRIANGLES                   "triangles"


#define XML_3MF_ELEMENT_BEAMLATTICE                       "beamlattice"
#define XML_3MF_ELEMENT_BEAMS                             "beams"
#define XML_3MF_ELEMENT_BEAM                              "beam"
#define XML_3MF_ELEMENT_BEAMSETS                          "beamsets"
#define XML_3MF_ELEMENT_BEAMSET                           "beamset"
#define XML_3MF_ELEMENT_REF                               "ref"

// Triangle element.
#define XML_3MF_ELEMENT_TRIANGLE                    "triangle"
#define XML_3MF_ATTRIBUTE_TRIANGLE_V1               "v1"
#define XML_3MF_ATTRIBUTE_TRIANGLE_V2               "v2"
#define XML_3MF_ATTRIBUTE_TRIANGLE_V3               "v3"
#define XML_3MF_ATTRIBUTE_TRIANGLE_COLORID          "colorid"
#define XML_3MF_ATTRIBUTE_TRIANGLE_CV1              "cv1"
#define XML_3MF_ATTRIBUTE_TRIANGLE_CV2              "cv2"
#define XML_3MF_ATTRIBUTE_TRIANGLE_CV3              "cv3"
#define XML_3MF_ATTRIBUTE_TRIANGLE_MATERIALID       "materialid"
#define XML_3MF_ATTRIBUTE_TRIANGLE_PID              "pid"
#define XML_3MF_ATTRIBUTE_TRIANGLE_P1               "p1"
#define XML_3MF_ATTRIBUTE_TRIANGLE_P2               "p2"
#define XML_3MF_ATTRIBUTE_TRIANGLE_P3               "p3"
#define XML_3MF_ATTRIBUTE_BEAMLATTICE_V1            "v1"
#define XML_3MF_ATTRIBUTE_BEAMLATTICE_V2            "v2"
#define XML_3MF_ATTRIBUTE_BEAMLATTICE_NAME          "name"
#define XML_3MF_ATTRIBUTE_BEAMLATTICE_IDENTIFIER    "identifier"
#define XML_3MF_ATTRIBUTE_BEAMLATTICE_R1            "r1"
#define XML_3MF_ATTRIBUTE_BEAMLATTICE_R2            "r2"
#define XML_3MF_ATTRIBUTE_BEAMLATTICE_CLIPPING      "clipping"
#define XML_3MF_ATTRIBUTE_BEAMLATTICE_CLIPPINGMESH  "clippingmesh"
#define XML_3MF_ATTRIBUTE_BEAMLATTICE_REPRESENTATIONMESH  "representationmesh"
#define XML_3MF_ATTRIBUTE_BEAMLATTICE_RADIUS        "radius"
#define XML_3MF_ATTRIBUTE_BEAMLATTICE_MINLENGTH     "minlength"
#define XML_3MF_ATTRIBUTE_BEAMLATTICE_CAPMODE       "cap"
#define XML_3MF_ATTRIBUTE_BEAMLATTICE_CAP1          "cap1"
#define XML_3MF_ATTRIBUTE_BEAMLATTICE_CAP2          "cap2"
#define XML_3MF_ATTRIBUTE_BEAMLATTICE_INDEX         "index"
#define XML_3MF_ATTRIBUTE_BEAMLATTICETAG_TAG        "tag"
// deprecated (has been used in draft version of the specification):
#define XML_3MF_ATTRIBUTE_BEAMLATTICE_ACCURACY      "accuracy"
#define XML_3MF_ATTRIBUTE_BEAMLATTICE_PRECISION     "precision"

// Vertex element.
#define XML_3MF_ELEMENT_VERTEX                      "vertex"
#define XML_3MF_ATTRIBUTE_VERTEX_X                  "x"
#define XML_3MF_ATTRIBUTE_VERTEX_Y                  "y"
#define XML_3MF_ATTRIBUTE_VERTEX_Z                  "z"

// Texturevertex element.
#define XML_3MF_ELEMENT_TEXTUREVERTEX               "texturevertex"
#define XML_3MF_ATTRIBUTE_TEXTUREVERTEX_U           "u"
#define XML_3MF_ATTRIBUTE_TEXTUREVERTEX_V           "v"
#define XML_3MF_ATTRIBUTE_TEXTUREVERTEX_W           "w"

// Texture2D element
#define XML_3MF_ELEMENT_TEXTURE2D                   "texture2d"
#define XML_3MF_ATTRIBUTE_TEXTURE2D_ID              "id"
#define XML_3MF_ATTRIBUTE_TEXTURE2D_PATH            "path"
#define XML_3MF_ATTRIBUTE_TEXTURE2D_CONTENTTYPE     "contenttype"
#define XML_3MF_ATTRIBUTE_TEXTURE2D_BOX             "box"
#define XML_3MF_ATTRIBUTE_TEXTURE2D_TILESTYLEU      "tilestyleu"
#define XML_3MF_ATTRIBUTE_TEXTURE2D_TILESTYLEV      "tilestylev"
#define XML_3MF_ATTRIBUTE_TEXTURE2D_TILESTYLE_WRAP  "wrap"
#define XML_3MF_ATTRIBUTE_TEXTURE2D_TILESTYLE_MIRROR "mirror"
#define XML_3MF_ATTRIBUTE_TEXTURE2D_TILESTYLE_CLAMP "clamp"
#define XML_3MF_ATTRIBUTE_TEXTURE2D_TILESTYLE_NONE  "none"
#define XML_3MF_ATTRIBUTE_TEXTURE2D_FILTER          "filter"
#define XML_3MF_ATTRIBUTE_TEXTURE2D_FILTER_AUTO     "auto"
#define XML_3MF_ATTRIBUTE_TEXTURE2D_FILTER_LINEAR   "linear"
#define XML_3MF_ATTRIBUTE_TEXTURE2D_FILTER_NEAREST  "nearest"

// Compositematerials and composite element (not implemented!)
#define XML_3MF_ELEMENT_COMPOSITEMATERIALS          "compositematerials"
#define XML_3MF_ATTRIBUTE_COMPOSITEMATERIALS_ID     "id"
#define XML_3MF_ATTRIBUTE_COMPOSITEMATERIALS_MATID  "matid"
#define XML_3MF_ATTRIBUTE_COMPOSITEMATERIALS_MATINDICES "matindices"
#define XML_3MF_ELEMENT_COMPOSITE                   "composite"
#define XML_3MF_ATTRIBUTE_COMPOSITE_VALUES          "values"

// Multiproperties (not implemented!)
#define XML_3MF_ELEMENT_MULTIPROPERTIES             "multiproperties"

// Geometry element.
#define XML_3MF_ELEMENT_GEOMETRY                    "geometry"
#define XML_3MF_ATTRIBUTE_GEOMETRY_DATA             "data"
#define XML_3MF_ATTRIBUTE_GEOMETRY_DEPTH            "depth"
#define XML_3MF_ATTRIBUTE_GEOMETRY_SLICE            "slice"
#define XML_3MF_ATTRIBUTE_GEOMETRY_OBJECTID         "objectid"
#define XML_3MF_ATTRIBUTE_GEOMETRY_OBJECT_TYPE      "objecttype"

// Geometry template element.
#define XML_3MF_ELEMENT_GEOMETRY_TEMPLATE           "geometrytemplate"
#define XML_3MF_ATTRIBUTE_GEOMETRY_TEMPLATE_ID      "id"
#define XML_3MF_ATTRIBUTE_GEOMETRY_TEMPLATE_DATA    "data"

// Component element.
#define XML_3MF_ELEMENT_COMPONENT                   "component"
#define XML_3MF_ATTRIBUTE_COMPONENT_OBJECTID        "objectid"
#define XML_3MF_ATTRIBUTE_COMPONENT_TRANSFORM       "transform"

// Color resource group
#define XML_3MF_ELEMENT_COLORGROUP					"colorgroup"
#define XML_3MF_ATTRIBUTE_COLORS_COLOR              "color"
#define XML_3MF_ATTRIBUTE_COLORS_ID                 "id"

// TexCoord resource group
#define XML_3MF_ELEMENT_TEX2DGROUP					"texture2dgroup"
#define XML_3MF_ATTRIBUTE_TEX2DGROUP_ID             "id"
#define XML_3MF_ELEMENT_TEX2COORD                   "tex2coord"
#define XML_3MF_ATTRIBUTE_TEX2DGROUP_TEXTUREID      "texid"

// Object element.
#define XML_3MF_ELEMENT_OBJECT                      "object"
#define XML_3MF_ATTRIBUTE_OBJECT_ID                 "id"
#define XML_3MF_ATTRIBUTE_OBJECT_COLOR_ID           "colorid"
#define XML_3MF_ATTRIBUTE_OBJECT_TYPE               "type"
#define XML_3MF_ATTRIBUTE_OBJECT_FILLMATERIALID     "fillmaterialid"
#define XML_3MF_ATTRIBUTE_OBJECT_MATERIALID         "materialid"
#define XML_3MF_ATTRIBUTE_OBJECT_FILLTEXTUREID      "filltextureid"
#define XML_3MF_ATTRIBUTE_OBJECT_MINWALLTHICKNESS   "minwallthickness"
#define XML_3MF_ATTRIBUTE_OBJECT_THUMBNAIL	        "thumbnail"
#define XML_3MF_ATTRIBUTE_OBJECT_FIT                "fit"
#define XML_3MF_ATTRIBUTE_OBJECT_NAME				"name"
#define XML_3MF_ATTRIBUTE_OBJECT_PARTNUMBER			"partnumber"
#define XML_3MF_ATTRIBUTE_OBJECT_PID                "pid"
#define XML_3MF_ATTRIBUTE_OBJECT_PINDEX             "pindex"
#define XML_3MF_ATTRIBUTE_OBJECT_SLICESTACKID       "slicestackid"
#define XML_3MF_ATTRIBUTE_OBJECT_MESHRESOLUTION     "meshresolution"
#define XML_3MF_VALUE_OBJECT_MESHRESOLUTION_LOW     "lowres"
#define XML_3MF_VALUE_OBJECT_MESHRESOLUTION_FULL    "fullres"

// Model element.
#define XML_3MF_ELEMENT_MODEL                       "model"
#define XML_3MF_ATTRIBUTE_MODEL_UNIT                "unit"
#define XML_3MF_ATTRIBUTE_MODEL_LANG                "lang"			// xml:lang
#define XML_3MF_ATTRIBUTE_MODEL_SPACE               "space"		// xml:space : forbidden
#define XML_3MF_ATTRIBUTE_REQUIREDEXTENSIONS        "requiredextensions"

// Build element.
#define XML_3MF_ELEMENT_BUILD                       "build" // was "make"
#define XML_3MF_ELEMENT_ITEM                        "item"
#define XML_3MF_ATTRIBUTE_ITEM_OBJECTID             "objectid"
#define XML_3MF_ATTRIBUTE_ITEM_TRANSFORM            "transform"
#define XML_3MF_ATTRIBUTE_ITEM_PARTNUMBER           "partnumber"

// Texture element.
#define XML_3MF_ELEMENT_TEXTURE                     "texture"
#define XML_3MF_ATTRIBUTE_TEXTURE_ID                "id"
#define XML_3MF_ATTRIBUTE_TEXTURE_PATH              "path"
#define XML_3MF_ATTRIBUTE_TEXTURE_WIDTH             "width"
#define XML_3MF_ATTRIBUTE_TEXTURE_HEIGHT            "height"
#define XML_3MF_ATTRIBUTE_TEXTURE_DEPTH             "depth"
#define XML_3MF_ATTRIBUTE_TEXTURE_CONTENTTYPE       "contenttype"
#define XML_3MF_ATTRIBUTE_TEXTURE_U                 "u"
#define XML_3MF_ATTRIBUTE_TEXTURE_V                 "v"
#define XML_3MF_ATTRIBUTE_TEXTURE_W                 "w"
#define XML_3MF_ATTRIBUTE_TEXTURE_SCALE             "scale"
#define XML_3MF_ATTRIBUTE_TEXTURE_ROTATION          "rotation"
#define XML_3MF_ATTRIBUTE_TEXTURE_TILESTYLEU        "tilestyleu"
#define XML_3MF_ATTRIBUTE_TEXTURE_TILESTYLEV        "tilestylev"
#define XML_3MF_ATTRIBUTE_TEXTURE_TILESTYLEW        "tilestylew"

// Color element.
#define XML_3MF_ELEMENT_COLOR                       "color"
#define XML_3MF_ATTRIBUTE_COLOR_ID                  "id"
#define XML_3MF_ATTRIBUTE_COLOR_VALUE               "value"


// Base Material element.
#define XML_3MF_ELEMENT_BASEMATERIALS                    "basematerials"
#define XML_3MF_ELEMENT_BASE                             "base"
#define XML_3MF_ATTRIBUTE_BASEMATERIALS_ID               "id"
#define XML_3MF_ATTRIBUTE_BASEMATERIAL_NAME              "name"
#define XML_3MF_ATTRIBUTE_BASEMATERIAL_DISPLAYCOLOR      "displaycolor"

// Material element.
#define XML_3MF_ELEMENT_MATERIAL                    "material"
#define XML_3MF_ATTRIBUTE_MATERIAL_ID               "id"
#define XML_3MF_ATTRIBUTE_MATERIAL_COLORID          "colorid"
#define XML_3MF_ATTRIBUTE_MATERIAL_NAME             "name"

#define XML_3MF_MODELUNIT_MICROMETER                "micron"
#define XML_3MF_MODELUNIT_MILLIMETER                "millimeter"
#define XML_3MF_MODELUNIT_CENTIMETER                "centimeter"
#define XML_3MF_MODELUNIT_INCH                      "inch"
#define XML_3MF_MODELUNIT_FOOT                      "foot"
#define XML_3MF_MODELUNIT_METER                     "meter"

#define XML_3MF_OBJECTTYPE_OTHER                    "other"
#define XML_3MF_OBJECTTYPE_MODEL                    "model"
#define XML_3MF_OBJECTTYPE_SUPPORT                  "support"
#define XML_3MF_OBJECTTYPE_SOLIDSUPPORT             "solidsupport"
#define XML_3MF_OBJECTTYPE_SURFACE                  "surface"

#define XML_3MF_BEAMLATTICE_CLIPMODE_NONE           "none"
#define XML_3MF_BEAMLATTICE_CLIPMODE_INSIDE         "inside"
#define XML_3MF_BEAMLATTICE_CLIPMODE_OUTSIDE        "outside"

#define XML_3MF_BEAMLATTICE_CAPMODE_SPHERE          "sphere"
#define XML_3MF_BEAMLATTICE_CAPMODE_HEMISPHERE      "hemisphere"
#define XML_3MF_BEAMLATTICE_CAPMODE_BUTT            "butt"
// depricated (from a draft version of the spec):
#define XML_3MF_BEAMLATTICE_CAPMODE_ROUND           "round"


#define XML_3MF_PRODUCTION_PATH                     "path"
#define XML_3MF_PRODUCTION_UUID                     "UUID"

#define XML_3MF_LANG_US                             "en-US"

#define XML_3MF_MAXIMUMCOORDINATEVALUE              1000000000.0f
#define XML_3MF_MAXMODELID                          2147483647
#define XML_3MF_MAXRESOURCEINDEX                    2147483647
#define XML_3MF_MAXRESOURCEID                       2147483647
#define XML_3MF_MAXMETADATACOUNT                    2147483647
#define XML_3MF_MAXBUILDITEMCOUNT                   2147483647
#define XML_3MF_MAXRESOURCECOUNT                    2147483647
#define XML_3MF_MAXIMUMBEAMRADIUSVALUE              1000000000.0f
#define XML_3MF_MAXIMUMSKINTHICKNESSVALUE           1000000000.0f
#define XML_3MF_MAXBEAMCOUNT                        2147483647

#endif // __NMR_MODELCONSTANTS
