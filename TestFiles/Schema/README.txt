This folder contains the XML Document Specifications for the
 core, material, production and slice
extenstions to 3MF.

_Usage:_
../../Tools/xmllint/xmllint.exe --schema wrapper.xsd model.xml --noout
wrapper.xsd is a wrapper to allow validation agains multiple xml schemata at once.

_Important Notes:_
 - production.xsd seems to be broken
	1. production_2015_06.xsd:33: element simpleType: Schemas parser error : Element '{http://www.w3.org/2001/XMLSchema}attribute': The attribute 'type' and the <simpleType> child are mutually exclusive. WXS schema wrapper.xsd failed to compile
	2. ST_ResourceID, mesh, components, ST_ObjectType, ST_ResourceIndex, ST_UriReference and ID are not defined/imported properly
   Therefore, it is currently excluded from the validation.
 - The value "1073741824" for maxOccurs should read "2147483647" in all *.xsd-files. "1073741824" (=2^31) is used, however, because xmllint cannot work with higher int values for maxOccurs.
 - In contrast to the embedded xsd in the CoreSpec of 3MF, the build-items in the file core_2015_02.xsd has an attribute "partnumber". This is in alignment with the text of the CoreSpec and the implementation in 3MF. This should be updated in the embedded xsd of the CoreSpec of 3MF.
