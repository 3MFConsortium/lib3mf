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

Abstract: This file implements utilities for the lib3mf API

*/

#include "lib3mf_utils.hpp"
#include "lib3mf_interfaceexception.hpp"

// Include custom headers here.


// using namespace Lib3MF::Impl;

NMR::NMATRIX3 Lib3MF::TransformToMatrix(const sLib3MFTransform Transform)
{
	NMR::NMATRIX3 matrix;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++)
			matrix.m_fields[j][i] = Transform.m_Fields[i][j];
		matrix.m_fields[3][i] = 0.f + 1.0f*(i == 3);
	}
	return matrix;
}

sLib3MFTransform Lib3MF::MatrixToTransform(const NMR::NMATRIX3 matrix)
{
	sLib3MFTransform transform;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++)
			transform.m_Fields[i][j] = matrix.m_fields[j][i];
	}
	return transform;
}


eLib3MFEncryptionAlgorithm Lib3MF::translateEncryptionAlgorithm(const NMR::eKeyStoreEncryptAlgorithm algorithm) {
	if (NMR::eKeyStoreEncryptAlgorithm::AES256_GCM == algorithm)
		return eLib3MFEncryptionAlgorithm::AES256_GCM;
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}
NMR::eKeyStoreEncryptAlgorithm Lib3MF::translateEncryptionAlgorithm(const eLib3MFEncryptionAlgorithm algorithm) {
	if (eLib3MFEncryptionAlgorithm::AES256_GCM == algorithm)
		return NMR::eKeyStoreEncryptAlgorithm::AES256_GCM;
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}

eLib3MFWrappingAlgorithm Lib3MF::translateWrappingAlgorithm(const NMR::eKeyStoreWrapAlgorithm algorithm) {
	if (NMR::eKeyStoreWrapAlgorithm::RSA_OAEP == algorithm)
		return eLib3MFWrappingAlgorithm::RSA_OAEP;
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}
NMR::eKeyStoreWrapAlgorithm Lib3MF::translateWrappingAlgorithm(const eLib3MFWrappingAlgorithm algorithm) {
	if (eLib3MFWrappingAlgorithm::RSA_OAEP == algorithm)
		return NMR::eKeyStoreWrapAlgorithm::RSA_OAEP;
	throw ELib3MFInterfaceException(LIB3MF_ERROR_NOTIMPLEMENTED);
}



eLib3MFCompression Lib3MF::translateCompression(bool compression) {
	if (compression)
		return eLib3MFCompression::Deflate;
	return eLib3MFCompression::NoCompression;
}

bool Lib3MF::translateCompression(const eLib3MFCompression compression) {
	return eLib3MFCompression::Deflate == compression;
}