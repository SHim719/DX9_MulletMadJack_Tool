#include "SpawnTrigger.h"

#include "GameInstance.h"

CSpawnTrigger::CSpawnTrigger(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CTriggerObject { pGraphic_Device }
{
}

CSpawnTrigger::CSpawnTrigger(const CSpawnTrigger& rhs)
	: CTriggerObject { rhs }
{
}

HRESULT CSpawnTrigger::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSpawnTrigger::Initialize(void* pArg)
{
	if (FAILED(Add_Components()))
		return E_FAIL;
	
	return S_OK;
}


HRESULT CSpawnTrigger::Add_Components()
{
	m_pTransform = dynamic_cast<CTransform*>(Add_Component(LEVEL_STATIC, L"Transform_Default", L"Transfom"));
	m_pBoxCollider = dynamic_cast<CBoxCollider*>(Add_Component(LEVEL_STATIC, L"Box_Collider_Default", L"Collider"));

	return S_OK;
}

void CSpawnTrigger::TriggerEvent()
{
	
}

CSpawnTrigger* CSpawnTrigger::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSpawnTrigger* pInstance = new CSpawnTrigger(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CSpawnTrigger"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSpawnTrigger::Clone(void* pArg)
{
	CSpawnTrigger* pInstance = new CSpawnTrigger(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CSpawnTrigger"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSpawnTrigger::Free()
{
	__super::Free();
}
