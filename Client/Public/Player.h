#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)

class CPlayer final : public CGameObject
{
private:
	CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual void PriorityTick(_float fTimeDelta) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;


private:
	void SetPos_Look();
	void SetPos_Up();
	void SetPos_Right();

private:
	_bool m_bGround = { false };
	_bool m_bSlope = { false };
	CTransform* m_pSlopeTransform = { nullptr };

	bool bFlag = false;
private:
	class CToolCamera*	m_pCamera = { nullptr };
	class CBoxCollider* m_pBoxCollider = { nullptr };
	CRigidbody* m_pRigidbody = { nullptr };

public:
	/* 원형을 생성하기위한 함수. */
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	/* 사본을 생성한다. */
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END