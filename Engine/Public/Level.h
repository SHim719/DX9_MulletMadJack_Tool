#pragma once

#include "Base.h"

/* Ŭ���̾�Ʈ���� ���� ����ϱ����� ���� �������� �θ� �Ǵ� Ŭ�����̴�. */

BEGIN(Engine)

class ENGINE_DLL CLevel abstract : public CBase
{
protected:
	CLevel(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel() = default;

public:
	_uint Get_LevelID() const {
		return m_iLevelID;
	}

public:
	virtual HRESULT Initialize();
	virtual void Tick(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	LPDIRECT3DDEVICE9			m_pGraphic_Device = { nullptr };
	class CGameInstance*		m_pGameInstance = { nullptr };
	_uint						m_iLevelID = { 0 };

public:
	virtual void Free() override;
};

END