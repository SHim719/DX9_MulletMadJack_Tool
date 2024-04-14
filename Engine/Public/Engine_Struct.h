#pragma once


namespace Engine
{
	typedef struct
	{
		HWND			hWnd;
		unsigned int	iWinSizeX;
		unsigned int	iWinSizeY;
		bool			isWindowed;
	}GRAPHIC_DESC;

	typedef struct tagLineIndices16
	{
		unsigned short		_0, _1;
	}LINEINDICES16;

	typedef struct tagLineIndices32
	{
		unsigned long		_0, _1;
	}LINEINDICES32;

	typedef struct tagFaceIndices16
	{
		unsigned short		_0, _1, _2;
	}FACEINDICES16;

	typedef struct tagFaceIndices32
	{
		unsigned long		_0, _1, _2;
	}FACEINDICES32;

	typedef struct
	{		
		D3DXVECTOR3		vPosition;
		D3DXVECTOR2		vTexcoord; /* �ؽ����� ������� ��ǥ.(0 ~ 1) */
		D3DXVECTOR3		vTexcoord1; /* �ؽ����� ������� ��ǥ.(0 ~ 1) */
	}VTXTEX;

	typedef struct
	{
		D3DXVECTOR3		vPosition;
		D3DXVECTOR3		vNormal; 
		D3DXVECTOR2		vTexcoord; 
	}VTXNORMAL;
	
	typedef struct tagVertexCubeTexture
	{
		D3DXVECTOR3		vPosition;
		D3DXVECTOR3		vTexture;
	}VTXCUBETEX;

	typedef struct
	{
		D3DXVECTOR3 vPosition;
		D3DCOLOR	Color;
	}VTXCOLOR;

	typedef struct
	{
		class CGameObject*	pSrcObject;
		unsigned int		iLevel;
		std::wstring		strDstLayer;
		D3DXVECTOR3			vRayWorldPos;
		D3DXVECTOR3			vRayDir;
	}RAY_DESC;
}
