#include "MapObjectXY.h"

#include "GameInstance.h"

CMapObjectXY::CMapObjectXY(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CMapObjectXY::CMapObjectXY(const CMapObjectXY& rhs)
	: CGameObject{ rhs }
{
}

HRESULT CMapObjectXY::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMapObjectXY::Initialize(void* pArg)
{
	if (E_FAIL == Add_Components())
		return E_FAIL;

	if (nullptr != pArg)
		m_iTexture_Index = *(_uint*)pArg;

	return S_OK;
}


void CMapObjectXY::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND, this);
}

HRESULT CMapObjectXY::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);
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

HRESULT CMapObjectXY::Add_Components()
{
	m_pTransform = dynamic_cast<CTransform*>(Add_Component(LEVEL_STATIC, TEXT("Transform_Default"), TEXT("Transform"), nullptr));
	if (nullptr == m_pTransform)
		return E_FAIL;

	m_pVIBufferCom = dynamic_cast<CVIBuffer_RectXY*>(Add_Component(LEVEL_STATIC, TEXT("VIBuffer_RectXY_Default"), TEXT("VIBuffer"), nullptr));
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	m_pTextureCom = dynamic_cast<CTexture*>(Add_Component(LEVEL_STATIC, TEXT("Object_Textures"), TEXT("Object_Textures"), nullptr));
	if (nullptr == m_pTextureCom)
		return E_FAIL;



	return S_OK;
}

CMapObjectXY* CMapObjectXY::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMapObjectXY* pInstance = new CMapObjectXY(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CMapObjectXY"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMapObjectXY::Clone(void* pArg)
{
	CMapObjectXY* pInstance = new CMapObjectXY(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CMapObjectXY"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMapObjectXY::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
