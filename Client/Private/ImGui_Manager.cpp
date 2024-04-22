#include "ImGui_Manager.h"
#include "GameInstance.h"
#include "Wall.h"
#include "Transform.h"
#include "ToolCamera.h"
#include "CMap_Manager.h"
#include "BoxCollider.h"
#include "VIBuffer.h"
#include "Door.h"
#include "SodaMachine.h"
#include "SpawnTrigger.h"

IMPLEMENT_SINGLETON(CImGui_Manager)

CImGui_Manager::CImGui_Manager()
{
	
}


HRESULT CImGui_Manager::Initialize(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	m_pGraphic_Device = pGraphic_Device;
	Safe_AddRef(pGraphic_Device);
    m_pGameInstance = CGameInstance::Get_Instance();
    Safe_AddRef(m_pGameInstance);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.DisplaySize = ImVec2(float(g_iWinSizeX), float(g_iWinSizeY));
    ImGui::StyleColorsDark();
    
    // Setup Platform/Renderer backends
    if (!ImGui_ImplWin32_Init(g_hWnd)) {
        MSG_BOX(L"Failed : ImGui_ImplWin32_Init(g_hWnd)");
        assert(false);
    }

    if (!ImGui_ImplDX9_Init(pGraphic_Device)) {
        MSG_BOX(L"Failed : ImGui_ImplDX9_Init(m_pGraphic_Device)");
        assert(false);
    }

    m_iTexture_Count[WALL] = 23;
    m_iTexture_Count[FLOOR] = 6;
    m_iTexture_Count[OBJECT] = 30;
    m_iTexture_Count[DOOR] = 5;
    m_iTexture_Count[MONSTER] = 3;
    //m_iTexture_Count[EVENT_TRIGGER] = 2;
    m_iTexture_Count[SLOPE] = 1;
   
	return S_OK;
}

void CImGui_Manager::Tick()
{
    ImGui_ImplDX9_NewFrame(); // GUI 프레임 시작
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Map", (bool*)0, ImGuiWindowFlags_MenuBar);
    MenuBar();
    ObjectBar();
    Key_Input();
    ImGui::End();

    ColliderWindow();
    DoorWindow(); 
    SodaMachineWindow();
    SpawnTriggerWindow();

    Transform_Gizmo();
}

void CImGui_Manager::Render()
{
    m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, FALSE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    m_pGraphic_Device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
  
    m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
}

void CImGui_Manager::SpawnTriggerWindow()
{
    if (SPAWN_TRIGGER != m_eScene_Select_Object || 1 != m_SelectObjIndices.size())
        return;

    CSpawnTrigger* pTrigger = dynamic_cast<CSpawnTrigger*>(m_vecObjects[SPAWN_TRIGGER][*m_SelectObjIndices.begin()]);
    if (nullptr == pTrigger)
        return;

    m_iMinIdx = pTrigger->Get_MinIdx();
    m_iMaxIdx = pTrigger->Get_MaxIdx();

    if (ImGui::Begin("SpawnTrigger", (bool*)0))
    {
        ImGui::SetCursorPos(ImVec2(15.5, 39.5));
        ImGui::Text("Enemy Index Min");

        ImGui::SetCursorPos(ImVec2(15.5, 74.5));
        ImGui::Text("Enemy Index Max");

        ImGui::SetCursorPos(ImVec2(139.f, 35.f));
        ImGui::PushItemWidth(200);

        if (ImGui::InputInt("##MinIdx", &m_iMinIdx))
        {
            pTrigger->Set_MinIdx(m_iMinIdx);
        }
        ImGui::PopItemWidth();

        ImGui::SetCursorPos(ImVec2(140, 69.875));
        ImGui::PushItemWidth(200);
        if (ImGui::InputInt("##MaxIdx", &m_iMaxIdx))
        {
            pTrigger->Set_MaxIdx(m_iMaxIdx);
        }
        ImGui::PopItemWidth();

    }
    ImGui::End();

}

void CImGui_Manager::SodaMachineWindow()
{
    if (SODAMACHINE != m_eScene_Select_Object || 0 == m_SelectObjIndices.size())
        return;

    CSodaMachine* pSodaMachine = dynamic_cast<CSodaMachine*>(m_vecObjects[m_eScene_Select_Object][*m_SelectObjIndices.begin()]);
    if (nullptr == pSodaMachine)
        return;

    m_vPourPos = pSodaMachine->Get_PourPos();

    if (ImGui::Begin("SodaMachine", (bool*)0))
    {
        ImGui::SetCursorPos(ImVec2(84, 44));
        ImGui::PushItemWidth(200);

        if (ImGui::InputFloat3("##", m_vPourPos))
        {
            pSodaMachine->Set_PourPos(m_vPourPos);
        }

        ImGui::PopItemWidth();

        ImGui::SetCursorPos(ImVec2(17, 46.5));
        ImGui::Text("Pour Pos");

    }
    ImGui::End();
}

void CImGui_Manager::DoorWindow()
{
    if (DOOR != m_eScene_Select_Object || 0 == m_SelectObjIndices.size())
        return;

    CDoor* pDoor = dynamic_cast<CDoor*>(m_vecObjects[m_eScene_Select_Object][*m_SelectObjIndices.begin()]);
    if (nullptr == pDoor)
        return;

    m_bDoorRight = pDoor->isDirection_Right();

    if (ImGui::Begin("Door", (bool*)0))
    {
        ImGui::SetCursorPos(ImVec2(13.5, 35.5));
        ImGui::Text("Door: ");

        ImGui::SetCursorPos(ImVec2(63.5, 33.5));

        if (ImGui::Checkbox(" Direction Right?", &m_bDoorRight))
        {
            if (true == m_bDoorRight)
                pDoor->Set_Direction(CDoor::DIRECTION::RIGHT);
            else
                pDoor->Set_Direction(CDoor::DIRECTION::LEFT);
        }

    }
    ImGui::End();

}

void CImGui_Manager::ColliderWindow()
{
    if (ImGui::Begin("Collider", (bool*)0))
    {
        ImGui::SetCursorPos(ImVec2(16.5, 37.5));
        ImGui::Text("Offset");

        ImGui::SetCursorPos(ImVec2(13, 67));
        ImGui::PushItemWidth(200);
        if (ImGui::InputFloat3("##Offset", &m_vOffset.x))
        {
            for (_int iIndex : m_SelectObjIndices)
            {
                CGameObject* pObj = m_vecObjects[(_int)m_eScene_Select_Object][iIndex];
                CBoxCollider* pBoxCollider = dynamic_cast<CBoxCollider*>(pObj->Find_Component(L"Collider"));
                pBoxCollider->Set_Offset(m_vOffset);
                pBoxCollider->Update_BoxCollider(pObj->Get_Transform()->Get_WorldMatrix());
            }
        }
        ImGui::PopItemWidth();

       
        ImGui::SetCursorPos(ImVec2(16.5, 109.5));
        ImGui::Text("Scale");

        ImGui::SetCursorPos(ImVec2(14, 143));
        ImGui::PushItemWidth(200);
        if (ImGui::InputFloat3("##ColliderScale", &m_vColliderScale.x))
        {
            for (_int iIndex : m_SelectObjIndices)
            {
                CGameObject* pObj = m_vecObjects[(_int)m_eScene_Select_Object][iIndex];
                CBoxCollider* pBoxCollider = dynamic_cast<CBoxCollider*>(pObj->Find_Component(L"Collider"));
                pBoxCollider->Set_Scale(m_vColliderScale);
                pBoxCollider->Update_BoxCollider(pObj->Get_Transform()->Get_WorldMatrix());
            }
        }
        ImGui::PopItemWidth();
    }
    ImGui::End();
}

void CImGui_Manager::MenuBar()
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open Map"))
            {
                CMap_Manager::Load_Map(m_vecObjects);
                for (int i = 0; i < OBJTYPE_END; ++i)
                {
                    m_vecSelected[i].clear();
                    m_vecSelected[i].resize(m_vecObjects[i].size(), 0);
                    m_SelectObjIndices.clear();
                }
            }

            if (ImGui::MenuItem("Save Map"))
            {
                CMap_Manager::Save_Map(m_vecObjects);
            }

            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

   
}


void CImGui_Manager::ObjectBar()
{
    if (ImGui::BeginTabBar("Object"))
    {
        if (ImGui::BeginTabItem("Object"))
        {
            Transform_View();
            Texture_View();

            ImGui::SeparatorText("Objects");
            ComboBox_Object();
            Tree_SceneObjects();

            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }
}

void CImGui_Manager::Transform_View()
{
    ImGui::SeparatorText("Transform");
    if (m_eScene_Select_Object >= OBJTYPE_END || m_SelectObjIndices.size() <= 0)
        return;

    Transform_Pos();
    Transform_Rot();
    Transform_Scale();
}

void CImGui_Manager::Transform_Pos()
{
    ImGui::Text("Position");
    ImGui::SameLine();
    ImGui::PushItemWidth(60.f);

    bool bFlag = false;
    bFlag = ImGui::InputFloat("##PositionX", &m_vPosition.x);
    if (bFlag)
    {
        for (_int iIndex : m_SelectObjIndices)
        {
            CGameObject* pObj = m_vecObjects[(_int)m_eScene_Select_Object][iIndex];
            pObj->Get_Transform()->Set_PosX(m_vPosition.x);
        }
    }
    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 1.f);

    bFlag = ImGui::InputFloat("##PositionY", &m_vPosition.y);
    if (bFlag)
    {
        for (_int iIndex : m_SelectObjIndices)
        {
            CGameObject* pObj = m_vecObjects[(_int)m_eScene_Select_Object][iIndex];
            pObj->Get_Transform()->Set_PosY(m_vPosition.y);
        }
        bFlag = false;
    }
    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 1.f);

    bFlag = ImGui::InputFloat("##PositionZ", &m_vPosition.z);

    if (bFlag)
    {
        for (_int iIndex : m_SelectObjIndices)
        {
            CGameObject* pObj = m_vecObjects[(_int)m_eScene_Select_Object][iIndex];
            pObj->Get_Transform()->Set_PosZ(m_vPosition.z);
        }
        bFlag = false;
    }
    ImGui::PopItemWidth();
}

void CImGui_Manager::Transform_Rot()
{
    ImGui::Text("Rotation");
    ImGui::SameLine();
    ImGui::PushItemWidth(60.f);

    bool bFlag = false;
    bFlag = ImGui::InputFloat("##RotationX", &m_vRotation.x);
    if (bFlag)
    {
        for (_int iIndex : m_SelectObjIndices)
        {
            CGameObject* pObj = m_vecObjects[(_int)m_eScene_Select_Object][iIndex];
            CTransform* pTransform = pObj->Get_Transform();

            _float3 vScale, vRot, vPos;
            _quat quat;
            D3DXMatrixDecompose(&vScale, &quat, &vPos, &pTransform->Get_WorldMatrix());
            QuatToPitchYawRoll(quat, vRot.x, vRot.y, vRot.z, false);

            D3DXQuaternionRotationYawPitchRoll(&quat, vRot.y, To_Radian(m_vRotation.x), vRot.z);
            pObj->Get_Transform()->Rotation_Quaternion(quat);
        }
    }

    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 1.f);

    bFlag = ImGui::InputFloat("##RotationY", &m_vRotation.y);
    if (bFlag)
    {
        for (_int iIndex : m_SelectObjIndices)
        {
            CGameObject* pObj = m_vecObjects[(_int)m_eScene_Select_Object][iIndex];
            CTransform* pTransform = pObj->Get_Transform();

            _float3 vScale, vRot, vPos;
            _quat quat;
            D3DXMatrixDecompose(&vScale, &quat, &vPos, &pTransform->Get_WorldMatrix());
            QuatToPitchYawRoll(quat, vRot.x, vRot.y, vRot.z, false);

            D3DXQuaternionRotationAxis(&quat, &_float3(0.f, 1.f, 0.f), To_Radian(m_vRotation.y));
            //D3DXQuaternionRotationYawPitchRoll(&quat, To_Radian(m_vRotation.y), vRot.x, vRot.z);
            pObj->Get_Transform()->Rotation_Quaternion(quat);
        }
    }

    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 1.f);

    bFlag = ImGui::InputFloat("##RotationZ", &m_vRotation.z);
    if (bFlag)
    {
        for (_int iIndex : m_SelectObjIndices)
        {
            CGameObject* pObj = m_vecObjects[(_int)m_eScene_Select_Object][iIndex];
            CTransform* pTransform = pObj->Get_Transform();

            _float3 vScale, vRot, vPos;
            _quat quat;
            D3DXMatrixDecompose(&vScale, &quat, &vPos, &pTransform->Get_WorldMatrix());
            QuatToPitchYawRoll(quat, vRot.x, vRot.y, vRot.z, false);

            D3DXQuaternionRotationYawPitchRoll(&quat, vRot.y, vRot.x, To_Radian(m_vRotation.z));
            pObj->Get_Transform()->Rotation_Quaternion(quat);
        }
    }

    ImGui::PopItemWidth();
}

void CImGui_Manager::Transform_Scale()
{
    ImGui::Text("Scale");
    ImGui::SameLine();
    ImGui::PushItemWidth(60.f);

    bool bFlag = false;
    bFlag = ImGui::InputFloat("##ScaleX", &m_vScale.x);
    if (bFlag)
    {
        if (0.f == m_vScale.x)
            return;
        for (_int iIndex : m_SelectObjIndices)
        {
            CGameObject* pObj = m_vecObjects[(_int)m_eScene_Select_Object][iIndex];
            pObj->Get_Transform()->Set_ScaleX(m_vScale.x);  

            CVIBuffer* pVIBuffer = dynamic_cast<CVIBuffer*>(pObj->Find_Component(L"VIBuffer"));
            pVIBuffer->Scaling_Texcoord(m_vScale);
        }
    }
    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 1.f);

    bFlag = ImGui::InputFloat("##ScaleY", &m_vScale.y);
    if (bFlag)
    {
        if (0.f == m_vScale.y)
            return;
        for (_int iIndex : m_SelectObjIndices)
        {
            CGameObject* pObj = m_vecObjects[(_int)m_eScene_Select_Object][iIndex];
            pObj->Get_Transform()->Set_ScaleY(m_vScale.y);

            CVIBuffer* pVIBuffer = dynamic_cast<CVIBuffer*>(pObj->Find_Component(L"VIBuffer"));
            pVIBuffer->Scaling_Texcoord(m_vScale);
        }
        bFlag = false;
    }
    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 1.f);

    bFlag = ImGui::InputFloat("##ScaleZ", &m_vScale.z);

    if (bFlag)
    {
        if (0.f == m_vScale.z)
            return;
        for (_int iIndex : m_SelectObjIndices)
        {
            CGameObject* pObj = m_vecObjects[(_int)m_eScene_Select_Object][iIndex];
            pObj->Get_Transform()->Set_ScaleZ(m_vScale.z);
        }
        bFlag = false;
    }
    ImGui::PopItemWidth();
}

void CImGui_Manager::Texture_View()
{
    ImGui::SeparatorText("Texture");

    if (m_eScene_Select_Object >= OBJTYPE_END || m_SelectObjIndices.size() <= 0)
        return;
  
    float fBeginCursorY = ImGui::GetCursorPosY();
    if (ImGui::BeginListBox("##Texture ListBox", { 70.f, 100.f }))
    {
        Texture_ListBox(m_eScene_Select_Object);
        ImGui::EndListBox();
    }
    float fEndCursorY = ImGui::GetCursorPosY();

    ImGui::SetCursorPos({ 100.f, fBeginCursorY });
    Render_TexIndex(m_eScene_Select_Object);
    ImGui::SetCursorPos({ 100.f, fBeginCursorY + 80.f });

    ImGui::Checkbox("Snap", &m_tGizmoDesc.bUseSnap);

    ImGui::SetCursorPosY(fEndCursorY);
}

void CImGui_Manager::Texture_ListBox(eObjectType eType)
{
    for (int i = 0; i < m_iTexture_Count[eType]; i++)
    {
        char buf[32];
        sprintf_s(buf, m_szObjName[eType].c_str(), (int)i);
        if (ImGui::Selectable(buf, m_iTexture_Select_Index == (int)i))
        {
            for (_int iIndex : m_SelectObjIndices)
                m_vecObjects[eType][iIndex]->Set_Texture_Index((_uint)i);
            break;
        }
    }

}

void CImGui_Manager::Render_TexIndex(eObjectType eType)
{
    _int iTexIndex = _int(m_vecObjects[eType][*m_SelectObjIndices.begin()]->Get_Texture_Index());
    for (_int iIndex : m_SelectObjIndices)
    {
        if (iTexIndex != _int(m_vecObjects[eType][iIndex]->Get_Texture_Index()))
        {
            iTexIndex = -1;
            break;
        }
    }
    ImGui::Text("Index: %d", iTexIndex);
}

void CImGui_Manager::Tree_SceneObjects()
{
    if (ImGui::TreeNode("Monster"))
    {
        Tree_View(MONSTER);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Trigger"))
    {
        Tree_View(EVENT_TRIGGER);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Wall"))
    {
        Tree_View(WALL);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Floor"))
    {
        Tree_View(FLOOR);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Object"))
    {
        Tree_View(OBJECT);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Door"))
    {
        Tree_View(DOOR);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("SodaMachine"))
    {
        Tree_View(SODAMACHINE);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("SodaMachine_Banner"))
    {
        Tree_View(SODAMACHINE_BANNER);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Border"))
    {
        Tree_View(BORDER);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Spawn_Trigger"))
    {
        Tree_View(SPAWN_TRIGGER);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Slope"))
    {
        Tree_View(SLOPE);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Box"))
    {
        Tree_View(BOX_OBJECT);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Ventilador"))
    {
        Tree_View(VENTILADOR);
        ImGui::TreePop();
    }

}

void CImGui_Manager::Tree_View(eObjectType eType)
{
    for (size_t i = 0; i < m_vecObjects[eType].size(); i++)
    {
        char buf[256];
        sprintf_s(buf, m_szObjName[eType].c_str(), (int)i);
        if (ImGui::Selectable(buf, m_vecSelected[eType][i]))
        {
            if (false == m_pGameInstance->GetKey(eKeyCode::LCtrl))
            {
                fill(m_vecSelected[eType].begin(), m_vecSelected[eType].end(), 0);
                m_SelectObjIndices.clear();
            }

            if (eType != m_eScene_Select_Object)
            {
                if (OBJTYPE_END != m_eScene_Select_Object)
                    fill(m_vecSelected[m_eScene_Select_Object].begin(), m_vecSelected[m_eScene_Select_Object].end(), 0);
                m_eScene_Select_Object = eType;
                m_SelectObjIndices.clear();
            }

            if (m_vecSelected[eType][i])
            {
                auto it = m_SelectObjIndices.find((_int)i);
                m_SelectObjIndices.erase(it);
            }
            else
            {
                m_SelectObjIndices.insert((_int)i);
            }

            m_vecSelected[eType][i] ^= 1;

            CTransform* pTransform = m_vecObjects[eType][i]->Get_Transform();
            D3DXQUATERNION quat;
            D3DXMatrixDecompose(&m_vScale, &quat, &m_vPosition, &pTransform->Get_WorldMatrix());
            QuatToPitchYawRoll(quat, m_vRotation.x, m_vRotation.y, m_vRotation.z, true);

            for (_uint iIndex : m_SelectObjIndices)
            {
                _float3 vPosition, vRotation, vScale;
                pTransform = m_vecObjects[eType][iIndex]->Get_Transform();

                D3DXMatrixDecompose(&vScale, &quat, &vPosition, &pTransform->Get_WorldMatrix());
                QuatToPitchYawRoll(quat, vRotation.x, vRotation.y, vRotation.z, true);

                if (m_vPosition.x != vPosition.x)
                    m_vPosition.x = 899898.f;
                if (m_vPosition.y != vPosition.y)
                    m_vPosition.y = 899898.f;
                if (m_vPosition.z != vPosition.z)
                    m_vPosition.z = 899898.f;
                if (m_vRotation.x != vRotation.x)
                    m_vRotation.x = 899898.f;
                if (m_vRotation.y != vRotation.y)
                    m_vRotation.y = 899898.f;
                if (m_vRotation.z != vRotation.z)
                    m_vRotation.z = 899898.f;
                if (m_vScale.x != m_vScale.x)
                    m_vScale.x = 899898.f;
                if (m_vScale.y != m_vScale.y)
                    m_vScale.y = 899898.f;
                if (m_vScale.z != m_vScale.z)
                    m_vScale.z = 899898.f;
            }

        }
    }
}

void CImGui_Manager::ComboBox_Object()
{
    static int iItemIdx = 0;
    const char* combo_value[OBJTYPE_END] = { "Monster", "Trigger",
        "Wall", "Floor", "Object", "Door", "SodaMachine", "SodaMachine_Banner", "Border", "Spawn_Trigger", "Slope"
    , "Box", "Ventilador"};
     
    ImGui::Combo("##Object", &iItemIdx, combo_value, IM_ARRAYSIZE(combo_value));

    ImGui::SameLine();
    if (ImGui::Button("Create", ImVec2(50, 0)))
    {
        m_vecObjects[(eObjectType)iItemIdx].push_back(m_pGameInstance->Add_Clone(LEVEL_TOOL, m_szLayerTags[(eObjectType)iItemIdx]
            , m_szPrototypeTags[(eObjectType)iItemIdx]));
        m_vecSelected[(eObjectType)iItemIdx].push_back(false);
    }
}

void CImGui_Manager::Key_Input()
{
    if (m_pGameInstance->GetKey(eKeyCode::LCtrl) && m_pGameInstance->GetKeyDown(eKeyCode::D))
    {
        if (0 == m_SelectObjIndices.size())
            return;

        CGameObject* pFromCopyObj = m_vecObjects[m_eScene_Select_Object][*m_SelectObjIndices.begin()];

        CGameObject* pToCopyObj = m_pGameInstance->Add_Clone(LEVEL_TOOL, m_szLayerTags[m_eScene_Select_Object]
            , m_szPrototypeTags[m_eScene_Select_Object]);
        m_vecObjects[m_eScene_Select_Object].push_back(pToCopyObj);
        m_vecSelected[m_eScene_Select_Object].push_back(false);

       pToCopyObj->Get_Transform()->Set_WorldMatrix(pFromCopyObj->Get_Transform()->Get_WorldMatrix());
       pToCopyObj->Set_Texture_Index(pFromCopyObj->Get_Texture_Index());

        CBoxCollider* pFromCopyBoxCollider = dynamic_cast<CBoxCollider*>(pFromCopyObj->Find_Component(L"Collider"));
        CBoxCollider* pToCopyBoxCollider = dynamic_cast<CBoxCollider*>(pToCopyObj->Find_Component(L"Collider"));
        if (pFromCopyBoxCollider && pToCopyBoxCollider)
        {
            pToCopyBoxCollider->Set_Offset(pFromCopyBoxCollider->Get_Offset());
            pToCopyBoxCollider->Set_Scale(pFromCopyBoxCollider->Get_Scale());
        }
        
        CVIBuffer* pVIBuffer = dynamic_cast<CVIBuffer*>(pToCopyObj->Find_Component(L"VIBuffer"));
        pVIBuffer->Scaling_Texcoord(pFromCopyObj->Get_Transform()->Get_Scale());
    }

    if (m_pGameInstance->GetKeyDown(eKeyCode::Delete))
    {
        if (m_SelectObjIndices.size() == 1)
        {
            auto pObj = m_vecObjects[m_eScene_Select_Object][*m_SelectObjIndices.begin()];
            auto iter = find_if(m_vecObjects[m_eScene_Select_Object].begin(), m_vecObjects[m_eScene_Select_Object].end(), 
                [&](CGameObject* _pObj)
                {
                    return pObj == _pObj;
                });

            if (m_vecObjects[m_eScene_Select_Object].end() != iter)
            {
                m_SelectObjIndices.clear();
                (*iter)->Set_Destroy(true);
                m_vecObjects[m_eScene_Select_Object].erase(iter);
            }
        }
    }
}

void CImGui_Manager::Transform_Gizmo()
{
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGuizmo::BeginFrame();
    ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

    _float4x4 identityMatrix = *D3DXMatrixIdentity(&identityMatrix);
    _float4x4 viewMatrix = CToolCamera::Get_Instance()->Get_CameraViewMatrix();
    _float4x4 projMatrix = CToolCamera::Get_Instance()->Get_CameraProjMatrix();

    if (m_SelectObjIndices.size() != 1)
        return;
#pragma region InputKey
    if (false == m_pGameInstance->GetKey(eKeyCode::RButton))
    {
        if (m_pGameInstance->GetKeyDown(eKeyCode::W))
        {
            m_tGizmoDesc.CurrentGizmoOperation = ImGuizmo::TRANSLATE;
            m_tGizmoDesc.snap[0] = 0.5f;
        }
        if (m_pGameInstance->GetKeyDown(eKeyCode::E))
        {
            m_tGizmoDesc.CurrentGizmoOperation = ImGuizmo::ROTATE;
            m_tGizmoDesc.snap[0] = 15.f;
        }
        if (m_pGameInstance->GetKeyDown(eKeyCode::R))
        {
            m_tGizmoDesc.CurrentGizmoOperation = ImGuizmo::SCALE;
            m_tGizmoDesc.snap[0] = 0.5f;
        }
    }
#pragma endregion 
    _float4x4 worldMatrix = m_vecObjects[m_eScene_Select_Object][*m_SelectObjIndices.begin()]->Get_Transform()->Get_WorldMatrix();

    if (ImGuizmo::Manipulate(*viewMatrix.m, *projMatrix.m
        , m_tGizmoDesc.CurrentGizmoOperation
        , m_tGizmoDesc.CurrentGizmoMode
        , *worldMatrix.m, NULL, m_tGizmoDesc.bUseSnap ? &m_tGizmoDesc.snap[0] : NULL, m_tGizmoDesc.boundSizing ? m_tGizmoDesc.bounds : NULL
        , m_tGizmoDesc.boundSizingSnap ? m_tGizmoDesc.boundsSnap : NULL))
    {
        D3DXQUATERNION quat;
        D3DXMatrixDecompose(&m_vScale, &quat, &m_vPosition, &worldMatrix);
        QuatToPitchYawRoll(quat, m_vRotation.x, m_vRotation.y, m_vRotation.z, true);
    }

    m_vecObjects[m_eScene_Select_Object][*m_SelectObjIndices.begin()]->Get_Transform()->Set_WorldMatrix(worldMatrix);
}

void CImGui_Manager::Free()
{
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    Safe_Release(m_pGraphic_Device);
    Safe_Release(m_pGameInstance);
}
