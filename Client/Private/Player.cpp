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
	m_pRigidbody = dynamic_cast<CRigidbody*>(Add_Component(LEVEL_STATIC, L"Rigidbody_Default", L"Rigidbody", m_pTransform));


	if (nullptr == m_pBoxCollider)
		return E_FAIL;

	return S_OK;
}

void CPlayer::PriorityTick(_float fTimeDelta)
{
}

void CPlayer::Tick(_float fTimeDelta)
{
	if (m_pGameInstance->GetKeyDown(eKeyCode::Space))
	{
		bFlag = !bFlag;
	}
	//m_pBoxCollider->Update_BoxCollider(m_pTransform->Get_WorldMatrix());

	if (m_pGameInstance->GetKeyDown(eKeyCode::NUMPAD0))
	{
		m_pTransform->Set_State(CTransform::STATE_POSITION, &_float3(0.f, 10.f, -1.f));
	}

	if (!bFlag)
		return;

	if (!m_bSlope)
	{
		if (m_pGameInstance->GetKey(eKeyCode::W))
		{
			_float3 vLook = m_pTransform->Get_State(CTransform::STATE_LOOK);
			vLook.y = 0.f;
			D3DXVec3Normalize(&vLook, &vLook);
			vLook *= 4.f;
			m_pRigidbody->Set_GroundVelocity(vLook);
		}
		else if (m_pGameInstance->GetKey(eKeyCode::S))
		{
			_float3 vLook = m_pTransform->Get_State(CTransform::STATE_LOOK);
			vLook.y = 0.f;
			D3DXVec3Normalize(&vLook, &vLook);
			vLook *= 4.f;
			m_pRigidbody->Set_GroundVelocity(-vLook);
		}

		if (m_pGameInstance->GetKey(eKeyCode::D))
		{
			_float3 vRight = m_pTransform->Get_State(CTransform::STATE_RIGHT);
			vRight.y = 0.f;
			D3DXVec3Normalize(&vRight, &vRight);
			vRight *= 4.f;
			m_pRigidbody->Add_GroundVelocity(vRight);
		}
		else if (m_pGameInstance->GetKey(eKeyCode::A))
		{
			_float3 vLeft = m_pTransform->Get_State(CTransform::STATE_RIGHT);
			vLeft.y = 0.f;
			D3DXVec3Normalize(&vLeft, &vLeft);
			vLeft *= 4.f;
			m_pRigidbody->Add_GroundVelocity(-vLeft);
		}

		if (m_pGameInstance->GetKeyDown(eKeyCode::G))
		{
			m_pRigidbody->Set_VelocityY(4.f);
			m_pRigidbody->Set_Ground(false);
		}

		m_pRigidbody->Update(fTimeDelta);
	}
	else
	{
		if (m_pGameInstance->GetKey(eKeyCode::W))
		{
			_float3 vLook = m_pSlopeTransform->Get_State(CTransform::STATE_LOOK);
			D3DXVec3Normalize(&vLook, &vLook); 
			vLook.x = -vLook.x;
			vLook.z = -vLook.z;
			vLook *= 6.f;
			m_pRigidbody->Set_Velocity(vLook);
		}
		//m_pRigidbody->Update(fTimeDelta);

		if (m_pGameInstance->GetKeyDown(eKeyCode::G))
		{
			m_pRigidbody->Set_VelocityY(4.f);
			m_pRigidbody->Set_Ground(false);
		}

		if (m_pGameInstance->GetKey(eKeyCode::D))
		{
			_float3 vRight = m_pSlopeTransform->Get_State(CTransform::STATE_RIGHT);
			D3DXVec3Normalize(&vRight, &vRight);
			vRight *= 5.f;
			m_pRigidbody->Add_GroundVelocity(vRight);
		}
		else if (m_pGameInstance->GetKey(eKeyCode::A))
		{
			_float3 vRight = m_pSlopeTransform->Get_State(CTransform::STATE_RIGHT);
			D3DXVec3Normalize(&vRight, &vRight);
			vRight *= 5.f;
			m_pRigidbody->Add_GroundVelocity(-vRight);
		}

		m_pRigidbody->Update(fTimeDelta);
	}
	
	
}

void CPlayer::LateTick(_float fTimeDelta)
{
	if (!bFlag)
		return;
	SetPos_Look();
	SetPos_Right();
	SetPos_Up();
}

HRESULT CPlayer::Render()
{

	return S_OK;
}

void CPlayer::SetPos_Look()
{
	RAY_DESC rayDesc;
	rayDesc.iLevel = LEVEL_TOOL;
	rayDesc.strDstLayer = L"Wall";
	rayDesc.vRayDir = m_pTransform->Get_State(CTransform::STATE_LOOK);
	rayDesc.vRayWorldPos = m_pTransform->Get_State(CTransform::STATE_POSITION);

	//rayDesc.vRayDir.y = 0.f;
	D3DXVec3Normalize(&rayDesc.vRayDir, &rayDesc.vRayDir);

	CGameObject* pHitObj = nullptr;
	_float3 fHitWorldPos;
	_float fDist = 0.f;
	if (m_pGameInstance->Ray_Cast(rayDesc, pHitObj, fHitWorldPos, fDist))
	{
		if (fDist <= 0.5f)
		{
			_float3 vPos = fHitWorldPos;
			vPos -= rayDesc.vRayDir * 0.5f;
			m_pTransform->Set_State(CTransform::STATE_POSITION, &vPos);
			return;
		}
	}

	rayDesc.vRayDir = -rayDesc.vRayDir;
	if (m_pGameInstance->Ray_Cast(rayDesc, pHitObj, fHitWorldPos, fDist))
	{
		if (fDist <= 0.5f)
		{
			_float3 vPos = fHitWorldPos;
			vPos -= rayDesc.vRayDir * 0.5f;
			m_pTransform->Set_State(CTransform::STATE_POSITION, &vPos);
		}
	}
}

void CPlayer::SetPos_Up()
{
	RAY_DESC rayDesc;
	rayDesc.iLevel = LEVEL_TOOL;
	rayDesc.strDstLayer = L"Floor";
	rayDesc.vRayDir = { 0.f, -1.f, 0.f };
	rayDesc.vRayWorldPos = m_pTransform->Get_State(CTransform::STATE_POSITION);

	CGameObject* pHitObj = nullptr;
	_float3 fHitWorldPos;
	_float fDist = 0.f;

	if (m_pGameInstance->Ray_Cast(rayDesc, pHitObj, fHitWorldPos, fDist) && fDist <= 0.75f)
	{
		_float3 vPos = fHitWorldPos;
		vPos.y += 0.75f;
		m_pTransform->Set_State(CTransform::STATE_POSITION, &vPos);
		m_pRigidbody->Set_OnGround();
		return;
	}
	else
	{
		m_pRigidbody->Set_Ground(false);
	}
	

	rayDesc.strDstLayer = L"Slope";
	if (m_pGameInstance->Ray_Cast(rayDesc, pHitObj, fHitWorldPos, fDist) && fDist <= 0.75f)
	{
		_float3 vPos = fHitWorldPos;
		vPos.y += 0.75f;
		m_pTransform->Set_State(CTransform::STATE_POSITION, &vPos);

		m_pSlopeTransform = pHitObj->Get_Transform();

		m_pRigidbody->Set_OnGround();
		_float3 vSlopeLook = m_pSlopeTransform->Get_State(CTransform::STATE_LOOK);
		D3DXVec3Normalize(&vSlopeLook, &vSlopeLook);
		vSlopeLook.x = -vSlopeLook.x;
		vSlopeLook.z = -vSlopeLook.z;
		m_pRigidbody->Set_Velocity(vSlopeLook);
		m_bSlope = true;
		return;
	}
	else
	{
		m_bSlope = false;
	}
	
	rayDesc.vRayDir = { 0.f, 1.f, 0.f };
	rayDesc.vRayWorldPos = m_pTransform->Get_State(CTransform::STATE_POSITION);
	rayDesc.strDstLayer = L"Floor";
	if (m_pGameInstance->Ray_Cast(rayDesc, pHitObj, fHitWorldPos, fDist))
	{
		if (fDist <= 0.75f)
		{
			_float3 vPos = fHitWorldPos;
			vPos.y -= 0.75f;
			m_pTransform->Set_State(CTransform::STATE_POSITION, &vPos);
		}
	}
}

void CPlayer::SetPos_Right()
{
	RAY_DESC rayDesc;
	rayDesc.iLevel = LEVEL_TOOL;
	rayDesc.strDstLayer = L"Wall";
	rayDesc.vRayDir = m_pTransform->Get_State(CTransform::STATE_RIGHT);
	rayDesc.vRayWorldPos = m_pTransform->Get_State(CTransform::STATE_POSITION);

	rayDesc.vRayDir.y = 0.f;
	rayDesc.vRayDir = *D3DXVec3Normalize(&rayDesc.vRayDir, &rayDesc.vRayDir);


	CGameObject* pHitObj = nullptr;
	_float3 fHitWorldPos;
	_float fDist = 0.f;
	if (m_pGameInstance->Ray_Cast(rayDesc, pHitObj, fHitWorldPos, fDist))
	{
		if (fDist <= 0.5f)
		{
			_float3 vPos = fHitWorldPos;
			vPos -= rayDesc.vRayDir * 0.5f;
			m_pTransform->Set_State(CTransform::STATE_POSITION, &vPos);
			return;
		}
	}

	rayDesc.vRayDir = -rayDesc.vRayDir;
	if (m_pGameInstance->Ray_Cast(rayDesc, pHitObj, fHitWorldPos, fDist))
	{
		if (fDist <= 0.5f)
		{
 			_float3 vPos = fHitWorldPos;
			vPos -= rayDesc.vRayDir * 0.5f;
			m_pTransform->Set_State(CTransform::STATE_POSITION, &vPos);
		}
	}
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
	Safe_Release(m_pRigidbody);
}
