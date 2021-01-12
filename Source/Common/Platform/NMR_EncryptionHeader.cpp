

#include "Common/Platform/NMR_EncryptionHeader.h"
#include "Common/NMR_ErrorConst.h"
#include "Common/NMR_Exception.h"

#include "Common/Platform/NMR_ImportStream.h"
#include "Common/Platform/NMR_ExportStream.h"

#include "Common/NMR_Architecture_Utils.h"
#include <cstring>
namespace NMR {

	CEncryptionHeader::CEncryptionHeader(std::vector<nfByte> const & additionalData)
		:m_rgAdditionalData(additionalData)
	{
	}

	size_t CEncryptionHeader::readFrom(PImportStream from) {
		uEncryptedFileHeader header = { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } };
		from->readBuffer(header.bytes, sizeof(header), true);
		if (isBigEndian()) {
			header.Header.Length.length = swapBytes(header.Header.Length.length);
		}
		constexpr size_t sigSize = sizeof(header.Header.Signature.bytes);
		if (strncmp((char *)header.Header.Signature.bytes, "%3McF", sigSize) != 0)
			throw CNMRException(NMR_ERROR_COULDNOTREADENCRYPTEDSTREAM);
		if (header.Header.majorVersion != 0 || header.Header.minorVersion != 0)
			throw CNMRException(NMR_ERROR_ENCRYPTEDCONTENTVERSIONUNSUPPORTED);
		constexpr size_t headerSize = sizeof(header);
		nfUint32 remainingBytes = header.Header.Length.length - headerSize;
		if (remainingBytes > 0) {
			m_rgAdditionalData.resize(remainingBytes, 0);
			from->readBuffer(m_rgAdditionalData.data(), remainingBytes, true);
		}
		m_nfHeaderSize = header.Header.Length.length;
		return m_nfHeaderSize;
	}
	size_t CEncryptionHeader::writeTo(PExportStream to) {
		uEncryptedFileHeader header = { { '%', '3', 'M', 'c', 'F', 0, 0, 0, 0, 0, 0, 0 } };
		constexpr size_t headerSize = sizeof(header);
		m_nfHeaderSize = (nfUint32)(headerSize + m_rgAdditionalData.size());
		header.Header.Length.length = (nfUint32)m_nfHeaderSize;
		if (isBigEndian()) {
			header.Header.Length.length = swapBytes(header.Header.Length.length);
		}
		to->writeBuffer(header.bytes, headerSize);
		if (m_rgAdditionalData.size() > 0)
			to->writeBuffer(m_rgAdditionalData.data(), m_rgAdditionalData.size());
		return m_nfHeaderSize;
	}
	std::vector<nfByte> const & CEncryptionHeader::additionalData() const {
		return m_rgAdditionalData;
	}
	nfUint64 CEncryptionHeader::headerSize() const {
		return m_nfHeaderSize;
	}
}