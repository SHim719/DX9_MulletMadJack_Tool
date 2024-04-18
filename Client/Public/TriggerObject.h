#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CBoxCollider;
END

BEGIN(Client)

class CTriggerObject : public CGameObject
{
protected:
	CTriggerObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTriggerObject(const CTriggerObject& rhs);
	virtual ~CTriggerObject() = default;
	
public:
	HRESULT Initialize_Prototype()	override;
	HRESULT Initialize(void* pArg)	override;
	void Tick(_float fTimeDelta)	override;
	void LateTick(_float fTimeDelta) override;
	HRESULT Render()				override;

protected:
	CBoxCollider* m_pBoxCollider = { nullptr };

protected:
	virtual void TriggerEvent() {} ; // ��ӹ޾Ƽ� �̺�Ʈ ó�����ٰ�.

public:
	static CTriggerObject* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg);
	void Free() override;
};

END