#ifndef NMR_ENCRYPTIONHEADER
#define NMR_ENCRYPTIONHEADER

#include <vector>
#include <memory>
#include "Common/NMR_Types.h"
#include "Common/NMR_Local.h"

namespace NMR {
	class CImportStream;
	using PImportStream = std::shared_ptr<CImportStream>;
	class CExportStream;
	using PExportStream = std::shared_ptr<CExportStream>;

	union uEncryptedFileHeader {
		nfByte bytes[12];
		struct sHeaderStructure {
			union uMagic {
				nfByte bytes[5];
			} Signature;
			nfByte majorVersion;
			nfByte minorVersion;
			nfByte unused;
			union uLength {
				nfByte bytes[4];
				nfUint32 length;
			} Length;
		} Header;
	};

	class CEncryptionHeader {
		std::vector<nfByte> m_rgAdditionalData;
		nfUint64 m_nfHeaderSize;
	public:
		CEncryptionHeader(std::vector<nfByte> const & additionalData = std::vector<nfByte>());
		size_t readFrom(PImportStream from);
		size_t writeTo(PExportStream to);

		std::vector<nfByte> const & additionalData() const;
		nfUint64 headerSize() const;
	};
}

#endif // !NMR_ENCRYPTIONHEADER
