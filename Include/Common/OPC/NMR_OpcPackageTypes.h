/*++

Copyright (C) 2015 netfabb GmbH (Original Author)

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

NMR_OpcPackageTypes.h defines an OPC Package types.

--*/

#ifndef __NMR_OPCPACKAGETYPES
#define __NMR_OPCPACKAGETYPES

#define OPCPACKAGE_PATH_CONTENTTYPES L"[Content_Types].xml"

#define OPCPACKAGE_PATH_ROOTRELATIONSHIPS L"_rels/.rels"
#define OPCPACKAGE_SCHEMA_CONTENTTYPES L"http://schemas.openxmlformats.org/package/2006/content-types"
#define OPCPACKAGE_SCHEMA_RELATIONSHIPS L"http://schemas.openxmlformats.org/package/2006/relationships"

#define OPC_CONTENTTYPES_CONTAINER L"Types"
#define OPC_CONTENTTYPES_NODE L"Default"
#define OPC_CONTENTTYPES_NODE_OVERRIDE L"Override"
#define OPC_CONTENTTYPES_ATTRIB_EXTENSION L"Extension"
#define OPC_CONTENTTYPES_ATTRIB_CONTENTTYPE L"ContentType"
#define OPC_CONTENTTYPES_ATTRIB_PARTNAME L"PartName"

#define OPC_RELS_RELATIONSHIP_CONTAINER L"Relationships"
#define OPC_RELS_RELATIONSHIP_NODE L"Relationship"

#define OPC_RELS_ATTRIB_TARGET L"Target"
#define OPC_RELS_ATTRIB_TYPE L"Type"
#define OPC_RELS_ATTRIB_ID L"Id"

#endif // __NMR_OPCPACKAGETYPES
