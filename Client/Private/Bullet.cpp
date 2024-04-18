#include "Bullet.h"

#include "GameInstance.h"


CBullet::CBullet(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CBullet::CBullet(const CBullet& rhs)
	: CGameObject{ rhs }
{
}

HRESULT CBullet::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CBullet::Initialize(void* pArg)
{
	if (E_FAIL == Add_Components())
		return E_FAIL;

	return S_OK;
}

void CBullet::PriorityTick(_float fTimeDelta)
{
}

void CBullet::Tick(_float fTimeDelta)
{

}

void CBullet::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CBullet::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, false);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	if (FAILED(m_pTransform->Bind_WorldMatrix()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Texture(m_iTexture_Index)))
		return E_FAIL;

	m_pVIBufferCom->Render();

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	return S_OK;
}

HRESULT CBullet::Add_Components()
{
	m_pTransform = dynamic_cast<CTransform*>(Add_Component(LEVEL_STATIC, TEXT("Transform_Default"), TEXT("Transform"), nullptr));
	if (nullptr == m_pTransform)
		return E_FAIL;

	m_pVIBufferCom = dynamic_cast<CVIBuffer*>(Add_Component(LEVEL_STATIC, TEXT("VIBuffer_Sphere_Default"), TEXT("VIBuffer"), nullptr));
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	m_pTextureCom = dynamic_cast<CTexture*>(Add_Component(LEVEL_STATIC, TEXT("Bullet_Textures"), TEXT("Bullet_Textures"), nullptr));
	if (nullptr == m_pTextureCom)
		return E_FAIL;
	return S_OK;
}

CBullet* CBullet::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CBullet* pInstance = new CBullet(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CBullet"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBullet::Clone(void* pArg)
{
	CBullet* pInstance = new CBullet(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CBullet"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBullet::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
