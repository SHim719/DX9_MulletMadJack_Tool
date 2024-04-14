#include "Soda.h"

#include "GameInstance.h"

CSoda::CSoda(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CSoda::CSoda(const CSoda& rhs)
	: CGameObject{ rhs }
{
}

HRESULT CSoda::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSoda::Initialize(void* pArg)
{
	if (E_FAIL == Add_Components())
		return E_FAIL;

	return S_OK;
}

void CSoda::PriorityTick(_float fTimeDelta)
{
}

void CSoda::Tick(_float fTimeDelta)
{
	m_fLifeTime -= fTimeDelta;
	if (m_fLifeTime <= 0.f)
	{
		m_bDestroyed = true;
		return;
	}
		
	m_pBoxCollider->Update_BoxCollider(m_pTransform->Get_WorldMatrix());
	m_pRigidbody->Update(fTimeDelta);
}

void CSoda::LateTick(_float fTimeDelta)
{
	_float4x4	ViewMatrix;
		
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);

	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	_float3		vScaled = m_pTransform->Get_Scale();

	_float3		vRight = *(_float3*)&ViewMatrix.m[0][0];
	_float3		vUp = *(_float3*)&ViewMatrix.m[1][0];
	_float3		vLook = *(_float3*)&ViewMatrix.m[2][0];

	m_pTransform->Set_State(CTransform::STATE_RIGHT, &(vRight * vScaled.x));
	m_pTransform->Set_State(CTransform::STATE_UP, &(vUp * vScaled.y));
	m_pTransform->Set_State(CTransform::STATE_LOOK, &(vLook * vScaled.z));

	m_pGameInstance->Add_RenderObjects(CRenderer::RENDER_BLEND, this);
}

HRESULT CSoda::Render()
{
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);
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

HRESULT CSoda::Add_Components()
{
	m_pTransform = dynamic_cast<CTransform*>(Add_Component(LEVEL_STATIC, TEXT("Transform_Default"), TEXT("Transform"), nullptr));
	if (nullptr == m_pTransform)
		return E_FAIL;

	m_pVIBufferCom = dynamic_cast<CVIBuffer_Rect*>(Add_Component(LEVEL_STATIC, TEXT("VIBuffer_Rect_Default"), TEXT("VIBuffer"), nullptr));
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	m_pTextureCom = dynamic_cast<CTexture*>(Add_Component(LEVEL_STATIC, TEXT("SodaCan_Texture"), TEXT("SodaCan_Texture"), nullptr));
	if (nullptr == m_pTextureCom)
		return E_FAIL;

	m_pRigidbody = dynamic_cast<CRigidbody*>(Add_Component(LEVEL_STATIC, TEXT("Rigidbody_Default"), TEXT("Rigidbody"), m_pTransform));
	if (nullptr == m_pRigidbody)
		return E_FAIL;

	m_pRigidbody->Set_Gravity({ 0.f, -10.f, 0.f });
	m_pRigidbody->Set_Friction(0.5f);

	CBoxCollider::BOXCOLLISION_DESC desc;
	desc.vScale = { 0.25f, 0.25f, 0.25f };

	m_pBoxCollider = dynamic_cast<CBoxCollider*>(Add_Component(LEVEL_STATIC, TEXT("Box_Collider_Default"), TEXT("Collider"), &desc));
	if (nullptr == m_pBoxCollider)
		return E_FAIL;

	return S_OK;
}

CSoda* CSoda::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSoda* pInstance = new CSoda(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CSoda"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSoda::Clone(void* pArg)
{
	CSoda* pInstance = new CSoda(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CSoda"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSoda::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRigidbody);
	Safe_Release(m_pBoxCollider);
}
