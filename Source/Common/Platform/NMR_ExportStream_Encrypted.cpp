#include "Common/Platform/NMR_ExportStream_Encrypted.h"
#include "Common/NMR_Exception.h"

namespace NMR {
	CExportStream_Encrypted::CExportStream_Encrypted(PExportStream pEncryptedStream, ContentEncryptionDescriptor context)
		:m_pEncryptedStream(pEncryptedStream), m_pDecryptContext(context)
	{
		if (nullptr == pEncryptedStream)
			throw CNMRException(NMR_ERROR_INVALIDPOINTER);
		m_header.writeTo(m_pEncryptedStream);
	}

	nfBool CExportStream_Encrypted::seekPosition(nfUint64 position, nfBool bHasToSucceed)
	{
		return m_pEncryptedStream->seekPosition(position + m_header.headerSize(), bHasToSucceed);
	}

	nfBool CExportStream_Encrypted::seekForward(nfUint64 bytes, nfBool bHasToSucceed)
	{
		return m_pEncryptedStream->seekForward(bytes, bHasToSucceed);
	}

	nfBool CExportStream_Encrypted::seekFromEnd(nfUint64 bytes, nfBool bHasToSucceed)
	{
		if (m_pEncryptedStream->getPosition() + m_header.headerSize() >= bytes) {
			return m_pEncryptedStream->seekFromEnd(bytes, bHasToSucceed);
		} else if (bHasToSucceed) {
			throw CNMRException(NMR_ERROR_COULDNOTSEEKSTREAM);
		}
		return false;
	}

	nfUint64 CExportStream_Encrypted::getPosition()
	{
		return m_pEncryptedStream->getPosition() - m_header.headerSize();
	}

	nfUint64 CExportStream_Encrypted::writeBuffer(const void * pBuffer, nfUint64 cbTotalBytesToWrite)
	{
		nfUint64 encryptedBytes = 0;
		if (cbTotalBytesToWrite > 0) {
			std::vector<nfByte> encryptBuffer(cbTotalBytesToWrite, 0);
			encryptedBytes = m_pDecryptContext.m_fnCrypt(cbTotalBytesToWrite, (const nfByte *)pBuffer, encryptBuffer.data(), m_pDecryptContext.m_sDekDecryptData);
			if (encryptedBytes > 0) {
				auto writtenBytes = m_pEncryptedStream->writeBuffer(encryptBuffer.data(), encryptedBytes);
				if (encryptedBytes != writtenBytes)
					throw CNMRException(NMR_ERROR_CALCULATIONTERMINATED);
			}
		}
		return encryptedBytes;
	}

}