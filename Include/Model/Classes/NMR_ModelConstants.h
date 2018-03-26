/*++

Copyright (C) 2015 Microsoft Corporation (Original Author)
Copyright (C) 2015 netfabb GmbH

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

#define PACKAGE_3D_RELS_CONTENT_TYPE L"application/vnd.openxmlformats-package.relationships+xml"
#define PACKAGE_3D_MODEL_CONTENT_TYPE L"application/vnd.ms-package.3dmanufacturing-3dmodel+xml"
#define PACKAGE_TEXTURE_CONTENT_TYPE L"application/vnd.ms-package.3dmanufacturing-3dmodeltexture"
#define PACKAGE_PRINT_TICKET_CONTENT_TYPE L"application/vnd.ms-printing.printticket+xml"
#define PACKAGE_CORE_PROPERTIES_CONTENT_TYPE L"application/vnd.openxmlformats-package.core-properties+xml"
#define PACKAGE_GIF_CONTENT_TYPE L"image/gif"
#define PACKAGE_JPG_CONTENT_TYPE L"image/jpeg"
#define PACKAGE_PNG_CONTENT_TYPE L"image/png"

#define PACKAGE_3D_MODEL_EXTENSION L"model"
#define PACKAGE_3D_TEXTURE_EXTENSION L"texture"
#define PACKAGE_3D_RELS_EXTENSION L"rels"
#define PACKAGE_3D_JPG_EXTENSION L"jpg"
#define PACKAGE_3D_JPEG_EXTENSION L"jpeg"
#define PACKAGE_3D_PNG_EXTENSION L"png"

#define PACKAGE_3D_MODEL_URI L"/3D/3dmodel.model"
#define PACKAGE_TEXTURE_URI_BASE L"/3D/Texture"
#define PACKAGE_PRINT_TICKET_URI L"/3D/Metadata/Model_PT.xml"
#define PACKAGE_CORE_PROPERTIES_URI L"/Metadata/CoreProperties.prop"
#define PACKAGE_THUMBNAIL_URI_BASE L"/Metadata"

#define NMR_MAXHANDLE 0xfffffffe

#define PACKAGE_XMLNS_093 L"http://schemas.microsoft.com/3dmanufacturing/2013/01"
#define PACKAGE_XMLNS_100 L"http://schemas.microsoft.com/3dmanufacturing/core/2015/02"

#define PACKAGE_START_PART_RELATIONSHIP_TYPE L"http://schemas.microsoft.com/3dmanufacturing/2013/01/3dmodel"
#define PACKAGE_PRINT_TICKET_RELATIONSHIP_TYPE L"http://schemas.microsoft.com/3dmanufacturing/2013/01/printticket"
#define PACKAGE_TEXTURE_RELATIONSHIP_TYPE L"http://schemas.microsoft.com/3dmanufacturing/2013/01/3dtexture"
#define PACKAGE_CORE_PROPERTIES_RELATIONSHIP_TYPE L"http://schemas.openxmlformats.org/package/2006/relationships/metadata/core-properties"
#define PACKAGE_THUMBNAIL_RELATIONSHIP_TYPE L"http://schemas.openxmlformats.org/package/2006/relationships/metadata/thumbnail"

#define XML_3MF_NAMESPACE_XML L"http://www.w3.org/XML/1998/namespace"
#define XML_3MF_NAMESPACE_XMLNS L"http://www.w3.org/2000/xmlns/"
#define XML_3MF_NAMESPACE_CORESPEC093 PACKAGE_XMLNS_093
#define XML_3MF_NAMESPACE_CORESPEC100 PACKAGE_XMLNS_100
#define XML_3MF_NAMESPACE_MATERIALSPEC L"http://schemas.microsoft.com/3dmanufacturing/material/2015/02"
#define XML_3MF_NAMESPACE_PRODUCTIONSPEC L"http://schemas.microsoft.com/3dmanufacturing/production/2015/06"
#define XML_3MF_NAMESPACE_BEAMLATTICESPEC L"http://schemas.microsoft.com/3dmanufacturing/beamlattice/2017/02"
#define XML_3MF_NAMESPACE_SLICESPEC L"http://schemas.microsoft.com/3dmanufacturing/slice/2015/07"

#define XML_3MF_NAMESPACEPREFIX_MATERIAL L"m"
#define XML_3MF_NAMESPACEPREFIX_PRODUCTION L"p"
#define XML_3MF_NAMESPACEPREFIX_BEAMLATTICE L"b"
#define XML_3MF_NAMESPACEPREFIX_SLICE L"s"

#define XML_3MF_ATTRIBUTE_XMLNS                     L"xmlns"
#define XML_3MF_ATTRIBUTE_PREFIX_XML                L"xml"
#define XML_3MF_ATTRIBUTE_MUST_UNDERSTAND           L"MustUnderstand"

#define XML_3MF_ELEMENT_METADATA                    L"metadata"
#define XML_3MF_ELEMENT_METADATA_ENRTY              L"entry"
#define XML_3MF_ATTRIBUTE_METADATA_NAME				L"name"
#define XML_3MF_METADATA_VALUE_1                L"Title"
#define XML_3MF_METADATA_VALUE_2                L"Designer"
#define XML_3MF_METADATA_VALUE_3                L"Description"
#define XML_3MF_METADATA_VALUE_4                L"Copyright"
#define XML_3MF_METADATA_VALUE_5                L"LicenseTerms"
#define XML_3MF_METADATA_VALUE_6                L"Rating"
#define XML_3MF_METADATA_VALUE_7                L"CreationDate"
#define XML_3MF_METADATA_VALUE_8                L"ModificationDate"


// Resources element.
#define XML_3MF_ELEMENT_RESOURCES                   L"resources"

// Mesh element.
#define XML_3MF_ELEMENT_MESH                        L"mesh"

// Slices element.
#define XML_3MF_ELEMENT_SLICES                      L"slices"
#define XML_3MF_ATTRIBUTE_SLICES_SLICEHEIGHT        L"sliceheight"

// Components element.
#define XML_3MF_ELEMENT_COMPONENTS                  L"components"

// Vertices element.
#define XML_3MF_ELEMENT_VERTICES                    L"vertices"

// Texturevertices element.
#define XML_3MF_ELEMENT_TEXTUREVERTICES             L"texturevertices"

// Triangles element.
#define XML_3MF_ELEMENT_TRIANGLES                   L"triangles"


#define XML_3MF_ELEMENT_BEAMLATTICE                       L"beamlattice"
#define XML_3MF_ELEMENT_BEAMS                             L"beams"
#define XML_3MF_ELEMENT_BEAM                              L"beam"
#define XML_3MF_ELEMENT_BEAMSETS                          L"beamsets"
#define XML_3MF_ELEMENT_BEAMSET                           L"beamset"
#define XML_3MF_ELEMENT_REF                               L"ref"

// Triangle element.
#define XML_3MF_ELEMENT_TRIANGLE                    L"triangle"
#define XML_3MF_ATTRIBUTE_TRIANGLE_V1               L"v1"
#define XML_3MF_ATTRIBUTE_TRIANGLE_V2               L"v2"
#define XML_3MF_ATTRIBUTE_TRIANGLE_V3               L"v3"
#define XML_3MF_ATTRIBUTE_TRIANGLE_COLORID          L"colorid"
#define XML_3MF_ATTRIBUTE_TRIANGLE_CV1              L"cv1"
#define XML_3MF_ATTRIBUTE_TRIANGLE_CV2              L"cv2"
#define XML_3MF_ATTRIBUTE_TRIANGLE_CV3              L"cv3"
#define XML_3MF_ATTRIBUTE_TRIANGLE_MATERIALID       L"materialid"
#define XML_3MF_ATTRIBUTE_TRIANGLE_PID              L"pid"
#define XML_3MF_ATTRIBUTE_TRIANGLE_P1               L"p1"
#define XML_3MF_ATTRIBUTE_TRIANGLE_P2               L"p2"
#define XML_3MF_ATTRIBUTE_TRIANGLE_P3               L"p3"
#define XML_3MF_ATTRIBUTE_BEAMLATTICE_V1            L"v1"
#define XML_3MF_ATTRIBUTE_BEAMLATTICE_V2            L"v2"
#define XML_3MF_ATTRIBUTE_BEAMLATTICE_NAME          L"name"
#define XML_3MF_ATTRIBUTE_BEAMLATTICE_IDENTIFIER    L"identifier"
#define XML_3MF_ATTRIBUTE_BEAMLATTICE_R1            L"r1"
#define XML_3MF_ATTRIBUTE_BEAMLATTICE_R2            L"r2"
#define XML_3MF_ATTRIBUTE_BEAMLATTICE_CLIPPING      L"clipping"
#define XML_3MF_ATTRIBUTE_BEAMLATTICE_CLIPPINGMESH  L"clippingmesh"
#define XML_3MF_ATTRIBUTE_BEAMLATTICE_REPRESENTATIONMESH  L"representationmesh"
#define XML_3MF_ATTRIBUTE_BEAMLATTICE_RADIUS        L"radius"
#define XML_3MF_ATTRIBUTE_BEAMLATTICE_MINLENGTH     L"minlength"
#define XML_3MF_ATTRIBUTE_BEAMLATTICE_CAPMODE       L"cap"
#define XML_3MF_ATTRIBUTE_BEAMLATTICE_CAP1          L"cap1"
#define XML_3MF_ATTRIBUTE_BEAMLATTICE_CAP2          L"cap2"
#define XML_3MF_ATTRIBUTE_BEAMLATTICE_INDEX         L"index"
#define XML_3MF_ATTRIBUTE_BEAMLATTICETAG_TAG        L"tag"
// deprecated (has been used in draft version of the specification):
#define XML_3MF_ATTRIBUTE_BEAMLATTICE_ACCURACY      L"accuracy"
#define XML_3MF_ATTRIBUTE_BEAMLATTICE_PRECISION     L"precision"

// Vertex element.
#define XML_3MF_ELEMENT_VERTEX                      L"vertex"
#define XML_3MF_ATTRIBUTE_VERTEX_X                  L"x"
#define XML_3MF_ATTRIBUTE_VERTEX_Y                  L"y"
#define XML_3MF_ATTRIBUTE_VERTEX_Z                  L"z"

// Texturevertex element.
#define XML_3MF_ELEMENT_TEXTUREVERTEX               L"texturevertex"
#define XML_3MF_ATTRIBUTE_TEXTUREVERTEX_U           L"u"
#define XML_3MF_ATTRIBUTE_TEXTUREVERTEX_V           L"v"
#define XML_3MF_ATTRIBUTE_TEXTUREVERTEX_W           L"w"

// Texture2D element
#define XML_3MF_ELEMENT_TEXTURE2D                   L"texture2d"
#define XML_3MF_ATTRIBUTE_TEXTURE2D_ID              L"id"
#define XML_3MF_ATTRIBUTE_TEXTURE2D_PATH            L"path"
#define XML_3MF_ATTRIBUTE_TEXTURE2D_CONTENTTYPE     L"contenttype"
#define XML_3MF_ATTRIBUTE_TEXTURE2D_BOX             L"box"
#define XML_3MF_ATTRIBUTE_TEXTURE2D_TILESTYLEU      L"tilestyleu"
#define XML_3MF_ATTRIBUTE_TEXTURE2D_TILESTYLEV      L"tilestylev"

// Compositematerials and composite element (not implemented!)
#define XML_3MF_ELEMENT_COMPOSITEMATERIALS          L"compositematerials"
#define XML_3MF_ATTRIBUTE_COMPOSITEMATERIALS_ID     L"id"
#define XML_3MF_ATTRIBUTE_COMPOSITEMATERIALS_MATID  L"matid"
#define XML_3MF_ATTRIBUTE_COMPOSITEMATERIALS_MATINDICES L"matindices"
#define XML_3MF_ELEMENT_COMPOSITE                   L"composite"
#define XML_3MF_ATTRIBUTE_COMPOSITE_VALUES          L"values"

// Multiproperties (not implemented!)
#define XML_3MF_ELEMENT_MULTIPROPERTIES             L"multiproperties"

// Geometry element.
#define XML_3MF_ELEMENT_GEOMETRY                    L"geometry"
#define XML_3MF_ATTRIBUTE_GEOMETRY_DATA             L"data"
#define XML_3MF_ATTRIBUTE_GEOMETRY_DEPTH            L"depth"
#define XML_3MF_ATTRIBUTE_GEOMETRY_SLICE            L"slice"
#define XML_3MF_ATTRIBUTE_GEOMETRY_OBJECTID         L"objectid"
#define XML_3MF_ATTRIBUTE_GEOMETRY_OBJECT_TYPE      L"objecttype"

// Geometry template element.
#define XML_3MF_ELEMENT_GEOMETRY_TEMPLATE           L"geometrytemplate"
#define XML_3MF_ATTRIBUTE_GEOMETRY_TEMPLATE_ID      L"id"
#define XML_3MF_ATTRIBUTE_GEOMETRY_TEMPLATE_DATA    L"data"

// Component element.
#define XML_3MF_ELEMENT_COMPONENT                   L"component"
#define XML_3MF_ATTRIBUTE_COMPONENT_OBJECTID        L"objectid"
#define XML_3MF_ATTRIBUTE_COMPONENT_TRANSFORM       L"transform"

// Color resource group
#define XML_3MF_ELEMENT_COLORGROUP					L"colorgroup"
#define XML_3MF_ATTRIBUTE_COLORS_COLOR              L"color"
#define XML_3MF_ATTRIBUTE_COLORS_ID                 L"id"

// TexCoord resource group
#define XML_3MF_ELEMENT_TEX2DGROUP					L"texture2dgroup"
#define XML_3MF_ATTRIBUTE_TEX2DGROUP_ID             L"id"
#define XML_3MF_ELEMENT_TEX2COORD                   L"tex2coord"
#define XML_3MF_ATTRIBUTE_TEX2DGROUP_TEXTUREID      L"texid"

// Object element.
#define XML_3MF_ELEMENT_OBJECT                      L"object"
#define XML_3MF_ATTRIBUTE_OBJECT_ID                 L"id"
#define XML_3MF_ATTRIBUTE_OBJECT_COLOR_ID           L"colorid"
#define XML_3MF_ATTRIBUTE_OBJECT_TYPE               L"type"
#define XML_3MF_ATTRIBUTE_OBJECT_FILLMATERIALID     L"fillmaterialid"
#define XML_3MF_ATTRIBUTE_OBJECT_MATERIALID         L"materialid"
#define XML_3MF_ATTRIBUTE_OBJECT_FILLTEXTUREID      L"filltextureid"
#define XML_3MF_ATTRIBUTE_OBJECT_MINWALLTHICKNESS   L"minwallthickness"
#define XML_3MF_ATTRIBUTE_OBJECT_THUMBNAIL	        L"thumbnail"
#define XML_3MF_ATTRIBUTE_OBJECT_FIT                L"fit"
#define XML_3MF_ATTRIBUTE_OBJECT_NAME				L"name"
#define XML_3MF_ATTRIBUTE_OBJECT_PARTNUMBER			L"partnumber"
#define XML_3MF_ATTRIBUTE_OBJECT_PID                L"pid"
#define XML_3MF_ATTRIBUTE_OBJECT_PINDEX             L"pindex"
#define XML_3MF_ATTRIBUTE_OBJECT_SLICESTACKID       L"slicestackid"
#define XML_3MF_ATTRIBUTE_OBJECT_MESHRESOLUTION     L"meshresolution"
#define XML_3MF_VALUE_OBJECT_MESHRESOLUTION_LOW     L"lowres"
#define XML_3MF_VALUE_OBJECT_MESHRESOLUTION_FULL    L"fullres"

// Model element.
#define XML_3MF_ELEMENT_MODEL                       L"model"
#define XML_3MF_ATTRIBUTE_MODEL_UNIT                L"unit"
#define XML_3MF_ATTRIBUTE_MODEL_LANG                L"lang"			// xml:lang
#define XML_3MF_ATTRIBUTE_MODEL_SPACE               L"space"		// xml:space : forbidden
#define XML_3MF_ATTRIBUTE_REQUIREDEXTENSIONS        L"requiredextensions"

// Build element.
#define XML_3MF_ELEMENT_BUILD                       L"build" // was "make"
#define XML_3MF_ELEMENT_ITEM                        L"item"
#define XML_3MF_ATTRIBUTE_ITEM_OBJECTID             L"objectid"
#define XML_3MF_ATTRIBUTE_ITEM_TRANSFORM            L"transform"
#define XML_3MF_ATTRIBUTE_ITEM_PARTNUMBER           L"partnumber"

// Texture element.
#define XML_3MF_ELEMENT_TEXTURE                     L"texture"
#define XML_3MF_ATTRIBUTE_TEXTURE_ID                L"id"
#define XML_3MF_ATTRIBUTE_TEXTURE_PATH              L"path"
#define XML_3MF_ATTRIBUTE_TEXTURE_WIDTH             L"width"
#define XML_3MF_ATTRIBUTE_TEXTURE_HEIGHT            L"height"
#define XML_3MF_ATTRIBUTE_TEXTURE_DEPTH             L"depth"
#define XML_3MF_ATTRIBUTE_TEXTURE_CONTENTTYPE       L"contenttype"
#define XML_3MF_ATTRIBUTE_TEXTURE_U                 L"u"
#define XML_3MF_ATTRIBUTE_TEXTURE_V                 L"v"
#define XML_3MF_ATTRIBUTE_TEXTURE_W                 L"w"
#define XML_3MF_ATTRIBUTE_TEXTURE_SCALE             L"scale"
#define XML_3MF_ATTRIBUTE_TEXTURE_ROTATION          L"rotation"
#define XML_3MF_ATTRIBUTE_TEXTURE_TILESTYLEU        L"tilestyleu"
#define XML_3MF_ATTRIBUTE_TEXTURE_TILESTYLEV        L"tilestylev"
#define XML_3MF_ATTRIBUTE_TEXTURE_TILESTYLEW        L"tilestylew"

// Color element.
#define XML_3MF_ELEMENT_COLOR                       L"color"
#define XML_3MF_ATTRIBUTE_COLOR_ID                  L"id"
#define XML_3MF_ATTRIBUTE_COLOR_VALUE               L"value"


// Base Material element.
#define XML_3MF_ELEMENT_BASEMATERIALS                    L"basematerials"
#define XML_3MF_ELEMENT_BASE                             L"base"
#define XML_3MF_ATTRIBUTE_BASEMATERIALS_ID	 			 L"id"
#define XML_3MF_ATTRIBUTE_BASEMATERIAL_NAME              L"name"
#define XML_3MF_ATTRIBUTE_BASEMATERIAL_DISPLAYCOLOR      L"displaycolor"

// Material element.
#define XML_3MF_ELEMENT_MATERIAL                    L"material"
#define XML_3MF_ATTRIBUTE_MATERIAL_ID               L"id"
#define XML_3MF_ATTRIBUTE_MATERIAL_COLORID			L"colorid"
#define XML_3MF_ATTRIBUTE_MATERIAL_NAME				L"name"

#define XML_3MF_MODELUNIT_MICROMETER                L"micron"
#define XML_3MF_MODELUNIT_MILLIMETER                L"millimeter"
#define XML_3MF_MODELUNIT_CENTIMETER                L"centimeter"
#define XML_3MF_MODELUNIT_INCH                      L"inch"
#define XML_3MF_MODELUNIT_FOOT                      L"foot"
#define XML_3MF_MODELUNIT_METER                     L"meter"

#define XML_3MF_OBJECTTYPE_OTHER                    L"other"
#define XML_3MF_OBJECTTYPE_MODEL                    L"model"
#define XML_3MF_OBJECTTYPE_SUPPORT                  L"support"
#define XML_3MF_OBJECTTYPE_SOLIDSUPPORT             L"solidsupport"

#define XML_3MF_BEAMLATTICE_CLIPMODE_NONE           L"none"
#define XML_3MF_BEAMLATTICE_CLIPMODE_INSIDE         L"inside"
#define XML_3MF_BEAMLATTICE_CLIPMODE_OUTSIDE        L"outside"

#define XML_3MF_BEAMLATTICE_CAPMODE_SPHERE          L"sphere"
#define XML_3MF_BEAMLATTICE_CAPMODE_HEMISPHERE      L"hemisphere"
#define XML_3MF_BEAMLATTICE_CAPMODE_BUTT            L"butt"
// depricated (from a draft version of the spec):
#define XML_3MF_BEAMLATTICE_CAPMODE_ROUND           L"round"


#define XML_3MF_PRODUCTION_PATH                     L"path"
#define XML_3MF_PRODUCTION_UUID                     L"UUID"

#define XML_3MF_LANG_US                             L"en-US"

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
