#pragma once

#include "Renderer.h"
#include "Component_Manager.h"
#include "Key_Manager.h"

/* Ŭ���̾�Ʈ�� ������ �߰����� ��Ȱ�� �����Ѵ�. */
/* Ŭ���̾�Ʈ���� ������ ����� �������� �Ѵٶ�� ������ �����ν��Ͻ��� ���ø��� �ǵ���. */

BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)
private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public:
	HRESULT Initialize_Engine(_uint iNumLevels, const GRAPHIC_DESC& GraphicDesc, _Out_ LPDIRECT3DDEVICE9* ppOut);
	void Tick_Engine(_float fTimeDelta);
	HRESULT Draw();
	void Clear(_uint iLevelIndex);
	void Clear_Objects(_uint iLevelIndex);

#pragma region GRAPHIC_DEVICE
public: /* For.Graphic_Device */
	void Render_Begin();
	void Render_End(HWND hWnd = 0);
#pragma endregion

#pragma region LEVEL_MANAGER
public: /* For.Level_Manager */
	HRESULT Change_Level(class CLevel* pNewLevel);
#pragma endregion

#pragma region TIMER_MANAGER
public:
	_float	Get_TimeDelta(const wstring& strTimerTag);
	HRESULT Add_Timer(const wstring& strTimerTag);
	void Compute_TimeDelta(const wstring& strTimerTag);
#pragma endregion

#pragma region OBJECT_MANAGER
public:
	HRESULT Add_Prototype(const wstring& strPrototypeTag, class CGameObject* pPrototype);
	class CGameObject* Add_Clone(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag, void* pArg = nullptr);
	class CLayer* Find_Layer(_uint iLevelIndex, const wstring& strLayerTag);
#pragma endregion

#pragma region COMPONENT_MANAGER
public:
	HRESULT Add_Prototype(_uint iLevelIndex, const wstring& strPrototypeTag, class CComponent* pPrototype);
	class CComponent* Clone_Component(_uint iLevelIndex, const wstring& strPrototypeTag, void* pArg = nullptr);
#pragma endregion

#pragma region COLLISION_MANAGER
	void Add_RayDesc(const RAY_DESC& RayDesc);
	_bool Ray_Cast(const RAY_DESC& RayDesc, OUT class CGameObject*& pOutHit, OUT _float3& fHitWorldPos, OUT _float& fDist);
#pragma endregion

#pragma region RENDERER
public:
	HRESULT Add_RenderObjects(CRenderer::RENDERGROUP eRenderGroup, class CGameObject* pRenderObject);
#pragma endregion

#pragma region KEY_MANAGER
	bool GetKeyDown(eKeyCode _keyCode) { return	 m_pKey_Manager->GetKeyDown(_keyCode); }
	bool GetKeyUp(eKeyCode _keyCode)	{ return m_pKey_Manager->GetKeyUp(_keyCode); }
	bool GetKey(eKeyCode _keyCode)	{ return m_pKey_Manager->GetKey(_keyCode); }
	bool GetKeyNone(eKeyCode _keyCode) { return m_pKey_Manager->GetKeyNone(_keyCode); }
#pragma endregion

private:
	class CGraphic_Device*			m_pGraphic_Device = { nullptr };
	class CLevel_Manager*			m_pLevel_Manager = { nullptr };
	class CTimer_Manager*			m_pTimer_Manager = { nullptr };
	class CObject_Manager*			m_pObject_Manager = { nullptr };
	class CComponent_Manager*		m_pComponent_Manager = { nullptr };
	class CRenderer*				m_pRenderer = { nullptr };
	class CKey_Manager*				m_pKey_Manager = { nullptr };
	class CCollision_Manager*		m_pCollision_Manager = { nullptr };

public:
	static void Release_Engine();
	virtual void Free() override;
	
};

END