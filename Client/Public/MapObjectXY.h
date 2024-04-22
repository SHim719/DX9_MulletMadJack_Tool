#pragma once


#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CVIBuffer_RectXY;
class CTexture;
class CBoxCollider;
END

BEGIN(Client)

class CMapObjectXY : public CGameObject
{
private:
	CMapObjectXY(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMapObjectXY(const CMapObjectXY& rhs);
	virtual ~CMapObjectXY() = default;

public:
	HRESULT Initialize_Prototype();
	HRESULT Initialize(void* pArg)			override;
	void LateTick(_float fTimeDelta)		override;
	HRESULT Render()						override;

private:
	CVIBuffer_RectXY* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };

private:
	HRESULT Add_Components();

public:
	static CMapObjectXY* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject* Clone(void* pArg) override;
	void Free() override;
};

END