#include "Common/Platform/NMR_ImportStream_Encrypted.h"
#include "Common/Platform/NMR_ImportStream_Unique_Memory.h"
#include "Common/NMR_Exception.h"
#include <vector>
#include <array>
namespace NMR {
	CImportStream_Encrypted::CImportStream_Encrypted(PImportStream pEncryptedStream, ContentEncryptionDescriptor pDecryptContext)
		:m_pEncryptedStream(pEncryptedStream), m_pDecryptContext(pDecryptContext)
	{
		if (nullptr == pEncryptedStream)
			throw CNMRException(NMR_ERROR_INVALIDPOINTER);
		//checkHeader();
	}

	void CImportStream_Encrypted::checkHeader() {
		union EncryptedFileHeader {
			nfByte bytes[12];
			struct HeaderStructure {
				union Magic {
					nfByte bytes[5];
					char * string;
				} Signature;
				nfByte majorVersion;
				nfByte minorVersion;
				nfByte unused;
				union Length {
					nfUint32 length;
					nfByte bytes[4];
				} Length;
			} Header;
		};
		EncryptedFileHeader header = { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} };
		m_pEncryptedStream->readBuffer(header.bytes, sizeof(header), true);
		if (strncmp(header.Header.Signature.string, "%3McF", sizeof(header.Header.Signature.bytes)) != 0)
			throw CNMRException(NMR_ERROR_COULDNOTREADENCRYPTEDSTREAM);
		if (header.Header.majorVersion != 0 || header.Header.minorVersion != 0)
			throw CNMRException(NMR_ERROR_ENCRYPTEDCONTENTFORMATNOTSUPPORTED);
		nfUint32 remainingBytes = header.Header.Length.length - sizeof(EncryptedFileHeader);
		if (remainingBytes > 0) {
			std::vector<nfByte> remainingBuffer(remainingBytes, 0);
			m_pEncryptedStream->readBuffer(remainingBuffer.data(), remainingBytes, true);
		}
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
		std::vector<nfByte> decBuffer(cbTotalBytesToRead, 0);
		nfUint64 n = m_pEncryptedStream->readBuffer(decBuffer.data(), cbTotalBytesToRead, bNeedsToReadAll);
		if (n > 0) {
			nfUint64 decryted = m_pDecryptContext.m_fnCrypt(n, decBuffer.data(), pBuffer, m_pDecryptContext.m_sDekDecryptData);
			if (decryted != n)
				throw CNMRException(NMR_ERROR_CALCULATIONTERMINATED);
		}
		return n;
	}

	nfUint64 CImportStream_Encrypted::retrieveSize() {
		return m_pEncryptedStream->retrieveSize();
	}

	void CImportStream_Encrypted::writeToFile(const nfWChar * pwszFileName) {
		m_pEncryptedStream->writeToFile(pwszFileName);
	}

	PImportStream CImportStream_Encrypted::copyToMemory() {
		nfUint64 cbStreamSize = retrieveSize();
		return std::make_shared<CImportStream_Unique_Memory>(this, cbStreamSize, false);
	}

	nfUint64 CImportStream_Encrypted::getPosition() {
		return m_pEncryptedStream->getPosition();
	}


}