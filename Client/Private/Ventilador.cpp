#include "Ventilador.h"

#include "GameInstance.h"


CVentilador::CVentilador(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CVentilador::CVentilador(const CVentilador& rhs)
	: CGameObject{ rhs }
{
}

HRESULT CVentilador::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CVentilador::Initialize(void* pArg)
{
	if (E_FAIL == Add_Components())
		return E_FAIL;

	if (nullptr != pArg)
		m_iTexture_Index = *(_uint*)pArg;

	m_pTransform->Set_RotationSpeed(To_Radian(480.f));
	return S_OK;
}

void CVentilador::PriorityTick(_float fTimeDelta)
{
}

void CVentilador::Tick(_float fTimeDelta)
{
	m_pBoxCollider->Update_BoxCollider(m_pTransform->Get_WorldMatrix());

	//m_pTransform->Turn(m_pTransform->Get_State(CTransform::STATE_LOOK), fTimeDelta);
}

void CVentilador::LateTick(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND, this);
}

HRESULT CVentilador::Render()
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

	m_pBoxCollider->Render();

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	return S_OK;
}

HRESULT CVentilador::Add_Components()
{
	m_pTransform = dynamic_cast<CTransform*>(Add_Component(LEVEL_STATIC, TEXT("Transform_Default"), TEXT("Transform"), nullptr));
	if (nullptr == m_pTransform)
		return E_FAIL;

	m_pVIBufferCom = dynamic_cast<CVIBuffer_Rect*>(Add_Component(LEVEL_STATIC, TEXT("VIBuffer_Rect_Default"), TEXT("VIBuffer"), nullptr));
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	m_pTextureCom = dynamic_cast<CTexture*>(Add_Component(LEVEL_STATIC, TEXT("Ventilador_Textures"), TEXT("Ventilador_Textures"), nullptr));
	if (nullptr == m_pTextureCom)
		return E_FAIL;

	CBoxCollider::BOXCOLLISION_DESC desc;
	desc.vScale = { 1.f, 1.f, 1.f };
	desc.vOffset = { 0.f, 0.f, 0.f };

	m_pBoxCollider = dynamic_cast<CBoxCollider*>(Add_Component(LEVEL_STATIC, TEXT("Box_Collider_Default"), TEXT("Collider"), &desc));
	if (nullptr == m_pBoxCollider)
		return E_FAIL;


	return S_OK;
}


CVentilador* CVentilador::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVentilador* pInstance = new CVentilador(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CVentilador"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVentilador::Clone(void* pArg)
{
	CVentilador* pInstance = new CVentilador(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CVentilador"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVentilador::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pBoxCollider);
}
