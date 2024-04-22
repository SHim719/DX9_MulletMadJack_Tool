#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameInstance;
class CGameObject;
END


BEGIN(Client)

enum eObjectType
{
	MONSTER,
	EVENT_TRIGGER,
	WALL,
	FLOOR,
	OBJECT,
	DOOR,
	SODAMACHINE,
	SODAMACHINE_BANNER,
	BORDER,
	SPAWN_TRIGGER,
	SLOPE,
	BOX_OBJECT,
	VENTILADOR,
	OBJTYPE_END,
};

enum eMonsterType
{
	WHITE_SUIT,
	CHAINSAW,
	DRONE,
	MONSTER_END,
};

enum eTriggerType
{
	TRIGGER_END,
};

class CImGui_Manager final : public CBase
{
	DECLARE_SINGLETON(CImGui_Manager)

public:

private:
	CImGui_Manager();
	~CImGui_Manager() = default;

public:
	HRESULT Initialize(LPDIRECT3DDEVICE9 pGraphic_Device);
	void	Tick();
	void	Render();
	
private:
	LPDIRECT3DDEVICE9	m_pGraphic_Device = { nullptr };
	CGameInstance*		m_pGameInstance = { nullptr };

private:
	_float3 m_vPosition = _float3(0.f, 0.f, 0.f);
	_float3	m_vRotation = _float3(0.f, 0.f, 0.f);
	_float3 m_vScale = _float3(1.f, 1.f, 1.f);

	eObjectType		m_eScene_Select_Object = { OBJTYPE_END };
	set<_int>		m_SelectObjIndices;
	vector<_byte>	m_vecSelected[OBJTYPE_END];

	vector<class CGameObject*> m_vecObjects[OBJTYPE_END];
private:
	int			m_iTextureIdx = { 0 };
	int			m_iTexture_Count[OBJTYPE_END] = {};

	eObjectType		m_eTexture_Select_Object = { OBJTYPE_END };
	int				m_iTexture_Select_Index = { -1 };

private:
	string	m_szObjName[OBJTYPE_END]
		= { "Monster%d", "Trigger%d", "Wall%d", "Floor%d", "Object%d", "Door%d", "Machine%d", "Machine_Banner%d", "Border%d"
	, "SpawnTrigger%d", "Slope%d", "Box%d", "Ventilador%d"};

	wstring m_szPrototypeTags[OBJTYPE_END] = { L"Prototype_Monster", L"Prototype_Trigger"
		, L"Prototype_Wall", L"Prototype_Floor", L"Prototype_MapObject"
		, L"Prototype_Door", L"Prototype_SodaMachine"
		, L"Prototype_SodaMachine_Banner",
	L"Prototype_Border", L"Prototype_SpawnTrigger", L"Prototype_Slope",
	L"Prototype_BoxObject", L"Prototype_Ventilador"};

	wstring m_szLayerTags[OBJTYPE_END] = { L"Monster", L"Trigger", L"Wall", L"Floor", L"MapObject", L"Door",
		L"SodaMachine", L"SodaMachine_Banner", L"MapObject", L"SpawnTrigger", L"Slope",
	L"BoxObject", L"Ventilador"};

#pragma region TriggerWindow
	void SpawnTriggerWindow();

	_int m_iMaxIdx = 0;
	_int m_iMinIdx = 0;

#pragma endregion 
#pragma region SodaMachineWindow
	void SodaMachineWindow();

	_float3 m_vPourPos = { 0.f, 0.f, 0.f };
#pragma endregion
#pragma region DoorWindow
	void DoorWindow();

	bool m_bDoorRight = false;
#pragma endregion 
#pragma region ColliderWindow
	void ColliderWindow();

	_float3 m_vOffset = { 0.f, 0.f, 0.f };
	_float3 m_vColliderScale = { 1.f, 1.f, 1.f };
#pragma endregion
#pragma region MenuBar
private:
	void MenuBar();
#pragma endregion
#pragma region Object Bar
	void ObjectBar();

	void Transform_View();
	void Transform_Pos();
	void Transform_Rot();
	void Transform_Scale();

	void Texture_View();
	void Texture_ListBox(eObjectType eType);
	void Render_TexIndex(eObjectType eType);

	void Tree_SceneObjects();
	void Tree_View(eObjectType eType);

	void ComboBox_Object();

	void Key_Input();
#pragma endregion
#pragma region Interact_Gizmo
private:
	typedef struct tagGizmoDesc
	{
		ImGuizmo::MODE CurrentGizmoMode = ImGuizmo::LOCAL;
		ImGuizmo::OPERATION CurrentGizmoOperation = ImGuizmo::TRANSLATE;
		bool bUseSnap = false;
		float snap[3] = { 0.5f, 0.5f, 0.5f };
		float bounds[6] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
		float boundsSnap[3] = { 0.1f, 0.1f, 0.1f };
		bool boundSizing = false;
		bool boundSizingSnap = false;
	}GIZMODESC;

private:
	GIZMODESC m_tGizmoDesc;
private:
	void Transform_Gizmo();

#pragma endregion


public:
	void Free() override;

private:
	void QuatToPitchYawRoll(const D3DXQUATERNION& q, FLOAT& pitch, FLOAT& yaw, FLOAT& roll, BOOL bToDegree)
	{
		FLOAT sqw = q.w * q.w;
		FLOAT sqx = q.x * q.x;
		FLOAT sqy = q.y * q.y;
		FLOAT sqz = q.z * q.z;

		// rotation about x-axis
		pitch = asinf(2.0f * (q.w * q.x - q.y * q.z));
		// rotation about y-axis
		yaw = atan2f(2.0f * (q.x * q.z + q.w * q.y), (-sqx - sqy + sqz + sqw));
		// rotation about z-axis
		roll = atan2f(2.0f * (q.x * q.y + q.w * q.z), (-sqx + sqy - sqz + sqw));

		if (bToDegree)
		{
			pitch = D3DXToDegree(pitch);
			yaw = D3DXToDegree(yaw);
			roll = D3DXToDegree(roll);
		}

	}
};

END
