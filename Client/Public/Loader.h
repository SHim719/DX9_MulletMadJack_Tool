#pragma once

#include "Client_Defines.h"
#include "Base.h"

/* 2. �ε� ���� ������ �ʿ��� �ڿ��� �����Ѵ�.  */
/* ���� ������ ���� �ڿ� �ε� : ��Ƽ������ */

BEGIN(Client)

class CLoader final : public CBase
{
private:
	CLoader(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLoader() = default;

public:
	HRESULT Initialize(LEVEL eNextLevelID);
	_uint Loading();
	_bool isFinished() const {
		return m_isFinished;
	}

	void Show_LoadingText() {
		SetWindowText(g_hWnd, m_szLoadingText);
	}

private:
	LPDIRECT3DDEVICE9	m_pGraphic_Device = { nullptr };
	HANDLE				m_hThread = {};
	CRITICAL_SECTION	m_CriticalSection = {};
	LEVEL				m_eNextLevelID = { LEVEL_END };

	_tchar				m_szLoadingText[MAX_PATH] = TEXT("");
	_float				m_fLoadingPercent = { 0.0f };
	_bool				m_isFinished = { false };

private:
	HRESULT Loading_For_Logo_Level();
	HRESULT Loading_For_GamePlay_Level();
	HRESULT Loading_For_Tool_Level();

public:
	static CLoader* Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevelID);
	virtual void Free() override;
};

END