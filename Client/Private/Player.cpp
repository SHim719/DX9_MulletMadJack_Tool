#include "..\Public\Player.h"

#include "GameInstance.h"

#include "ToolCamera.h"
#include "BoxCollider.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CPlayer::CPlayer(const CPlayer & rhs)
	: CGameObject{ rhs }
{
}

HRESULT CPlayer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayer::Initialize(void* pArg)
{
	m_pCamera = CToolCamera::Get_Instance();

	m_pTransform = m_pCamera->Get_Transform();
	Safe_AddRef(m_pTransform);

	m_pBoxCollider = dynamic_cast<CBoxCollider*>(Add_Component(LEVEL_STATIC, L"Box_Collider_Default", L"Collider"));

	if (nullptr == m_pBoxCollider)
		return E_FAIL;

	return S_OK;
}

void CPlayer::PriorityTick(_float fTimeDelta)
{
}

void CPlayer::Tick(_float fTimeDelta)
{
	m_pBoxCollider->Update_BoxCollider(m_pTransform->Get_WorldMatrix());
}

void CPlayer::LateTick(_float fTimeDelta)
{
}

HRESULT CPlayer::Render()
{

	return S_OK;
}

CPlayer * CPlayer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPlayer*	pInstance = new CPlayer(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CPlayer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

// CGameObject*	p사본 = 원형->Clone();

CGameObject * CPlayer::Clone(void* pArg)
{
	CPlayer*	pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created : CPlayer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();

	Safe_Release(m_pBoxCollider);
	Safe_Release(m_pTransform);
}
