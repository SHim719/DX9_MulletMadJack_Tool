#pragma once

#include "Client_Defines.h"
#include "Component.h"


BEGIN(Engine)
class CGameObject;
class CGameInstance;
END

BEGIN(Client)

class CTerrain_Collider : public CComponent
{
protected:
	CTerrain_Collider(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTerrain_Collider(const CTerrain_Collider& rhs);
	virtual ~CTerrain_Collider() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	void Update();

private:
	CGameInstance* m_pGameInstance = { nullptr };
	CGameObject* m_pOwner = { nullptr };

	_float m_fWidth = { 0.f };
	_float m_fHeight = { 0.f };

private:
	void Check_Look();
	void Check_Up();
	void Check_Right();

public:
	void Set_Width(_float fWidth) { m_fWidth = fWidth; }
	void Set_Height(_float fHeight) { m_fHeight = fHeight; }


public:
	static CTerrain_Collider* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END

