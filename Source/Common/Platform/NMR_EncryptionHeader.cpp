

#include "Common/Platform/NMR_EncryptionHeader.h"
#include "Common/NMR_ErrorConst.h"
#include "Common/NMR_Exception.h"
namespace NMR {
	std::vector<nfByte> CEncryptionHeader::readFrom(PImportStream from) {
		uEncryptedFileHeader header = { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } };
		from->readBuffer(header.bytes, sizeof(header), true);
		if (strncmp(header.Header.Signature.string, "%3McF", sizeof(header.Header.Signature.bytes)) != 0)
			throw CNMRException(NMR_ERROR_COULDNOTREADENCRYPTEDSTREAM);
		if (header.Header.majorVersion != 0 || header.Header.minorVersion != 0)
			throw CNMRException(NMR_ERROR_ENCRYPTEDCONTENTVERSIONUNSUPPORTED);
		nfUint32 remainingBytes = header.Header.Length.length - sizeof(uEncryptedFileHeader);
		std::vector<nfByte> remainingBuffer(remainingBytes, 0);
		if (remainingBytes > 0) {
			from->readBuffer(remainingBuffer.data(), remainingBytes, true);
		}
		return remainingBuffer;
	}
	void CEncryptionHeader::writeTo(PExportStream to, std::vector<nfByte> const & additionalData) {
		uEncryptedFileHeader header = { {'%', '3', 'M', 'c', 'F', 0, 0, 0, 0, 0, 0, 12} };
		if (!additionalData.empty())
			header.Header.Length.length += (nfUint32)additionalData.size();
		to->writeBuffer(header.bytes, sizeof(header));
		to->writeBuffer(additionalData.data(), additionalData.size());
	}
}