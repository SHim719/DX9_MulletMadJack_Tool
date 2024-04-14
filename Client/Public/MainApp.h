#pragma once


#include "Client_Defines.h"
#include "Base.h"

/* 1.������ ��ü���� �帧�� �����Ѵ�. */
/* 2.DirectX�� ���� �������� �������� �ϱ����� �غ����(����̽��� �ʱ�ȭ)�� �����Ѵ�.*/

//class CŬ�����̸�
//{
//private: or protected:
//	������;
//	�Ҹ���;
//
//public:
//	����;
//public: 
//	�Լ�;
//protected:
//	����;
//protected:
//	�Լ�;
//private:
//	����;
//private:
//	�Լ�;
//
//public:
//	����(Create)������ �Լ�();
//	�Ҹ�(Free)���õ� �Լ�();
//};

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)

class CMainApp final : public CBase
{
private:
	CMainApp();
	virtual ~CMainApp() { }

public:
	HRESULT Initialize();
	
	void Tick(_float fTimeDelta);
	HRESULT Render();

private:
	CGameInstance*				m_pGameInstance = { nullptr };
	LPDIRECT3DDEVICE9			m_pGraphic_Device = { nullptr };

private:
	HRESULT Open_Level(LEVEL eStartLevelID);
	HRESULT Ready_Prototype_GameObjects();
	HRESULT Ready_Prototype_Components();
	HRESULT Insert_Default_GameObjects();

public:
	static CMainApp* Create();
	virtual void Free() override;
};

END

