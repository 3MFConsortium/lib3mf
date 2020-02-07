#ifndef NMR_IOPCPACKAGEWRITER
#define NMR_IOPCPACKAGEWRITER

#include <memory>
#include <string>
#include "Common/NMR_Types.h"

namespace NMR {
	class COpcPackageRelationship;
	class COpcPackagePart;
	using POpcPackagePart = std::shared_ptr<COpcPackagePart>;

	class IOpcPackageWriter {
		virtual POpcPackagePart addPart(_In_ std::string sPath) = 0;
	};
}
#endif // !NMR_IOPCPACKAGEWRITER
