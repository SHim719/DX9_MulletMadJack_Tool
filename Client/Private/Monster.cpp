#include "Monster.h"

#include "GameInstance.h"


CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CMonster::CMonster(const CMonster& rhs)
	: CGameObject{ rhs }
{
}

HRESULT CMonster::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CMonster::Initialize(void* pArg)
{
	if (E_FAIL == Add_Components())
		return E_FAIL;

	if (nullptr != pArg)
		m_iTexture_Index = *(_uint*)pArg;

	return S_OK;
}

void CMonster::PriorityTick(_float fTimeDelta)
{
}

void CMonster::Tick(_float fTimeDelta)
{

}

void CMonster::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CMonster::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, false);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	if (FAILED(m_pTransform->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture(m_iTexture_Index)))
		return E_FAIL;

	m_pVIBufferCom->Render();


	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

HRESULT CMonster::Add_Components()
{
	m_pTransform = dynamic_cast<CTransform*>(Add_Component(LEVEL_STATIC, TEXT("Transform_Default"), TEXT("Transform"), nullptr));
	if (nullptr == m_pTransform)
		return E_FAIL;

	m_pVIBufferCom = dynamic_cast<CVIBuffer*>(Add_Component(LEVEL_STATIC, TEXT("VIBuffer_Rect_Default"), TEXT("VIBuffer"), nullptr));
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	m_pTextureCom = dynamic_cast<CTexture*>(Add_Component(LEVEL_STATIC, TEXT("Monster_Textures"), TEXT("Monster_Textures"), nullptr));
	if (nullptr == m_pTextureCom)
		return E_FAIL;
	return S_OK;
}



CMonster* CMonster::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMonster* pInstance = new CMonster(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CMonster"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMonster::Clone(void* pArg)
{
	CMonster* pInstance = new CMonster(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CMonster"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonster::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
