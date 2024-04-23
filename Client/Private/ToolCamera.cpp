#include "ToolCamera.h"
#include "GameInstance.h"
#include "Key_Manager.h"

IMPLEMENT_SINGLETON(CToolCamera)


HRESULT CToolCamera::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CToolCamera::Initialize(void* pArg)
{
	m_pGraphic_Device = (LPDIRECT3DDEVICE9)pArg;
	Safe_AddRef(m_pGraphic_Device);

	m_pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(m_pGameInstance);

	CTransform::TRANSFORM_DESC	TransformDesc{};

	TransformDesc.fSpeedPerSec = 3.0f;

	m_pTransformCom = dynamic_cast<CTransform*>(m_pGameInstance->Clone_Component(LEVEL_STATIC, TEXT("Transform_Default"), &TransformDesc));
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, &_float3 { 0.f, 0.0f, -1.5f });

	D3DXMatrixPerspectiveFovLH(&m_ProjMatrix, D3DXToRadian(90.0f), (_float)g_iWinSizeX / g_iWinSizeY, 0.1f, 300.f);
	return S_OK;
}

void CToolCamera::PriorityTick(_float fTimeDelta)
{
}

void CToolCamera::Tick(_float fTimeDelta)
{
	m_tPrevMousePos = m_tCurrentMousePos;
	GetCursorPos(&m_tCurrentMousePos);
	ScreenToClient(g_hWnd, &m_tCurrentMousePos);

	if (m_pGameInstance->GetKeyDown(eKeyCode::Space))
	{
		m_bCamMode = !m_bCamMode;
	}

	if (m_bCamMode)
	{
		if (m_pGameInstance->GetKeyDown(eKeyCode::LShift))
		{
			m_pTransformCom->Set_Speed(20.f);
		}

		else if (m_pGameInstance->GetKeyNone(eKeyCode::LShift))
		{
			m_pTransformCom->Set_Speed(3.f);
		}

		if (m_pGameInstance->GetKey(eKeyCode::RButton))
		{
			if (m_pGameInstance->GetKey(eKeyCode::W))
			{
				m_pTransformCom->Go_Straight(fTimeDelta * 2.f);
			}

			if (m_pGameInstance->GetKey(eKeyCode::S))
			{
				m_pTransformCom->Go_Backward(fTimeDelta * 2.f);
			}

			if (m_pGameInstance->GetKey(eKeyCode::A))
			{
				m_pTransformCom->Go_Left(fTimeDelta * 2.f);
			}

			if (m_pGameInstance->GetKey(eKeyCode::D))
			{
				m_pTransformCom->Go_Right(fTimeDelta * 2.f);
			}

			if (m_pGameInstance->GetKey(eKeyCode::E))
			{
				m_pTransformCom->Go_Up(fTimeDelta * 2.f);
			}

			if (m_pGameInstance->GetKey(eKeyCode::Q))
			{
				m_pTransformCom->Go_Down(fTimeDelta * 2.f);
			}
			Rotation_By_Mouse();
		}
	}
	else
	{
		Rotation_By_Mouse();
	}

	if (m_pGameInstance->GetKeyDown(eKeyCode::LButton))
	{
		Mouse_Ray();
	}
}

void CToolCamera::LateTick(_float fTimeDelta)
{
}

HRESULT CToolCamera::Render()
{
	return S_OK;
}

void CToolCamera::Rotation_By_Mouse()
{
	m_pTransformCom->Add_RollInput(To_Radian((m_tCurrentMousePos.y - m_tPrevMousePos.y) * 0.2f));
	m_pTransformCom->Add_YAxisInput(To_Radian((m_tCurrentMousePos.x - m_tPrevMousePos.x) * 0.2f));
}

void CToolCamera::Mouse_Ray()
{
	_float3 fMouseNDC_Near = _float3(m_tCurrentMousePos.x * 2.0f / g_iWinSizeX - 1, -m_tCurrentMousePos.y * 2.0f / g_iWinSizeY + 1, 0.f);
	_float3 fMouseNDC_Far = _float3(m_tCurrentMousePos.x * 2.0f / g_iWinSizeX - 1, -m_tCurrentMousePos.y * 2.0f / g_iWinSizeY + 1, 1.f);
	
	_float4x4 inverseProjView;
	D3DXMatrixInverse(&inverseProjView, nullptr, &(m_ViewMatrix * m_ProjMatrix));
	
	_float3 fMouseWorld_Near = *D3DXVec3TransformCoord(&fMouseWorld_Near, &fMouseNDC_Near, &inverseProjView);
	_float3 fMouseWorld_Far = *D3DXVec3TransformCoord(&fMouseWorld_Far, &fMouseNDC_Far, &inverseProjView);

	_float3 vRayDir = *D3DXVec3Normalize(&vRayDir, &(fMouseWorld_Far - fMouseWorld_Near));

	RAY_DESC rayDesc{};
	rayDesc.iLevel = LEVEL_TOOL;
	rayDesc.strDstLayer = L"Wall";
	rayDesc.vRayDir = vRayDir;
	rayDesc.vRayWorldPos = fMouseWorld_Near;

	m_pGameInstance->Add_RayDesc(rayDesc);
}

void CToolCamera::Free()
{
	Safe_Release(m_pGraphic_Device);
	Safe_Release(m_pGameInstance);
	
	Safe_Release(m_pTransformCom);
	
}
