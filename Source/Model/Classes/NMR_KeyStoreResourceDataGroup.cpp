#include "Model/Classes/NMR_KeyStoreResourceDataGroup.h"
#include "Common/NMR_Exception.h"
#include <memory>

namespace NMR {
	CKeyStoreResourceDataGroup::CKeyStoreResourceDataGroup(PUUID const & keyUUID)
	{
		m_sKeyUUID = keyUUID;
	}
	CKeyStoreResourceDataGroup::CKeyStoreResourceDataGroup(PUUID const & keyUUID, std::vector<PKeyStoreAccessRight> const & ar, std::vector<PKeyStoreResourceData> const & rd)
	{
		m_sKeyUUID = keyUUID;
		m_accessRights = ar;
		m_resourcesData = rd;
	}

}
