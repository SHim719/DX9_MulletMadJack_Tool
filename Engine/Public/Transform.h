#pragma once

#include "Component.h"

/* 1. 월드 상태에서의 모든 상태변환을 제어한다. */
/* 2. 월드 변환 행렬을 보관한다. */
/* 3. 월드 변환이 필요한 모든 객체들이 보유하는 컴포너늩다. */

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	typedef struct
	{
		_float		fSpeedPerSec;
		_float		fRotationPerSec;
	}TRANSFORM_DESC;

	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };

private:
	CTransform(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTransform(const CTransform& rhs);
	virtual ~CTransform() = default;

public:
	_float3 Get_State(STATE eState) {
		return *(_float3*)&m_WorldMatrix.m[eState][0];
	}

	_float3 Get_Scale()
	{
		return _float3(D3DXVec3Length(&Get_State(STATE_RIGHT)),
			D3DXVec3Length(&Get_State(STATE_UP)),
			D3DXVec3Length(&Get_State(STATE_LOOK)));
	}
	
	_float3 Get_Rotation()
	{
		return m_vRotation;
	}

	void Set_State(STATE eState, const _float3* pState) {
		memcpy(&m_WorldMatrix.m[eState][0], pState, sizeof(_float3));
	}

	void Multiply_Scale(const _float3& vScale);

	void Set_Scale(const _float3& vScale);

	void Set_ScaleX(const _float fX);
	void Set_ScaleY(const _float fY);
	void Set_ScaleZ(const _float fZ);

	void Set_PosX(const _float& fX) { m_WorldMatrix.m[STATE_POSITION][0] = fX; }
	void Set_PosY(const _float& fY) { m_WorldMatrix.m[STATE_POSITION][1] = fY; }
	void Set_PosZ(const _float& fZ) { m_WorldMatrix.m[STATE_POSITION][2] = fZ; }

	void Add_Pos(const _float3& vAdd);
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	HRESULT Bind_WorldMatrix() {
		return m_pGraphic_Device->SetTransform(D3DTS_WORLD, &m_WorldMatrix);
	}

	_float4x4 Get_InverseWorldMatrix()
	{
		return *D3DXMatrixInverse(&_float4x4{}, nullptr, &m_WorldMatrix);
	}

	const _float4x4& Get_WorldMatrix() const
	{
		return m_WorldMatrix;
	}

	void Set_WorldMatrix(const _float4x4& worldMatrix)
	{
		m_WorldMatrix = worldMatrix;
	}

public:
	void Go_Straight(_float fTimeDelta);
	void Go_Backward(_float fTimeDelta);
	void Go_Right(_float fTimeDelta);
	void Go_Left(_float fTimeDelta);
	void Go_Up(_float fTimeDelta);
	void Go_Down(_float fTimeDelta);

	// 현재 상태기준으로 회전 속도로 회전
	void Turn(const _float3& vAxis, _float fTimeDelta);
	void Rotation(const _float3& vAxis, _float fRadian);
	void Rotation_XYZ(const _float3& vXYZ);
	void Rotation_Quaternion(const _quat& vQuat);

	void LookAt(const _float3& vWorldPoint);
	void LookAt_ForLandObject(const _float3& vWorldPoint);

	void Add_YAxisInput(_float fRadian);
	void Add_RollInput(_float fRadian);

	void Set_RotationSpeed(_float _fRadian) { m_fRotationPerSec = _fRadian; }
	void Set_Speed(_float fSpeed) { m_fSpeedPerSec = fSpeed; }
private:
	_float4x4			m_WorldMatrix;
	_float3				m_vRotation = _float3(0.f, 0.f, 0.f);

private:
	_float				m_fSpeedPerSec = { 3.0f };
	_float				m_fRotationPerSec = { 0.0f };
public:
	static CTransform* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END