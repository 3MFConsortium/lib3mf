#include "Common/Platform/NMR_ImportStream_Encrypted.h"
#include "Common/NMR_Exception.h"
#include <vector>
namespace NMR {
	CImportStream_Encrypted::CImportStream_Encrypted(PImportStream pEncryptedStream, DEKDESCRIPTOR pDecryptContext)
		:m_pEncryptedStream(pEncryptedStream), m_pDecryptContext(pDecryptContext)
	{
		if (nullptr == pEncryptedStream)
			throw CNMRException(NMR_ERROR_INVALIDPOINTER);
	}

	nfBool CImportStream_Encrypted::seekPosition(nfUint64 position, nfBool bHasToSucceed) {
		return m_pEncryptedStream->seekPosition(position, bHasToSucceed);
	}

	nfBool CImportStream_Encrypted::seekForward(nfUint64 bytes, nfBool bHasToSucceed) {
		return m_pEncryptedStream->seekForward(bytes, bHasToSucceed);
	}

	nfBool CImportStream_Encrypted::seekFromEnd(nfUint64 bytes, nfBool bHasToSucceed) {
		return m_pEncryptedStream->seekFromEnd(bytes, bHasToSucceed);
	}

	nfUint64 CImportStream_Encrypted::readBuffer(nfByte * pBuffer, nfUint64 cbTotalBytesToRead, nfBool bNeedsToReadAll) {
		nfUint64 n = m_pEncryptedStream->readBuffer(pBuffer, cbTotalBytesToRead, bNeedsToReadAll);
		std::vector<nfByte> decBuffer(cbTotalBytesToRead, 0);
		m_pDecryptContext.m_fnDecrypt(pBuffer, cbTotalBytesToRead, decBuffer.data(), m_pDecryptContext.m_sDekDecryptData);
		std::copy(decBuffer.begin(), decBuffer.end(), pBuffer);
		return n;
	}

	nfUint64 CImportStream_Encrypted::retrieveSize() {
		return m_pEncryptedStream->retrieveSize();
	}

	void CImportStream_Encrypted::writeToFile(const nfWChar * pwszFileName) {
		m_pEncryptedStream->writeToFile(pwszFileName);
	}

	PImportStream CImportStream_Encrypted::copyToMemory() {
		return m_pEncryptedStream->copyToMemory();
	}

	nfUint64 CImportStream_Encrypted::getPosition() {
		return m_pEncryptedStream->getPosition();
	}


}