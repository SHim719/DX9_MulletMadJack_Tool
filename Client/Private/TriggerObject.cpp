#include "TriggerObject.h"

#include "GameInstance.h"

CTriggerObject::CTriggerObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject { pGraphic_Device }
{
}

CTriggerObject::CTriggerObject(const CTriggerObject& rhs)
	: CGameObject { rhs }
	, m_pBoxCollider { rhs.m_pBoxCollider }
{
	Safe_AddRef(m_pBoxCollider);
}

HRESULT CTriggerObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTriggerObject::Initialize(void* pArg)
{
	m_pTransform = dynamic_cast<CTransform*>(Add_Component(LEVEL_STATIC, L"Transform_Default", L"Transfom"));
	m_pBoxCollider = dynamic_cast<CBoxCollider*>(Add_Component(LEVEL_STATIC, L"Box_Collider_Default", L"Collider"));

	return S_OK;
}

void CTriggerObject::Tick(_float fTimeDelta)
{
	m_pBoxCollider->Update_BoxCollider(m_pTransform->Get_WorldMatrix());
}

void CTriggerObject::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CTriggerObject::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);
	if (m_pBoxCollider)
		m_pBoxCollider->Render();

	return S_OK;
}

CTriggerObject* CTriggerObject::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTriggerObject* pInstance = new CTriggerObject(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CTriggerObject"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTriggerObject::Clone(void* pArg)
{
	CTriggerObject* pInstance = new CTriggerObject(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CTriggerObject"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTriggerObject::Free()
{
	__super::Free();

	Safe_Release(m_pBoxCollider);
}
