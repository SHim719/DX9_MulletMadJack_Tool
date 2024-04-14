#include "..\Public\VIBuffer_RectDeep.h"
#include "Transform.h"

CVIBuffer_RectDeep::CVIBuffer_RectDeep(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer{ pGraphic_Device }
{
}

CVIBuffer_RectDeep::CVIBuffer_RectDeep(const CVIBuffer_RectDeep& rhs)
	: CVIBuffer{ rhs }
{
}

HRESULT CVIBuffer_RectDeep::Initialize_Prototype()
{
	
	return S_OK;
}

HRESULT CVIBuffer_RectDeep::Initialize(void* pArg)
{
	Safe_Release(m_pVB);

	m_iNumVertices = 4;
	m_iVertexStride = sizeof(VTXNORMAL);
	m_FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1; //| D3DFVF_TEXCOORDSIZE2(0);
	m_ePrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumPrimitives = 2;

	/* ���� ������ ������� ���� �迭�� �Ҵ��Ѵ٤�. */
	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	VTXNORMAL* pVertices = { nullptr };

	/* ���İ� �Ҵ��� ������ ���� ä���. */
	/* �Ҵ��س��� ���� �迭�� �ּҸ� pVertices�� �����Ѵ�. */

	m_vecPositions.resize(4);

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vPosition = _float3(-0.5f, 0.5f, 0.f);
	pVertices[0].vTexcoord = _float2(0.0f, 0.f);

	pVertices[1].vPosition = _float3(0.5f, 0.5f, 0.f);
	pVertices[1].vTexcoord = _float2(1.0f, 0.f);

	pVertices[2].vPosition = _float3(0.5f, -0.5f, 0.f);
	pVertices[2].vTexcoord = _float2(1.0f, 1.0f);

	pVertices[3].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[3].vTexcoord = _float2(0.0f, 1.0f);

	m_pVB->Unlock();

	m_vecPositions[0] = pVertices[0].vPosition;
	m_vecPositions[1] = pVertices[1].vPosition;
	m_vecPositions[2] = pVertices[2].vPosition;
	m_vecPositions[3] = pVertices[3].vPosition;

	m_iIndexSizeofPrimitive = sizeof(FACEINDICES16);
	m_eIndexFormat = D3DFMT_INDEX16;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	FACEINDICES16* pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	pIndices[0]._0 = 0;
	pIndices[0]._1 = 1;
	pIndices[0]._2 = 2;

	pIndices[1]._0 = 0;
	pIndices[1]._1 = 2;
	pIndices[1]._2 = 3;

	m_pIB->Unlock();

	return S_OK;
}

void CVIBuffer_RectDeep::Reset_Texcoord(vector<_float2>& vecTexcoord)
{
	VTXNORMAL* pVertices = { nullptr };

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].vTexcoord = vecTexcoord[0];
	pVertices[1].vTexcoord = vecTexcoord[1];
	pVertices[2].vTexcoord = vecTexcoord[2];
	pVertices[3].vTexcoord = vecTexcoord[3];

	m_pVB->Unlock();
}


CVIBuffer_RectDeep* CVIBuffer_RectDeep::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_RectDeep* pInstance = new CVIBuffer_RectDeep(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CVIBuffer_RectDeep"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CComponent* CVIBuffer_RectDeep::Clone(void* pArg)
{
	CVIBuffer_RectDeep* pInstance = new CVIBuffer_RectDeep(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CVIBuffer_RectDeep"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_RectDeep::Free()
{
	__super::Free();


}
