#pragma once

#include "Client_Defines.h"
#include "Engine_Defines.h"

#include "ImGui_Manager.h"



BEGIN(Engine)
class CGameObject;
END

class CMap_Manager
{
	
public:
	static HRESULT Save_Map(vector<CGameObject*>* vecObjects);

public:
	static HRESULT Load_Map(vector<CGameObject*>* vecObjects);
	static void Load_Object(HANDLE hFile, vector<CGameObject*>* vecObjects);

private:
	static wstring strLayers[OBJTYPE_END];
	static wstring strPrototypeTags[OBJTYPE_END];
};

