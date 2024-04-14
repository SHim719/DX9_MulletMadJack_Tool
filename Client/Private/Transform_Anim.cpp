#include "Transform_Anim.h"
#include "Transform.h"

CTransform_Anim::CTransform_Anim(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent { pGraphic_Device }
{
}

CTransform_Anim::CTransform_Anim(const CTransform_Anim& rhs)
	: CComponent { rhs }
{
}

HRESULT CTransform_Anim::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTransform_Anim::Initialize(void* pArg)
{
	m_pTransform = (CTransform*)pArg;
	if (nullptr == m_pTransform)
		return E_FAIL;

	Safe_AddRef(m_pTransform);

	return S_OK;
}

void CTransform_Anim::Update(_float fTimeDelta)
{
	if (nullptr == m_pNowPlayingAnim)
		return;
	if (m_iNowFrame == m_pNowPlayingAnim->back().iKeyFrame + 1)
	{
		if (m_bLoop)
			m_iNowFrame = 0;
		else
			m_pNowPlayingAnim = nullptr;
	}



	m_iNowFrame++;
}

HRESULT CTransform_Anim::Insert_Animations(const wstring& strAnimKey, const vector<KEYFRAME_DESC>& animations)
{
	auto& it = m_Animations.find(strAnimKey);
	if (m_Animations.end() != it)
		return E_FAIL;

	m_Animations.insert({ strAnimKey, animations });

	return S_OK;
}

HRESULT CTransform_Anim::Play(const wstring& strAnimKey, _bool bLoop)
{
	auto& it = m_Animations.find(strAnimKey);
	if (m_Animations.end() != it)
		return E_FAIL;

	m_bLoop = bLoop;

	return S_OK;
}

CTransform_Anim* CTransform_Anim::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTransform_Anim* pInstance = new CTransform_Anim(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CTransform_Anim"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CTransform_Anim::Clone(void* pArg)
{
	CTransform_Anim* pInstance = new CTransform_Anim(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CTransform_Anim"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTransform_Anim::Free()
{
	__super::Free();

	Safe_Release(m_pTransform);
}
