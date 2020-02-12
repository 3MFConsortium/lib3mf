#include "Common/Platform/NMR_ExportStream_Encrypted.h"
#include "Common/NMR_Exception.h"

namespace NMR {
	CExportStream_Encrypted::CExportStream_Encrypted(PExportStream pEncryptedStream, DEKDESCRIPTOR context)
		:m_pEncryptedStream(pEncryptedStream), m_pDecryptContext(context)
	{
		if (nullptr == pEncryptedStream)
			throw CNMRException(NMR_ERROR_INVALIDPOINTER);
	}

	nfBool CExportStream_Encrypted::seekPosition(nfUint64 position, nfBool bHasToSucceed)
	{
		return m_pEncryptedStream->seekPosition(position, bHasToSucceed);
	}

	nfBool CExportStream_Encrypted::seekForward(nfUint64 bytes, nfBool bHasToSucceed)
	{
		return m_pEncryptedStream->seekForward(bytes, bHasToSucceed);
	}

	nfBool CExportStream_Encrypted::seekFromEnd(nfUint64 bytes, nfBool bHasToSucceed)
	{
		return m_pEncryptedStream->seekFromEnd(bytes, bHasToSucceed);
	}

	nfUint64 CExportStream_Encrypted::getPosition()
	{
		return m_pEncryptedStream->getPosition();
	}

	nfUint64 CExportStream_Encrypted::writeBuffer(const void * pBuffer, nfUint64 cbTotalBytesToWrite)
	{
		std::vector<nfByte> encryptBuffer(cbTotalBytesToWrite, 0);
		auto encryptedBytes = m_pDecryptContext.m_fnCrypt(cbTotalBytesToWrite, (const nfByte *)pBuffer, encryptBuffer.data(), m_pDecryptContext.m_sDekDecryptData);
		__NMRASSERT(encryptedBytes == cbTotalBytesToWrite)
		auto writtenBytes = m_pEncryptedStream->writeBuffer(encryptBuffer.data(), encryptedBytes);
		return writtenBytes;
	}

}