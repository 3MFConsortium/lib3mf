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

Abstract: This is the class declaration of CImageStack

*/


#ifndef __LIB3MF_IMAGESTACK
#define __LIB3MF_IMAGESTACK

#include "lib3mf_interfaces.hpp"

// Parent classes
#include "lib3mf_image3d.hpp"
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4250)
#endif

// Include custom headers here.
#include "Model/Classes/NMR_ModelImageStack.h"

namespace Lib3MF {
namespace Impl {


/*************************************************************************************************************************
 Class declaration of CImageStack 
**************************************************************************************************************************/

class CImageStack : public virtual IImageStack, public virtual CImage3D {
private:

	/**
	* Put private members here.
	*/
	NMR::PModelImageStack m_pModelImageStack;

protected:

	/**
	* Put protected members here.
	*/

public:

	/**
	* Put additional public members here. They will not be visible in the external API.
	*/
	CImageStack() = delete;
	CImageStack(NMR::PModelImageStack pModelImageStack);

	/**
	* Public member functions to implement.
	*/

	Lib3MF_uint32 GetRowCount() override;

	void SetRowCount(const Lib3MF_uint32 nRowCount) override;

	Lib3MF_uint32 GetColumnCount() override;

	void SetColumnCount(const Lib3MF_uint32 nColumnCount) override;

	Lib3MF_uint32 GetSheetCount() override;

	IAttachment * GetSheet(const Lib3MF_uint32 nIndex) override;

	void SetSheet(const Lib3MF_uint32 nIndex, IAttachment* pSheet) override;

	IAttachment * CreateEmptySheet(const Lib3MF_uint32 nIndex, const std::string & sPath) override;

	IAttachment * CreateSheetFromBuffer(const Lib3MF_uint32 nIndex, const std::string & sPath, const Lib3MF_uint64 nDataBufferSize, const Lib3MF_uint8 * pDataBuffer) override;

	IAttachment * CreateSheetFromFile(const Lib3MF_uint32 nIndex, const std::string & sPath, const std::string & sFileName) override;

};

} // namespace Impl
} // namespace Lib3MF

#ifdef _MSC_VER
#pragma warning(pop)
#endif
#endif // __LIB3MF_IMAGESTACK
