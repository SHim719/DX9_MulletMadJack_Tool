#pragma once

#include "Client_Defines.h"
#include "Base.h"


BEGIN(Engine)
class CTransform;
class CGameInstance;
END


class CToolCamera 
{
	DECLARE_SINGLETON(CToolCamera)

private:
	CToolCamera() {} 
	~CToolCamera() {}

public:
	HRESULT Initialize_Prototype();
	HRESULT Initialize(void* pArg);
										
	void PriorityTick(_float fTimeDelta);
	void Tick(_float fTimeDelta);
	void LateTick(_float fTimeDelta);
										
	HRESULT Render();

private:
	LPDIRECT3DDEVICE9 m_pGraphic_Device = { nullptr };
	CGameInstance* m_pGameInstance = { nullptr };


private:
	_float4x4 m_ViewMatrix;
	_float4x4 m_ProjMatrix;
	
public:
	void Update_ViewMatrix() { m_ViewMatrix = m_pTransformCom->Get_InverseWorldMatrix(); }
	void Set_Transform_View() { m_pGraphic_Device->SetTransform(D3DTS_VIEW, &m_ViewMatrix);  }
	void Set_Transform_Proj() { m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix); }

public:
	const _float4x4& Get_CameraViewMatrix() const { return m_ViewMatrix; }
	const _float4x4& Get_CameraProjMatrix() const { return m_ProjMatrix; }

private:
	CTransform*		m_pTransformCom = { nullptr };

public:
	CTransform*		Get_Transform() { return m_pTransformCom; }

private:
	POINT m_tPrevMousePos = {};
	POINT m_tCurrentMousePos = { -1, -1 }; 

private:
	void Rotation_By_Mouse();
	void Mouse_Ray();
public:
	void Free();
};

