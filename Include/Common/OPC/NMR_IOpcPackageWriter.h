#ifndef NMR_IOPCPACKAGEWRITER
#define NMR_IOPCPACKAGEWRITER

#include <memory>
#include <string>
#include <list>
#include "Common/NMR_Types.h"
#include "Common/NMR_Local.h"

namespace NMR {
	class COpcPackageRelationship;
	using POpcPackageRelationship = std::shared_ptr<COpcPackageRelationship>;
	class COpcPackagePart;
	using POpcPackagePart = std::shared_ptr<COpcPackagePart>;

	class IOpcPackageWriter {
	public:
		virtual POpcPackagePart addPart(_In_ std::string sPath) = 0;
		virtual void addContentType(_In_ std::string sExtension, _In_ std::string sContentType) = 0;
		virtual void addContentType(_In_ POpcPackagePart pOpcPackagePart, _In_ std::string sContentType) = 0;
		virtual POpcPackageRelationship addRootRelationship(_In_ std::string sType, _In_ COpcPackagePart * pTargetPart) = 0;
		virtual POpcPackageRelationship addPartRelationship(_In_ POpcPackagePart pOpcPackagePart, _In_ std::string sType, _In_ COpcPackagePart * pTargetPart) = 0;
		virtual std::list<POpcPackageRelationship> addWriterSpecificRelationships(_In_ POpcPackagePart pOpcPackagePart, _In_ COpcPackagePart* pTargetPart) = 0;
		virtual void close() {}
	};

	using PIOpcPackageWriter = std::shared_ptr<IOpcPackageWriter>;
}
#endif // !NMR_IOPCPACKAGEWRITER
