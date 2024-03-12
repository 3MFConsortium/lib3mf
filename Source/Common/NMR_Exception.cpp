/*++

Copyright (C) 2019 3MF Consortium

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

NMR_Exception.cpp implements the generic Exception Class.
Each exception is identified via a global ErrorCode
(to be looked up in NMR_ErrorConst.h)

--*/

#include "Common/NMR_Exception.h"
#include <cmath>

namespace NMR {

	CNMRException::CNMRException(_In_ nfError errorcode) : std::exception()
	{
		m_errorcode = errorcode;
	}

	const char * CNMRException::what() const throw ()
	{
		switch (m_errorcode) {
		// Success / user interaction (0x0XXX)
		case NMR_USERABORTED: return "The called function was aborted by the user";
		// General error codes (0x1XXX)
		case NMR_ERROR_NOTIMPLEMENTED: return "The called function is not fully implemented";
		case NMR_ERROR_INVALIDPARAM: return "The call parameter to the function was invalid";
		case NMR_ERROR_CALCULATIONTERMINATED: return "The Calculation has to be canceled";
		case NMR_ERROR_COULDNOTLOADLIBRARY: return "The DLL Library of the DLL Filters could not be loaded";
		case NMR_ERROR_GETPROCFAILED: return "The DLL Library of the DLL Filters is invalid";
		case NMR_ERROR_DLLNOTLOADED: return "The DLL Library has not been loaded or could not be loaded";
		case NMR_ERROR_DLLFUNCTIONNOTFOUND: return "The DLL Library of the DLL Filters is invalid";
		case NMR_ERROR_DLLINVALIDPARAM: return "The DLL Library has got an invalid parameter";
		case NMR_ERROR_DLLNOINSTANCE: return "No Instance of the DLL has been created";
		case NMR_ERROR_DLLINVALIDFILTERNAME: return "The DLL does not support the suspected filters";
		case NMR_ERROR_DLLMISSINGPARAMETER: return "Not all parameters are provided to the DLL";
		case NMR_ERROR_INVALIDBLOCKSIZE: return "The provided Blocksize is invalid (like in CPagedVector)";
		case NMR_ERROR_INVALIDINDEX: return "The provided Index is invalid (like in CPagedVector, Node Index)";
		case NMR_ERROR_SINGULARMATRIX: return "A Matrix could not be inverted in the Matrix functions (as it is singular)";
		case NMR_ERROR_MODELMISMATCH: return "The Model Object does not match the model which is it added to";
		case NMR_ERROR_ABSTRACT: return "The function called is abstract and should not have been called";
		case NMR_ERROR_INVALIDHEADBLOCK: return "The current block is not assigned";
		case NMR_ERROR_COMINITIALIZATIONFAILED: return "COM CoInitialize failed";
		case NMR_ERROR_STANDARDCPPEXCEPTION: return "A Standard C++ Exception occurred";
		case NMR_ERROR_INVALIDMESH: return "No mesh has been given";
		case NMR_ERROR_COULDNOTCREATECONTEXT: return "Context could not be created";
		case NMR_ERROR_EMPTYSTRINGTOINTCONVERSION: return "Wanted to convert empty string to integer";
		case NMR_ERROR_INVALIDSTRINGTOINTCONVERSION: return "Wanted to convert string with non-numeric characters to integer";
		case NMR_ERROR_STRINGTOINTCONVERSIONOUTOFRANGE: return "Wanted to convert too large number string to integer";
		case NMR_ERROR_EMPTYSTRINGTODOUBLECONVERSION: return "Wanted to convert empty string to double";
		case NMR_ERROR_INVALIDSTRINGTODOUBLECONVERSION: return "Wanted to convert string with non-numeric characters to double";
		case NMR_ERROR_STRINGTODOUBLECONVERSIONOUTOFRANGE: return "Wanted to convert too large number string to double";
		case NMR_ERROR_TOOMANYVALUESINMATRIXSTRING: return "Too many values (>12) have been found in a matrix string";
		case NMR_ERROR_NOTENOUGHVALUESINMATRIXSTRING: return "Not enough values (<12) have been found in a matrix string";
		case NMR_ERROR_INVALIDBUFFERSIZE: return "Invalid buffer size";
		case NMR_ERROR_INSUFFICIENTBUFFERSIZE: return "Insufficient buffer size";
		case NMR_ERROR_INVALIDCOMPONENT: return "No component has been given";
		case NMR_ERROR_INVALIDHEXVALUE: return "Invalid hex value";
		case NMR_ERROR_RANGEERROR: return "Range error";
		case NMR_ERROR_INVALIDPOINTER: return "Passed invalid null pointer";
		case NMR_ERROR_XMLELEMENTNOTOPEN: return "XML Element not open";
		case NMR_ERROR_INVALIDXMLNAME: return "Invalid XML Name";
		case NMR_ERROR_INVALIDINTEGERTRIPLET: return "Invalid Integer Triplet String";
		case NMR_ERROR_INVALIDZIPENTRYKEY: return "Invalid ZIP Entry key";
		case NMR_ERRORINVALIDZIPNAME: return "Invalid ZIP Name";
		case NMR_ERROR_ZIPSTREAMCANNOTSEEK: return "ZIP Stream cannot seek";
		case NMR_ERROR_COULDNOTCONVERTTOUTF8: return "Could not convert to UTF8";
		case NMR_ERROR_COULDNOTCONVERTTOUTF16: return "Could not convert to UTF16";
		case NMR_ERROR_ZIPENTRYOVERFLOW: return "ZIP Entry overflow";
		case NMR_ERROR_INVALIDZIPENTRY: return "Invalid ZIP Entry";
		case NMR_ERROR_EXPORTSTREAMNOTEMPTY: return "Export Stream not empty";
		case NMR_ERROR_DEFLATEINITFAILED: return "Deflate init failed";
		case NMR_ERROR_ZIPALREADYFINISHED: return "Zip already finished";
		case NMR_ERROR_COULDNOTDEFLATE: return "Could not deflate data";
		case NMR_ERROR_XMLWRITER_CLOSENODEERROR: return "Could not close written XML node";
		case NMR_ERROR_INVALIDOPCPARTURI: return "Invalid OPC Part URI";
		case NMR_ERROR_COULDNOTCONVERTNUMBER: return "Could not convert number";
		case NMR_ERROR_COULDNOTGETSTREAMPOSITION: return "Could not get stream position";
		case NMR_ERROR_COULDNOTREADZIPFILE: return "Could not read ZIP file";
		case NMR_ERROR_COULDNOTSEEKINZIP: return "Could not seek in ZIP file";
		case NMR_ERROR_COULDNOTSTATZIPENTRY: return "Could not stat ZIP entry";
		case NMR_ERROR_COULDNOTOPENZIPENTRY: return "Could not open ZIP entry";
		case NMR_ERROR_INVALIDXMLDEPTH: return "Invalid XML Depth";
		case NMR_ERROR_XMLELEMENTNOTEMPTY: return "XML Element not empty";
		case NMR_ERROR_COULDNOTINITITALIZECOM: return "Could not initialize COM";
		case NMR_ERROR_CALLBACKSTREAMCANNOTSEEK: return "Callback stream cannot seek";
		case NMR_ERROR_COULDNOTWRITETOCALLBACKSTREAM: return "Could not write to callback stream";
		case NMR_ERROR_INVALIDCAST: return "Invalid Type Case";
		case NMR_ERROR_BUFFERISFULL: return "Buffer is full";
		case NMR_ERROR_COULDNOTREADFROMCALLBACKSTREAM: return "Could not read from callback stream";
		case NMR_ERROR_OPC_MISSING_EXTENSION_FOR_RELATIONSHIP: return "Content Types does not contain extension for relatioship.";
		case NMR_ERROR_OPC_MISSING_EXTENSION_FOR_MODEL: return "Content Types does not contain extension or partname for model.";
		case NMR_ERROR_INVALIDXMLENCODING: return "Document is not UTF-8 encoded.";
		case NMR_ERROR_FORBIDDENXMLATTRITIBUTE: return "Document contains a forbidden XML-attribute.";
		case NMR_ERROR_DUPLICATE_PRINTTICKET: return "Document contains more than one printticket.";
		case NMR_ERROR_OPC_DUPLICATE_RELATIONSHIP_ID: return "Document contains a duplicate relationship ID.";
		case NMR_ERROR_INVALIDRELATIONSHIPTYPEFORTEXTURE: return "A texture must use a OPC part with relationshiptype 3D Texture.";
		case NMR_ERROR_IMPORTSTREAMISEMPTY: return "An attachment to be read does not have any content.";
		case NMR_ERROR_UUIDGENERATIONFAILED: return "Generation of a UUID failed.";
		case NMR_ERROR_ZIPENTRYNON64_TOOLARGE: return "A ZIP Entry is too large for non zip64 zip-file";
		case NMR_ERROR_ATTACHMENTTOOLARGE: return "An individual custom attachment is too large.";
		case NMR_ERROR_ZIPCALLBACK: return "Error in libzip callback.";
		case NMR_ERROR_ZIPCONTAINSINCONSISTENCIES: return "ZIP file contains inconsistencies. It might load with errors or incorrectly.";
		case NMR_ERROR_XMLNAMESPACEALREADYREGISTERED: return "An XML namespace is already registered.";
		case NMR_ERROR_XMLPREFIXALREADYREGISTERED: return "An XML prefix is already registered.";
		case NMR_ERROR_COULDNOTINITINFLATE: return "Failed to initialize a zlib buffer";
		case NMR_ERROR_COULDNOTINFLATE: return "Failed to decompress part";
		case NMR_ERROR_COULDNOTINITDEFLATE: return "Failed to initialize a zlib buffer";
		case NMR_ERROR_TOOMANYSLICES: return "Too many slices";

		// Unhandled exception
		case NMR_ERROR_GENERICEXCEPTION: return NMR_GENERICEXCEPTIONSTRING;


		// Core framework error codes (0x2XXX)
		case NMR_ERROR_NOPROGRESSINTERVAL: return "No Progress Interval has been specified in the progress handler";
		case NMR_ERROR_DUPLICATENODE: return "An Edge with two identical nodes has been tried to added to a mesh";
		case NMR_ERROR_TOOMANYNODES: return "The mesh exceeds more than NMR_MESH_MAXEDGECOUNT (2^31-1, around two billion) nodes";
		case NMR_ERROR_TOOMANYFACES: return "The mesh exceeds more than NMR_MESH_MAXFACECOUNT (2^31-1, around two billion) faces";
		case NMR_ERROR_INVALIDNODEINDEX: return "The index provided for the node is invalid";
		case NMR_ERROR_INVALIDFACEINDEX: return "The index provided for the face is invalid";
		case NMR_ERROR_INVALIDMESHTOPOLOGY: return "The mesh topology structure is corrupt";
		case NMR_ERROR_INVALIDCOORDINATES: return "The coordinates exceed NMR_MESH_MAXCOORDINATE (= 1 billion mm)";
		case NMR_ERROR_NORMALIZEDZEROVECTOR: return "A zero Vector has been tried to normalized, which is impossible";
		case NMR_ERROR_COULDNOTOPENFILE: return "The specified file could not be opened";
		case NMR_ERROR_COULDNOTCREATEFILE: return "The specified file could not be created";
		case NMR_ERROR_COULDNOTSEEKSTREAM: return "Seeking in a stream was not possible";
		case NMR_ERROR_COULDNOTREADSTREAM: return "Reading from a stream was not possible";
		case NMR_ERROR_COULDNOTWRITESTREAM: return "Writing to a stream was not possible";
		case NMR_ERROR_COULDNOTREADFULLDATA: return "Reading from a stream was only possible partially";
		case NMR_ERROR_COULDNOTWRITEFULLDATA: return "Writing to a stream was only possible partially";
		case NMR_ERROR_NOIMPORTSTREAM: return "No Import Stream was provided to the importer";
		case NMR_ERROR_INVALIDFACECOUNT: return "The specified facecount in the file was not valid";
		case NMR_ERROR_INVALIDUNITS: return "The specified units of the file was not valid";
		case NMR_ERROR_COULDNOTSETUNITS: return "The specified units could not be set (for example, the CVectorTree already had some entries)";
		case NMR_ERROR_TOOMANYEDGES: return "The mesh exceeds more than NMR_MESH_MAXEDGECOUNT (2^31-1, around two billion) edges";
		case NMR_ERROR_INVALIDEDGEINDEX: return "The index provided for the edge is invalid";
		case NMR_ERROR_DUPLICATEEDGE: return "The mesh has an face with two identical edges";
		case NMR_ERROR_MANIFOLDEDGES: return "Could not add face to an edge, because it was already two-manifold";
		case NMR_ERROR_COULDNOTDELETEEDGE: return "Could not delete edge, because it had attached faces";
		case NMR_ERROR_INTERNALMERGEERROR: return "Mesh Merging has failed, because the mesh structure was currupted";
		case NMR_ERROR_EDGESARENOTFORMINGTRIANGLE: return "The internal triangle structure is corrupted";
		case NMR_ERROR_NOEXPORTSTREAM: return "No Export Stream was provided to the exporter";
		case NMR_ERROR_COULDNOTSETPARAMETER: return "Could not set parameter, because the queue was not empty";
		case NMR_ERROR_INVALIDRECORDSIZE: return "Mesh Information records size is invalid";
		case NMR_ERROR_MESHINFORMATIONCOUNTMISMATCH: return "Mesh Information Face Count dies not match with mesh face count";
		case NMR_ERROR_INVALIDMESHINFORMATIONINDEX: return "Could not access mesh information";
		case NMR_ERROR_MESHINFORMATIONBUFFER_FULL: return "Mesh Information Backup could not be created";
		case NMR_ERROR_NOMESHINFORMATIONCONTAINER: return "No Mesh Information Container has been assigned";
		case NMR_ERROR_DISCRETEMERGEERROR: return "Internal Mesh Merge Error because of corrupt mesh structure";
		case NMR_ERROR_DISCRETEEDGELENGTHVIOLATION: return "Discrete Edges may only have a max length of 30000.";
		case NMR_ERROR_OCTTREE_OUTOFBOUNDS: return "OctTree Node is out of the OctTree Structure";
		case NMR_ERROR_COULDNOTDELETENODE: return "Could not delete mesh node, because it still had some edges connected to it";
		case NMR_ERROR_INVALIDINFORMATIONTYPE: return "Mesh Information has not been found";
		case NMR_ERROR_FACESARENOTIDENTICAL: return "Mesh Information could not be copied";
		case NMR_ERROR_DUPLICATETEXTURE: return "Texture is already existing";
		case NMR_ERROR_DUPLICATETEXTUREID: return "Texture ID is already existing";
		case NMR_ERROR_PARTTOOLARGE: return "Part is too large";
		case NMR_ERROR_DUPLICATETEXTUREPATH: return "Texture getPath is already existing";
		case NMR_ERROR_DUPLICATETEXTUREWIDTH: return "Texture width is already existing";
		case NMR_ERROR_DUPLICATETEXTUREHEIGHT: return "Texture height is already existing";
		case NMR_ERROR_DUPLICATETEXTUREDEPTH: return "Texture depth is already existing";
		case NMR_ERROR_DUPLICATETEXTURECONTENTTYPE: return "Texture content type is already existing";
		case NMR_ERROR_DUPLICATETEXTUREU: return "Texture U coordinate is already existing";
		case NMR_ERROR_DUPLICATETEXTUREV: return "Texture V coordinate is already existing";
		case NMR_ERROR_DUPLICATETEXTUREW: return "Texture W coordinate is already existing";
		case NMR_ERROR_DUPLICATETEXTURESCALE: return "Texture scale is already existing";
		case NMR_ERROR_DUPLICATETEXTUREROTATION: return "Texture rotation is already existing";
		case NMR_ERROR_DUPLICATETILESTYLEU: return "Texture tilestyle U is already existing";
		case NMR_ERROR_DUPLICATETILESTYLEV: return "Texture tilestyle V is already existing";
		case NMR_ERROR_DUPLICATETILESTYLEW: return "Texture tilestyle W is already existing";
		case NMR_ERROR_DUPLICATECOLORID: return "Color ID is already existing";
		case NMR_ERROR_INVALIDMESHINFORMATIONDATA: return "Mesh Information Block was not assigned";
		case NMR_ERROR_INVALIDMESHINFORMATION: return "Mesh Information Object was not assigned";
		case NMR_ERROR_TOOMANYBEAMS: return "The mesh exceeds more than NMR_MESH_MAXBEAMCOUNT (2^31-1, around two billion) beams";

		// Model error codes (0x8XXX)
		case NMR_ERROR_OPCREADFAILED: return "3MF Loading - OPC could not be loaded";
		case NMR_ERROR_NOMODELSTREAM: return "No model stream in OPC Container";
		case NMR_ERROR_MODELREADFAILED: return "Model XML could not be parsed";
		case NMR_ERROR_NO3MFOBJECT: return "No 3MF Object in OPC Container";
		case NMR_ERROR_COULDNOTWRITEMODELSTREAM: return "Could not write Model Stream to OPC Container";
		case NMR_ERROR_OPCFACTORYCREATEFAILED: return "Could not create OPC Factory";
		case NMR_ERROR_OPCPARTSETREADFAILED: return "Could not read OPC Part Set";
		case NMR_ERROR_OPCRELATIONSHIPSETREADFAILED: return "Could not read OPC Relationship Set";
		case NMR_ERROR_OPCRELATIONSHIPSOURCEURIFAILED: return "Could not get Relationship Source URI";
		case NMR_ERROR_OPCRELATIONSHIPTARGETURIFAILED: return "Could not get Relationship Target URI";
		case NMR_ERROR_OPCRELATIONSHIPCOMBINEURIFAILED: return "Could not Combine Relationship URIs";
		case NMR_ERROR_OPCRELATIONSHIPGETPARTFAILED: return "Could not get Relationship Part";
		case NMR_ERROR_OPCGETCONTENTTYPEFAILED: return "Could not retrieve content type";
		case NMR_ERROR_OPCCONTENTTYPEMISMATCH: return "Content type mismatch";
		case NMR_ERROR_OPCRELATIONSHIPENUMERATIONFAILED: return "Could not enumerate relationships";
		case NMR_ERROR_OPCRELATIONSHIPNOTFOUND: return "Could not find relationship type";
		case NMR_ERROR_OPCRELATIONSHIPNOTUNIQUE: return "Relationship type not unique";
		case NMR_ERROR_OPCCOULDNOTGETMODELSTREAM: return "Could not get OPC Model Stream";
		case NMR_ERROR_CREATEXMLREADERFAILED: return "Could not create XML Reader";
		case NMR_ERROR_SETXMLREADERINPUTFAILED: return "Could not set XML reader input";
		case NMR_ERROR_COULDNOTSEEKMODELSTREAM: return "Could not seek in XML Model Stream";
		case NMR_ERROR_SETXMLPROPERTIESFAILED: return "Could not set XML reader properties";
		case NMR_ERROR_READXMLNODEFAILED: return "Could not read XML node";
		case NMR_ERROR_COULDNOTGETLOCALXMLNAME: return "Could not retrieve local xml node name";
		case NMR_ERROR_COULDPARSEXMLCONTENT: return "Could not parse XML Node content";
		case NMR_ERROR_COULDNOTGETXMLTEXT: return "Could not get XML Node value";
		case NMR_ERROR_COULDNOTGETXMLATTRIBUTES: return "Could not retrieve XML Node attributes";
		case NMR_ERROR_COULDNOTGETXMLVALUE: return "Could not get XML attribute value";
		case NMR_ERROR_ALREADYPARSEDXMLNODE: return "XML Node has already been parsed";
		case NMR_ERROR_INVALIDMODELUNIT: return "Invalid Model Unit";
		case NMR_ERROR_INVALIDMODELOBJECTID: return "Invalid Model Object ID";
		case NMR_ERROR_MISSINGMODELOBJECTID: return "No Model Object ID has been given";
		case NMR_ERROR_DUPLICATEMODELOBJECT: return "Model Object is already existing";
		case NMR_ERROR_DUPLICATEOBJECTID: return "Model Object ID was given twice";
		case NMR_ERROR_AMBIGUOUSOBJECTDEFINITON: return "Model Object Content was ambiguous";
		case NMR_ERROR_MODELCOORDINATEMISSING: return "Model Vertex is missing a coordinate";
		case NMR_ERROR_INVALIDMODELCOORDINATES: return "Invalid Model Coordinates";
		case NMR_ERROR_INVALIDMODELCOORDINATEINDICES: return "Invalid Model Coordinate Indices";
		case NMR_ERROR_NODENAMEISEMPTY: return "XML Node Name is empty";
		case NMR_ERROR_INVALIDMODELNODEINDEX: return "Invalid model node index";
		case NMR_ERROR_OPCPACKAGECREATEFAILED: return "Could not create OPC Package";
		case NMR_ERROR_COULDNOTWRITEOPCPACKAGETOSTREAM: return "Could not write OPC Package to Stream";
		case NMR_ERROR_COULDNOTCREATEOPCPARTURI: return "Could not create OPC Part URI";
		case NMR_ERROR_COULDNOTCREATEOPCPART: return "Could not create OPC Part";
		case NMR_ERROR_OPCCOULDNOTGETCONTENTSTREAM: return "Could not get OPC Content Stream";
		case NMR_ERROR_OPCCOULDNOTRESIZESTREAM: return "Could not resize OPC Stream";
		case NMR_ERROR_OPCCOULDNOTSEEKSTREAM: return "Could not seek in OPC Stream";
		case NMR_ERROR_OPCCOULDNOTCOPYSTREAM: return "Could not copy OPC Stream";
		case NMR_ERROR_COULDNOTRETRIEVEOPCPARTNAME: return "Could not retrieve OPC Part name";
		case NMR_ERROR_COULDNOTCREATEOPCRELATIONSHIP: return "Could not create OPC Relationship";
		case NMR_ERROR_COULDNOTCREATEXMLWRITER: return "Could not create XML Writer";
		case NMR_ERROR_COULDNOTSETXMLOUTPUT: return "Could not set XML Output stream";
		case NMR_ERROR_COULDNOTSETXMLPROPERTY: return "Could not set XML Property";
		case NMR_ERROR_COULDNOTWRITEXMLSTARTDOCUMENT: return "Could not write XML Start Document";
		case NMR_ERROR_COULDNOTWRITEXMLENDDOCUMENT: return "Could not write XML End Document";
		case NMR_ERROR_COULDNOTFLUSHXMLWRITER: return "Could not flush XML Writer";
		case NMR_ERROR_COULDNOTWRITEXMLSTARTELEMENT: return "Could not write XML Start Element";
		case NMR_ERROR_COULDNOTWRITEXMLENDELEMENT: return "Could not write XML End Element";
		case NMR_ERROR_COULDNOTWRITEXMLATTRIBUTE: return "Could not write XML Attribute String";
		case NMR_ERROR_MISSINGBUILDITEMOBJECTID: return "Build item Object ID was not specified";
		case NMR_ERROR_DUPLICATEBUILDITEMOBJECTID: return "Build item Object ID is ambiguous ";
		case NMR_ERROR_INVALIDBUILDITEMOBJECTID: return "Build item Object ID is invalid";
		case NMR_ERROR_COULDNOTFINDBUILDITEMOBJECT: return "Could not find Object associated to the Build item ";
		case NMR_ERROR_COULDNOTFINDCOMPONENTOBJECT: return "Could not find Object associated to Component";
		case NMR_ERROR_DUPLICATECOMPONENTOBJECTID: return "Component Object ID is ambiguous ";
		case NMR_ERROR_MISSINGMODELTEXTUREID: return "Texture ID was not specified";
		case NMR_ERROR_MISSINGOBJECTCONTENT: return "An object has no supported content type";
		case NMR_ERROR_INVALIDREADEROBJECT: return "Invalid model reader object";
		case NMR_ERROR_INVALIDWRITEROBJECT: return "Invalid model writer object";
		case NMR_ERROR_UNKNOWNMODELRESOURCE: return "Unknown model resource";
		case NMR_ERROR_INVALIDSTREAMTYPE: return "Invalid stream type";
		case NMR_ERROR_DUPLICATEMATERIALID: return "Duplicate Material ID";
		case NMR_ERROR_DUPLICATEWALLTHICKNESS: return "Duplicate Wallthickness";
		case NMR_ERROR_DUPLICATEFIT: return "Duplicate Fit";
		case NMR_ERROR_DUPLICATEOBJECTTYPE: return "Duplicate Object Type";
		case NMR_ERROR_MODELTEXTURECOORDINATEMISSING: return "Texture coordinates missing";
		case NMR_ERROR_TOOMANYVALUESINCOLORSTRING: return "Too many values in color string";
		case NMR_ERROR_INVALIDVALUEINCOLORSTRING: return "Invalid value in color string";
		case NMR_ERROR_DUPLICATECOLORVALUE: return "Duplicate node color value";
		case NMR_ERROR_MISSINGMODELCOLORID: return "Missing model color ID";
		case NMR_ERROR_MISSINGMODELMATERIALID: return "Missing model material ID";
		case NMR_ERROR_INVALIDBUILDITEM: return "No Build Item has been given";
		case NMR_ERROR_INVALIDOBJECT: return "No Object has been given";
		case NMR_ERROR_INVALIDMODEL: return "No Model has been given";
		case NMR_ERROR_INVALIDMODELRESOURCE: return "No valid Model Resource has been given";
		case NMR_ERROR_DUPLICATEMETADATA: return "Duplicate Model Metadata";
		case NMR_ERROR_INVALIDMETADATA: return "Invalid Model Metadata";
		case NMR_ERROR_INVALIDMODELCOMPONENT: return "Invalid Model Component";
		case NMR_ERROR_INVALIDMODELOBJECTTYPE: return "Invalid Model Object Type";
		case NMR_ERROR_MISSINGMODELRESOURCEID: return "Missing Model Resource ID";
		case NMR_ERROR_DUPLICATERESOURCEID: return "Duplicate Resource ID";
		case NMR_ERROR_COULDNOTWRITEXMLCONTENT: return "Could not write XML Content";
		case NMR_ERROR_COULDNOTGETNAMESPACE: return "Could not get XML Namespace";
		case NMR_ERROR_HANDLEOVERFLOW: return "Handle overflow";
		case NMR_ERROR_NORESOURCES: return "No resources in model file";
		case NMR_ERROR_NOBUILD: return "No build section in model file";
		case NMR_ERROR_DUPLICATERESOURCES: return "Duplicate resources section in model file";
		case NMR_ERROR_DUPLICATEBUILDSECTION: return "Duplicate build section in model file";
		case NMR_ERROR_DUPLICATEMODELNODE: return "Duplicate model node in XML Stream";
		case NMR_ERROR_NOMODELNODE: return "No model node in XML Stream";
		case NMR_ERROR_RESOURCENOTFOUND: return "Resource not found";
		case NMR_ERROR_UNKNOWNREADERCLASS: return "Unknown reader class";
		case NMR_ERROR_UNKNOWNWRITERCLASS: return "Unknown writer class";
		case NMR_ERROR_MODELTEXTURENOTFOUND: return "Texture not found";
		case NMR_ERROR_INVALIDCONTENTTYPE: return "Invalid Content Type";
		case NMR_ERROR_INVALIDBASEMATERIAL: return "Invalid Base Material";
		case NMR_ERROR_TOOMANYMATERIALS: return "Too many materials";
		case NMR_ERROR_INVALIDTEXTURE: return "Invalid texture";
		case NMR_ERROR_COULDNOTGETHANDLE: return "Could not get handle";
		case NMR_ERROR_BUILDITEMNOTFOUND: return "Build item not found";
		case NMR_ERROR_OPCCOULDNOTGETTEXTUREURI: return "Could not get texture URI";
		case NMR_ERROR_MODELRELATIONSHIPSETREADFAILED: return "Model Relationship read failed";
		case NMR_ERROR_NOTEXTURESTREAM: return "Texture stream is not available";
		case NMR_ERROR_COULDNOTCREATESTREAM: return "Could not create stream";
		case NMR_ERROR_NOTSUPPORTINGLEGACYCMYK: return "Not supporting legacy CMYK color";
		case NMR_ERROR_INVALIDTEXTUREREFERENCE: return "Invalid Texture Reference";
		case NMR_ERROR_INVALIDTEXTUREID: return "Invalid Texture ID";
		case NMR_ERROR_NOMODELTOWRITE: return "No model to write";
		case NMR_ERROR_OPCRELATIONSHIPGETTYPEFAILED: return "Failed to get OPC Relationship type";
		case NMR_ERROR_OPCCOULDNOTGETATTACHMENTURI: return "Could not get attachment URI";
		case NMR_ERROR_DUPLICATEATTACHMENTPATH: return "Duplicate Attachment Path";
		case NMR_ERROR_INVALIDMODELATTACHMENT: return "Invalid Model Attachment";
		case NMR_ERROR_ATTACHMENTNOTFOUND: return "Could not find Model Attachment";
		case NMR_ERROR_INVALIDREQUIREDEXTENSIONPREFIX: return "The prefix of a required extension is invalid";
		case NMR_ERROR_REQUIREDEXTENSIONNOTSUPPORTED: return "A required extension is not supported";
		case NMR_ERROR_BEAMLATTICECLIPPINGRESOURCENOTDEFINED: return "The resource defined as clippingmesh has not yet been defined in the model";
		case NMR_ERROR_BEAMLATTICEINVALIDATTRIBUTE: return "An attribute of the beamlattice is invalid";
		case NMR_ERROR_OPCCOULDNOTGETSLICEREFURI: return "Could not get sliceref URI";
		case NMR_ERROR_OPCCOULDNOTGETSLICEREFSTREAM: return "Could not get sliceref stream";
		case NMR_ERROR_OPCCOULDNOTGETATTACHMENTSTREAM: return "Could not get attachment stream";
		case NMR_ERROR_DUPLICATE_SLICESTACKID: return "Object has dublicate slicestack ID";
		case NMR_ERROR_SLICESTACKRESOURCE_NOT_FOUND: return "Could not find Slicestack Resource";
		case NMR_ERROR_SLICESTACK_SLICESANDSLICEREF: return "Slicestack contains slices and slicerefs";
		case NMR_ERROR_ILLFORMATUUID: return "A UUID is ill formatted";
		case NMR_ERROR_INVALIDSLICESTACK: return "A slice stack resource is invalid";
		case NMR_ERROR_DUPLICATEPATH: return "Duplicate getPath attribute";
		case NMR_ERROR_DUPLICATEUUID: return "Duplicate UUID attribute";
		case NMR_ERROR_REFERENCESTOODEEP: return "References in production extension go deeper than one level.";
		case NMR_ERROR_SLICEREFSTOODEEP: return "A slicestack referenced via a slicepath cannot reference another slicestack.";
		case NMR_ERROR_SLICES_Z_NOTINCREASING: return "The z-coordinates of slices within a slicestack are not increasing.";
		case NMR_ERROR_SLICEPOLYGONNOTCLOSED: return "A slice polygon of a model- or solidsupport-object is not closed.";
		case NMR_ERROR_CLOSED_SLICE_POLYGON_IS_LINE: return "A closed slice polygon is actually a line.";
		case NMR_ERROR_NAMESPACE_INVALID_ELEMENT: return "Invalid Element in namespace.";
		case NMR_ERROR_NAMESPACE_INVALID_ATTRIBUTE: return "Invalid Attribute in namespace.";
		case NMR_ERROR_DUPLICATETEZTOP: return "Duplicate Z-top-value in a slice.";
		case NMR_ERROR_MISSINGTEZTOP: return "Z-top-value is missing in a slice.";
		case NMR_ERROR_SLICE_INVALIDATTRIBUTE: return "Invalid attribute in slice extension";
		case NMR_ERROR_SLICETRANSFORMATIONPLANAR: return "A slicestack posesses a nonplanar transformation.";
		case NMR_ERROR_UUID_NOT_UNIQUE: return "A UUID is not unique within a package.";
		case NMR_ERROR_METADATA_COULDNOTGETNAMESPACE: return "Could not get XML Namespace for a metadatum.";
		case NMR_ERROR_INVALID_SLICESEGMENT_VERTEXINDEX: return "Invalid index for slice segment or polygon.";
		case NMR_ERROR_MISSINGUUID: return "A UUID for a build, build item or object is missing.";
		case NMR_ERROR_INVALID_SLICEPATH: return "A slicepath is invalid.";
		case NMR_ERROR_UNKNOWNMETADATA: return "Unknown Model Metadata.";
		case NMR_ERROR_DUPLICATE_MESHRESOLUTION: return "Object has duplicate meshresolution attribute.";
		case NMR_ERROR_INVALID_MESHRESOLUTION: return "Object has invalid value for meshresolution attribute.";
		case NMR_ERROR_INVALIDREADERWARNINGSOBJECT: return "Invalid model reader warnings object.";
		case NMR_ERROR_OPCCOULDNOTGETTHUMBNAILSTREAM: return "Could not get OPC thumbnail stream.";
		case NMR_ERROR_DUPLICATEOBJECTTHUMBNAIL: return "Duplicate object thumbnail.";
		case NMR_ERROR_DUPLICATETHUMBNAIL: return "Duplicate thumbnail.";
		case NMR_ERROR_DUPLICATEPID: return "Duplicate Property ID.";
		case NMR_ERROR_DUPLICATEPINDEX: return "Duplicate Property Index.";
		case NMR_ERROR_MISSINGOBJECTLEVELPID: return "A MeshObject with triangle-properties is missing an object-level property.";
		case NMR_ERROR_INVALIDOBJECTLEVELPID: return "A MeshObject with triangle-properties has an invalid object-level property.";
		case NMR_ERROR_BUILDITEMOBJECT_MUSTNOTBE_OTHER: return "Build-item must not reference object of type OTHER.";
		case NMR_ERROR_OBJECTLEVELPID_ON_COMPONENTSOBJECT: return "A components object must not have an object-level PID.";
		case NMR_ERROR_BEAMLATTICENODESTOOCLOSE: return "Nodes used for a beam are closer then the specified minimal length.";
		case NMR_ERROR_BEAMLATTICE_INVALID_REPRESENTATIONRESOURCE: return "The resource defined as representationmesh is invalid.";
		case NMR_ERROR_BEAMLATTICE_INVALID_OBJECTTYPE: return "Beamlattice is defined on wrong object type.";
		case NMR_ERROR_SLICE_ONEVERTEX: return "Slice only contains one vertex.";
		case NMR_ERROR_SLICE_ONEPOINT: return "Slice contains only one point within a polygon";
		case NMR_ERROR_INVALIDTILESTYLE: return "Invalid Tile Style";
		case NMR_ERROR_INVALIDFILTER: return "Invalid Filter";
		case NMR_ERROR_DUPLICATEMETADATAGROUP: return "Duplicate MetaDataGroup";
		case NMR_ERROR_SLICES_MIXING_SLICES_WITH_SLICEREFS: return "A SliceStack must not contain slices and slicerefs";
		case NMR_ERROR_SLICES_SLICEREF_CIRCULAR: return "SliceStack references must not be circular";
		case NMR_ERROR_SLICES_REFS_Z_NOTINCREASING: return "z-position of slicerefs is not increasing";
		case NMR_ERROR_SLICES_REFS_LEVELTOODEEP: return "level of slicereferences is too deep";
		case NMR_ERROR_PROPERTYIDALREADYREGISTERED: return "Property already registered";
		case NMR_ERROR_INVALIDPROPERTYRESOURCEID: return "Invalid Property Resource ID";
		case NMR_ERROR_PROPERTYIDNOTFOUND: return "Property ID not found";
		case NMR_ERROR_TOOMANYCOLORS: return "Too many colors.";
		case NMR_ERROR_TOOMANYCOMPOSITES: return "Too many composites.";
		case NMR_ERROR_MIXINGRATIO_OUTOFRANGE: return "Mixing ratio of composite out of range.";
		case NMR_ERROR_MIXINGRATIO_MISSING: return "A composite mixing ratio is missing.";
		case NMR_ERROR_MIXINGRATIO_TOOMANY: return "Too many composite mixing ratio in the list of values";
		case NMR_ERROR_MULTIPROPERTIES_MUST_NOT_CONTAIN_MULTIPROPERTIES: return "A multiproperties must not contain another multiproperties";
		case NMR_ERROR_MULTIPROPERTIES_MUST_NOT_CONTAIN_MULTIPLE_MATERIALS: return "A multiproperties must not contain multiple materials";
		case NMR_ERROR_MULTIPROPERTIES_MUST_NOT_CONTAIN_MULTIPLE_COLORGOURPS: return "A multiproperties must not contain multiple colorgroups";
		case NMR_ERROR_MULTIPROPERTIES_INVALID_RESOURCE: return "A resource of invalid type was passed to a MultiPropertyGroup";
		case NMR_ERROR_MULTIPROPERTIES_NOT_ENOUGH_PROPERTYIDS_SPECIFIED: return "A MultiProperty does not define enough PropertyIDs";
		case NMR_ERROR_MULTIPROPERTIES_MISSING_PIDS: return "A MultiProperty-element is missing the pids-attribute";
		case NMR_ERROR_MULTIPROPERTIES_DIFFERNT_NUMBER_OF_BLENDMETHODS_AND_PIDS: return "The number of blendmethods and pids does not match";
		case NMR_ERROR_DUPLICATE_PIDS: return "Multiproperties element has duplicate pids attribute";
		case NMR_ERROR_DUPLICATE_BLENDMETHOS: return "Multiproperties element has duplicate blendmethods attribute";
		case NMR_ERROR_INVALID_BLENDMETHOD_ATTRIBUTE: return "A blendmethods attribute is invalid";
		case NMR_ERROR_MULTIPROPERTIES_INVALID_MULTI_ELEMENT: return "A multi-element is invalid";
		case NMR_ERROR_INVALID_RESOURCE_INDEX: return "A Resource Index is invalid";
		case NMR_ERROR_VERSION093_NOT_SUPPORTED: return "This document contains content from Version 093 of the core-specification. This is not fully supported by Lib3MF version 2 or later.";
		case NMR_ERROR_ATTACHMENTMODELMISMATCH: return "The model of an attachment does not match the model of a resource.";
		case NMR_ERROR_DUPLICATEPACKAGEPATH: return "The PackagePath of a model is not unique.";
		case NMR_ERROR_PRODUCTIONEXTENSION_REQUIRED: return "Serialization of this Model requires the production extension.";
		case NMR_ERROR_MODELRESOURCE_IN_DIFFERENT_MODEL: return "Referenced model resource must not be in a different model.";
		case NMR_ERROR_PATH_NOT_ABSOLUTE: return "A path attribute element is not absolute.";
		case NMR_ERROR_BEAMSET_IDENTIFIER_NOT_UNIQUE: return "A beamset identifier is not unique.";
		case NMR_ERROR_DUPLICATETRANSFORM: return "A transform attribute is duplicated";
		case NMR_ERROR_INVALIDCOMPONENTRESOURCE: return "A component resource is invalid.";
		
			//keystore error codes
		case NMR_ERROR_KEYSTOREDUPLICATECONSUMER: return "A consumer already exists for this consumerid";
		case NMR_ERROR_KEYSTOREDUPLICATECONSUMERID: return "The attribute consumerid is duplicated";
		case NMR_ERROR_KEYSTOREDUPLICATECONSUMERKEYID: return "The KeyStore Consumer key id is not unique ";
		case NMR_ERROR_KEYSTOREDUPLICATERESOURCEDATAPATH: return "The attribute path is duplicated";
		case NMR_ERROR_KEYSTOREDUPLICATECONSUMERINDEX: return "The attribute consumerindex is duplicated";
		case NMR_ERROR_KEYSTOREDUPLICATERESOURCEDATA: return "A ResourceData already exists for this path in the KeyStore";
		case NMR_ERROR_KEYSTOREDUPLICATEACCESSRIGHT: return "An AccessRight already exists for this consumer in a ResourceDataGroup";
		case NMR_ERROR_KEYSTOREDUPLICATERESOURCEDATAGROUP: return "A resource data group already exist for this keyuuid";
		case NMR_ERROR_KEYSTOREINVALIDALGORITHM: return "The algorithm attribute is invalid";
		case NMR_ERROR_KEYSTOREINVALIDENCODING: return "The keystore element value is not base64 encoded.";
		case NMR_ERROR_KEYSTOREINVALIDCOMPRESSION: return "The KeyStore ResourceData compression is invalid";
		case NMR_ERROR_KEYSTOREINVALIDCIPHERVALUE: return "Invalid CipherValue elment value";
		case NMR_ERROR_KEYSTOREINVALIDMGF: return "The mfgalgorithm attribute has invalid value";
		case NMR_ERROR_KEYSTOREINVALIDDIGEST: return "The digestmethod attribute has invalid value";
		case NMR_ERROR_KEYSTOREINVALIDCONSUMERINDEX: return "The attribute consumerindex is invalid";
		case NMR_ERROR_KEYSTOREINVALIDKEYUUID: return "Attribute keyuuid is invalid, a default value has been assigned.";
		case NMR_ERROR_KEYSTOREMISSINGCIPHERDATA: return "Element cipherdata is missing";
		case NMR_ERROR_KEYSTOREMISSINGCONSUMERID: return "Attribute consumerid is missing, a default value has been assigned.";
		case NMR_ERROR_KEYSTOREMISSINGCONSUMERINDEX: return "Attribute consumerindex is missing, a default value has been assigned.";
		case NMR_ERROR_KEYSTOREMISSINGKEKPARAMS: return "Element kekparams is missing, , default values have been assigned.";
		case NMR_ERROR_KEYSTOREMISSINGCEKPARAMS: return "Element cekparams is missing.";
		case NMR_ERROR_KEYSTOREMISSINGKEYUUID: return "Attribute keyuuid is missing, , a default value has been assigned.";
		case NMR_ERROR_KEYSTOREMISSINGPATH: return "Attribute path is missing on a resouredata";
		case NMR_ERROR_KEYSTOREMISSINGALGORTHM: return "An algorithm attribute is missing, a default value has been assigned.";
		case NMR_ERROR_KEYSTOREINCONSISTENTKEKPARAMS: return "Element kekparams ha invalid configuration";
		case NMR_ERROR_KEYSTOREOPCCOULDNOTGETSTREAM: return "Could not get KeyStore stream";
		case NMR_ERROR_KEYSTOREUNSUPPORTEDALGORITHM: return "The algorithm attribute is unsupported";
		case NMR_ERROR_KEYSTORETOOMANYELEMENTS: return "Too many elements added to a keystore tree";

		// XML Parser Error Constants(0x9XXX)
		case NMR_ERROR_XMLPARSER_INVALIDATTRIBVALUE: return "Invalid XML attribute value";
		case NMR_ERROR_XMLPARSER_INVALIDPARSERESULT: return "Invalid XML parse result";
		case NMR_ERROR_XMLPARSER_TOOMANYUSEDCHARS: return "Too many XML characters used";
		case NMR_ERROR_XMLPARSER_INVALIDENDDELIMITER: return "Invalid XML end delimiter";
		case NMR_ERROR_XMLPARSER_INVALIDNAMESPACEPREFIX: return "Invalid XML namespace prefix";
		case NMR_ERROR_XMLPARSER_COULDNOTPARSEENTITY: return "Could not parse XML entity";
		case NMR_ERROR_XMLPARSER_EMPTYELEMENTNAME: return "Empty XML element name";
		case NMR_ERROR_XMLPARSER_INVALIDCHARACTERINELEMENTNAME: return "Invalid characters in XML element name";
		case NMR_ERROR_XMLPARSER_EMPTYINSTRUCTIONNAME: return "Empty XML instruction name";
		case NMR_ERROR_XMLPARSER_INVALIDINSTRUCTIONNAME: return "Invalid XML instruction name";
		case NMR_ERROR_XMLPARSER_COULDNOTCLOSEINSTRUCTION: return "Could not close XML instruction";
		case NMR_ERROR_XMLPARSER_COULDNOTENDELEMENT: return "Could not end XML element";
		case NMR_ERROR_XMLPARSER_EMPTYENDELEMENT: return "Empty XML end element";
		case NMR_ERROR_XMLPARSER_COULDNOTCLOSEELEMENT: return "Could not close XML element";
		case NMR_ERROR_XMLPARSER_INVALIDATTRIBUTENAME: return "Invalid XML attribute name";
		case NMR_ERROR_XMLPARSER_SPACEINATTRIBUTENAME: return "Space in XML attribute name";
		case NMR_ERROR_XMLPARSER_NOQUOTESAROUNDATTRIBUTE: return "No quotes around XML attribute";
		case NMR_ERROR_DUPLICATE_RELATIONSHIP: return "A relationship is duplicated.";
		case NMR_ERROR_DUPLICATE_CONTENTTYPE: return "A content type is duplicated.";
		case NMR_ERROR_CONTENTTYPE_EMPTY_EXTENSION: return "A content type does not have a valid extension.";
		case NMR_ERROR_CONTENTTYPE_EMPTY_CONTENTTYPE: return "A content type does not have a content type-value.";
		case NMR_ERROR_CONTENTTYPE_EMPTY_PARTNAME: return "An override content type does not have a partname.";
		case NMR_ERROR_XMLPARSER_INVALID_ESCAPESTRING: return "XML contains an invalid escape character.";
		case NMR_ERROR_DUPLICATE_BOX_ATTRIBUTE: return "A box attribute is duplicated.";
		case NMR_ERROR_DUPLICATE_MATINDICES_ATTRIBUTE: return "A matindices attribute is duplicated.";

		// Library errors (0xAXXX)
		case NMR_ERROR_COULDNOTGETINTERFACEVERSION: return "Could not get interface version";
		case NMR_ERROR_INVALIDINTERFACEVERSION: return "Invalid interface version";
		case NMR_ERROR_INVALIDSTREAMSIZE: return "Invalid stream size";
		case NMR_ERROR_INVALIDNAMELENGTH: return "Invalid name length";
		case NMR_ERROR_COULDNOTCREATEMODEL: return "Could not create model";
		case NMR_ERROR_INVALIDTEXTURETYPE: return "Invalid Texture type";
		case NMR_ERROR_KEKDESCRIPTORNOTFOUND: return "Required key encryption client was not registered for a consumer";
		case NMR_ERROR_DEKDESCRIPTORNOTFOUND: return "Required data encryption client was not registered";
		case NMR_ERROR_RNGCALLBACKNOTCRYPTOSTRONG: return "Using cryptographically weak random number generator";

		default:
			return "unknown error";
		}
	}

	nfError CNMRException::getErrorCode() const
	{
		return m_errorcode;
	}

}
