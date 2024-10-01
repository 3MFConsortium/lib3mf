/*++

Copyright (C) 2019 3MF Consortium (Original Author)

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

Abstract: This is the class declaration of CBinaryStream

*/


#ifndef __LIB3MF_BINARYSTREAM
#define __LIB3MF_BINARYSTREAM

#include "lib3mf_interfaces.hpp"

// Parent classes
#include "lib3mf_base.hpp"
#pragma warning( push)
#pragma warning( disable : 4250)

// Include custom headers here.
#include "Common/ChunkedBinaryStream/NMR_ChunkedBinaryStreamWriter.h"

namespace Lib3MF {
namespace Impl {


/*************************************************************************************************************************
 Class declaration of CBinaryStream 
**************************************************************************************************************************/

class CBinaryStream : public virtual IBinaryStream, public virtual CBase {
private:

	NMR::PChunkedBinaryStreamWriter m_pStreamWriter;
	std::string m_sPackageIndexPath;
	std::string m_sPackageBinaryPath;
	std::string m_sUUID;

protected:


public:

	CBinaryStream (std::string sPackageIndexPath, std::string sPackageBinaryPath, NMR::PChunkedBinaryStreamWriter pStreamWriter);

	std::string GetBinaryPath();
	std::string GetIndexPath();
	std::string GetUUID();


	void DisableDiscretizedArrayCompression();
	void EnableDiscretizedArrayCompression(const Lib3MF_double dUnits, const Lib3MF::eBinaryStreamPredictionType ePredictionType);

	void EnableLZ4(const Lib3MF_uint32 nCompressionLevel) override;

	void EnableZLib(const Lib3MF_uint32 nCompressionLevel) override;

	void EnableZstd(const Lib3MF_uint32 nCompressionLevel) override;

};

} // namespace Impl
} // namespace Lib3MF

#pragma warning( pop )
#endif // __LIB3MF_BINARYSTREAM
