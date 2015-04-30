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

NMR_ModelWriterNode_Model.h defines the Model Writer Model Node Class.
This is the class for exporting the 3mf model stream root node.

--*/

#ifndef __NMR_MODELWRITERNODE100_MODEL
#define __NMR_MODELWRITERNODE100_MODEL

#include "Model/Writer/NMR_ModelWriterNode.h" 
#include "Model/Classes/NMR_ModelComponentsObject.h" 
#include "Common/Platform/NMR_XmlWriter.h"

namespace NMR {

	class CModelWriterNode100_Model : public CModelWriterNode {
	protected:
		void writeResources();
		void writeBuild();

		void writeComponentsObject(_In_ CModelComponentsObject * pComponentsObject);
	public:
		CModelWriterNode100_Model() = delete;
		CModelWriterNode100_Model(_In_ CModel * pModel, _In_ CXmlWriter * pXMLWriter);
		
		virtual void writeToXML();
	};

}

#endif // __NMR_MODELWRITERNODE100_MODEL
