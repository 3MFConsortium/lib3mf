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

Abstract: This is the class declaration of CImage3D

*/


#ifndef __LIB3MF_IMAGE3D
#define __LIB3MF_IMAGE3D

#include "lib3mf_interfaces.hpp"

// Parent classes
#include "lib3mf_resource.hpp"
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4250)
#endif

// Include custom headers here.
#include "Model/Classes/NMR_ModelImage3D.h"
#include "Model/Classes/NMR_Model.h"

namespace Lib3MF {
namespace Impl {


/*************************************************************************************************************************
 Class declaration of CImage3D 
**************************************************************************************************************************/

class CImage3D : public virtual IImage3D, public virtual CResource {
private:

protected:

	NMR::PModelImage3D m_pImage3D;

public:

	CImage3D (NMR::PModelImage3D pModelImage3D);
	
	Lib3MF_uint32 GetSizeX();

	Lib3MF_uint32 GetSizeY();

	Lib3MF_uint32 GetSheetCount();

	IAttachment * GetSheet(const Lib3MF_uint32 nIndex);

	Lib3MF_double GetSheetMinValue(const Lib3MF_uint32 nIndex);

	Lib3MF_double GetSheetMaxValue(const Lib3MF_uint32 nIndex);

	IAttachment * CreateEmptySheet(const Lib3MF_uint32 nIndex, const std::string & sPath, const Lib3MF_double dMin, const Lib3MF_double dMax);

	IAttachment * CreateSheetFromBuffer(const Lib3MF_uint32 nIndex, const std::string & sPath, const Lib3MF_uint64 nDataBufferSize, const Lib3MF_uint8 * pDataBuffer, const Lib3MF_double dMin, const Lib3MF_double dMax);

	IAttachment * CreateSheetFromFile(const Lib3MF_uint32 nIndex, const std::string & sPath, const std::string & sFileName, const Lib3MF_double dMin, const Lib3MF_double dMax);

	void SetSheet(const Lib3MF_uint32 nIndex, IAttachment* pSheet);

	void SetSheetMinValue(const Lib3MF_uint32 nIndex, const Lib3MF_double dMinVal);

	void SetSheetMaxValue(const Lib3MF_uint32 nIndex, const Lib3MF_double dMaxVal);

	NMR::PModelImage3D getModelImage3D ();

};

} // namespace Impl
} // namespace Lib3MF

#ifdef _MSC_VER
#pragma warning(pop)
#endif
#endif // __LIB3MF_IMAGE3D
