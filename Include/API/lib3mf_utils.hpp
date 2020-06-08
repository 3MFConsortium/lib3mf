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

Abstract: This file contains utilties used in the API of lib3mf

*/


#ifndef __LIB3MF_UTILS
#define __LIB3MF_UTILS

#include "lib3mf_interfaces.hpp"

// Include custom headers here.
#include "Common/Math/NMR_Matrix.h" 
#include "Common/NMR_SecureContentTypes.h"

namespace Lib3MF {

NMR::NMATRIX3 TransformToMatrix(const sLib3MFTransform Transform);

sLib3MFTransform MatrixToTransform(const NMR::NMATRIX3 matrix);

eLib3MFEncryptionAlgorithm translateEncryptionAlgorithm(const NMR::eKeyStoreEncryptAlgorithm algorithm);

NMR::eKeyStoreEncryptAlgorithm translateEncryptionAlgorithm(const eLib3MFEncryptionAlgorithm algorithm);

eLib3MFWrappingAlgorithm translateWrappingAlgorithm(const NMR::eKeyStoreWrapAlgorithm algorithm);

NMR::eKeyStoreWrapAlgorithm translateWrappingAlgorithm(const eLib3MFWrappingAlgorithm algorithm);

eLib3MFCompression translateCompression(bool compression);
bool translateCompression(const eLib3MFCompression compression);

}

#endif // __LIB3MF_UTILS
