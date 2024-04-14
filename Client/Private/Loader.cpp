#include "..\Public\Loader.h"

CLoader::CLoader(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGraphic_Device{ pGraphic_Device }
{
	Safe_AddRef(m_pGraphic_Device);
}

// typedef unsigned(__stdcall* _beginthreadex_proc_type)(void*);
unsigned int APIENTRY Loading_Main(void* pArg)
{
	CLoader*		pLoader = (CLoader*)pArg;	

	return pLoader->Loading();	
}


HRESULT CLoader::Initialize(LEVEL eNextLevelID)
{
	m_eNextLevelID = eNextLevelID;

	InitializeCriticalSection(&m_CriticalSection);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, Loading_Main, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

unsigned int CLoader::Loading()
{
	EnterCriticalSection(&m_CriticalSection);

	HRESULT		hr{};

	switch (m_eNextLevelID)
	{
	case LEVEL_LOGO:
		hr = Loading_For_Logo_Level();
		break;
	case LEVEL_GAMEPLAY:
		hr = Loading_For_GamePlay_Level();
		break;
	case LEVEL_TOOL:
		hr = Loading_For_Tool_Level();
		break;
	}

	LeaveCriticalSection(&m_CriticalSection);

	if (FAILED(hr))
		return 1;

	return 0;
}

HRESULT CLoader::Loading_For_Logo_Level()
{	
	lstrcpy(m_szLoadingText, TEXT("�ؽ��ĸ� �ε� �� �Դϴ�."));
	

	lstrcpy(m_szLoadingText, TEXT("����(��) �ε� �� �Դϴ�."));


	lstrcpy(m_szLoadingText, TEXT("������(��) �ε� �� �Դϴ�."));
	

	lstrcpy(m_szLoadingText, TEXT("���̴���(��) �ε� �� �Դϴ�."));
	

	lstrcpy(m_szLoadingText, TEXT("��ü������(��) �ε� �� �Դϴ�."));


	lstrcpy(m_szLoadingText, TEXT("�ε��� �Ϸ�Ǿ����ϴ�."));

	m_isFinished = true;
	
	return S_OK;
}

HRESULT CLoader::Loading_For_GamePlay_Level()
{
	lstrcpy(m_szLoadingText, TEXT("�ؽ��ĸ� �ε� �� �Դϴ�."));
	

	lstrcpy(m_szLoadingText, TEXT("����(��) �ε� �� �Դϴ�."));
	

	lstrcpy(m_szLoadingText, TEXT("������(��) �ε� �� �Դϴ�."));
	

	lstrcpy(m_szLoadingText, TEXT("���̴���(��) �ε� �� �Դϴ�."));

	lstrcpy(m_szLoadingText, TEXT("��ü������(��) �ε� �� �Դϴ�."));
	

	lstrcpy(m_szLoadingText, TEXT("�ε��� �Ϸ�Ǿ����ϴ�."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Tool_Level()
{
	lstrcpy(m_szLoadingText, TEXT("�ε��� �Ϸ�Ǿ����ϴ�."));

	m_isFinished = true;
	return S_OK;
}

CLoader * CLoader::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevelID)
{
	CLoader*	pInstance = new CLoader(pGraphic_Device);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX(TEXT("Failed to Created : CLoader"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);

	DeleteCriticalSection(&m_CriticalSection);

	DeleteObject(m_hThread);
	CloseHandle(m_hThread);

	Safe_Release(m_pGraphic_Device);
}
