#include "Mesh_DoorLeft.h"



CMesh_DoorLeft::CMesh_DoorLeft(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer{ pGraphic_Device }
{
}

CMesh_DoorLeft::CMesh_DoorLeft(const CMesh_DoorLeft& rhs)
	: CVIBuffer	{ rhs }
{
}

HRESULT CMesh_DoorLeft::Initialize_Prototype()
{
	m_iNumVertices = 8;
	m_iVertexStride = sizeof(VTXCOLOR);
	m_FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE; //| D3DFVF_TEXCOORDSIZE2(0);
	m_ePrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumPrimitives = 12;

	/* 내가 지정한 정보대로 정점 배열을 할당한다ㅡ. */
	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXNORMAL* pVertices = { nullptr };

	/* 내ㅔ가 할당한 공간에 값을 채운다. */
	/* 할당해놨던 정점 배열의 주소를 pVertices에 저장한다. */
	

	m_vecPositions.resize(36);

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);
	pVertices[0].vPosition = m_vecPositions[0] = _float3(0.062500, -1.500000, 0.750000);
	pVertices[1].vPosition = m_vecPositions[0] = _float3(-0.062500, -1.500000, 0.750000);
	pVertices[2].vPosition = m_vecPositions[0] = _float3(0.062500, 1.500000, 0.750000);
	pVertices[3].vPosition = m_vecPositions[0] = _float3(-0.062500, 1.500000, 0.750000);
	pVertices[4].vPosition = m_vecPositions[0]=	_float3(- 0.062500, -1.500000, 0.750000	 );
	pVertices[5].vPosition = m_vecPositions[0]=	_float3(- 0.062500, -1.500000, -0.750000 );
	pVertices[6].vPosition = m_vecPositions[0]=	_float3(- 0.062500, 1.500000, 0.750000	 );
	pVertices[7].vPosition = m_vecPositions[0]=	_float3(- 0.062500, 1.500000, -0.750000	 );
	pVertices[8].vPosition = m_vecPositions[0]=	_float3(- 0.062500, -1.50000, -0.750000	 );
	pVertices[9].vPosition = m_vecPositions[0]=	_float3(0.062500, -1.500000, -0.750000	 );
	pVertices[0].vPosition = m_vecPositions[0]=	_float3(- 0.062500, 1.500000, -0.750000	 );
	pVertices[0].vPosition = m_vecPositions[0]=	_float3(0.062500, 1.500000, -0.750000	 );
	pVertices[0].vPosition = m_vecPositions[0]=	_float3(0.062500, -1.500000, -0.750000	 );
	pVertices[0].vPosition = m_vecPositions[0]=	_float3(0.062500, -1.500000, 0.750000	 );
	pVertices[0].vPosition = m_vecPositions[0]=	_float3(0.062500, 1.500000, -0.750000	 );
	pVertices[0].vPosition = m_vecPositions[0]=	_float3(0.062500, 1.500000, 0.750000	 );
	pVertices[0].vPosition = m_vecPositions[0]=	_float3(0.062500, 1.500000, 0.750000	 );
	pVertices[0].vPosition = m_vecPositions[0]=	_float3(- 0.062500, 1.500000, 0.750000	 );
	pVertices[0].vPosition = m_vecPositions[0]=	_float3(0.062500, 1.500000, -0.750000	 );
	pVertices[0].vPosition = m_vecPositions[0]=	_float3(- 0.062500, 1.500000, -0.750000	 );
	pVertices[0].vPosition = m_vecPositions[0]=	_float3(0.062500, -1.500000, -0.750000	 );
	pVertices[0].vPosition = m_vecPositions[0]=	_float3(- 0.062500, -1.50000, -0.750000	 );
	pVertices[0].vPosition = m_vecPositions[0]=	_float3(0.062500, -1.500000, 0.750000	 );
	pVertices[0].vPosition = m_vecPositions[0]=	_float3(- 0.062500, -1.50000, 0.750000	 );

	m_pVB->Unlock();

	m_iIndexSizeofPrimitive = sizeof(FACEINDICES16);
	m_eIndexFormat = D3DFMT_INDEX16;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	FACEINDICES16* pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	pIndices[0]._0 = 1; pIndices[0]._1 = 5; pIndices[0]._2 = 6;
	pIndices[1]._0 = 1; pIndices[1]._1 = 6; pIndices[1]._2 = 2;

	pIndices[2]._0 = 4; pIndices[2]._1 = 0; pIndices[2]._2 = 3;
	pIndices[3]._0 = 4; pIndices[3]._1 = 3; pIndices[3]._2 = 7;

	pIndices[4]._0 = 4; pIndices[4]._1 = 5; pIndices[4]._2 = 1;
	pIndices[5]._0 = 4; pIndices[5]._1 = 1; pIndices[5]._2 = 0;

	pIndices[6]._0 = 3; pIndices[6]._1 = 2; pIndices[6]._2 = 6;
	pIndices[7]._0 = 3; pIndices[7]._1 = 6; pIndices[7]._2 = 7;

	pIndices[8]._0 = 5; pIndices[8]._1 = 4; pIndices[8]._2 = 7;
	pIndices[9]._0 = 5; pIndices[9]._1 = 7; pIndices[9]._2 = 6;

	pIndices[10]._0 = 0; pIndices[10]._1 = 1; pIndices[10]._2 = 2;
	pIndices[11]._0 = 0; pIndices[11]._1 = 2; pIndices[11]._2 = 3;

	m_pIB->Unlock();

	return S_OK;
}

HRESULT CMesh_DoorLeft::Initialize(void* pArg)
{
	return S_OK;
}

CMesh_DoorLeft* CMesh_DoorLeft::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMesh_DoorLeft* pInstance = new CMesh_DoorLeft(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CMesh_DoorLeft"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CMesh_DoorLeft::Clone(void* pArg)
{
	CMesh_DoorLeft* pInstance = new CMesh_DoorLeft(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CMesh_DoorLeft"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMesh_DoorLeft::Free()
{
	__super::Free();

}
