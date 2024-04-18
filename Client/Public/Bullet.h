#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
class CVIBuffer;
class CTexture;
class CBoxCollider;
END

BEGIN(Client)
class CBullet : public CGameObject
{
private:
	CBullet(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBullet(const CBullet& rhs);
	virtual ~CBullet() = default;

public:
	HRESULT Initialize_Prototype();
	HRESULT Initialize(void* pArg)			override;
	void PriorityTick(_float fTimeDelta)	override;
	void Tick(_float fTimeDelta)			override;
	void LateTick(_float fTimeDelta)		override;
	HRESULT Render()						override;

private:
	CVIBuffer* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };

private:
	HRESULT Add_Components();

public:
	static CBullet* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject* Clone(void* pArg) override;
	void Free() override;
};

END