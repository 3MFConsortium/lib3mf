#ifndef NMR_ENCRYPTIONHEADER
#define NMR_ENCRYPTIONHEADER

#include <vector>
#include "Common/NMR_Types.h"
#include "Common/Platform/NMR_ImportStream.h"
#include "Common/Platform/NMR_ExportStream.h"
namespace NMR {

	union uEncryptedFileHeader {
		nfByte bytes[12];
		struct sHeaderStructure {
			union uMagic {
				nfByte bytes[5];
				char * string;
			} Signature;
			nfByte majorVersion;
			nfByte minorVersion;
			nfByte unused;
			union uLength {
				nfUint32 length;
				nfByte bytes[4];
			} Length;
		} Header;
	};

	class CEncryptionHeader {
	public:
		std::vector<nfByte> readFrom(PImportStream from);
		void writeTo(PExportStream to, std::vector<nfByte> const & additionalData);
	};
}

#endif // !NMR_ENCRYPTIONHEADER
