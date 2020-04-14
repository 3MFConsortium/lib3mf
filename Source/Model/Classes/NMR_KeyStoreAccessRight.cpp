#include "Model/Classes/NMR_KeyStoreAccessRight.h"
#include "Common/NMR_Exception.h"
namespace NMR {
	CKeyStoreAccessRight::CKeyStoreAccessRight(PKeyStoreConsumer const & consumer, KEKPARAMS const & params)
		:m_pConsumer(consumer), m_sKekParams(params)
	{
		if (nullptr == m_pConsumer)
			throw CNMRException(NMR_ERROR_INVALIDPARAM);

		if (params.m_eAlgorithm != eKeyStoreWrapAlgorithm::RSA_OAEP) {
			throw CNMRException(NMR_ERROR_KEYSTOREUNSUPPORTEDALGORITHM);
		}

		if (params.m_eMask != eKeyStoreMaskGenerationFunction::MGF1_SHA1
			|| params.m_eMask != eKeyStoreMaskGenerationFunction::MGF1_SHA256) {
			throw CNMRException(NMR_ERROR_KEYSTOREUNSUPPORTEDALGORITHM);
		}

		if (params.m_eDigest != eKeyStoreMessageDigest::SHA1
			|| params.m_eDigest != eKeyStoreMessageDigest::SHA256)
			throw CNMRException(NMR_ERROR_KEYSTOREUNSUPPORTEDALGORITHM);
	}

	KEKPARAMS CKeyStoreAccessRight::getKEKParams()
	{
		return m_sKekParams;
	}

	PKeyStoreConsumer CKeyStoreAccessRight::getConsumer()
	{
		return m_pConsumer;
	}
}