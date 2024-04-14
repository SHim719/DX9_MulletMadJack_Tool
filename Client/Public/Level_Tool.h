#pragma once

#include "Client_Defines.h"
#include "Level.h"

class CLevel_Tool : public CLevel
{
private:
	CLevel_Tool(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_Tool() = default;

public:
	virtual HRESULT Initialize();
	virtual void Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;


public:
	static CLevel_Tool* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	void Free() override;
};

