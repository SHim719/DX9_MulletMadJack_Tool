#include "VIBuffer_Elevator_L.h"
#include "Transform.h"

CVIBuffer_Elevator_L::CVIBuffer_Elevator_L(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer{ pGraphic_Device }
{
}

CVIBuffer_Elevator_L::CVIBuffer_Elevator_L(const CVIBuffer_Elevator_L& rhs)
	: CVIBuffer{ rhs }
{
}

HRESULT CVIBuffer_Elevator_L::Initialize_Prototype()
{
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
	pVertices[0].vTexcoord = { 0.f, 0.f };
	pVertices[0].vNormal = _float3(0.0f, 0.f, -1.f);

	pVertices[1].vPosition = _float3(0.5f, 0.5f, 0.f);
	pVertices[1].vTexcoord = { 0.5f, 0.0f };
	pVertices[1].vNormal = _float3(0.0f, 0.f, -1.f);

	pVertices[2].vPosition = _float3(0.5f, -0.5f, 0.f);
	pVertices[2].vTexcoord = { 0.5f, 1.f };
	pVertices[2].vNormal = _float3(0.0f, 0.f, -1.f);

	pVertices[3].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[3].vTexcoord = { 0.f, 1.f };
	pVertices[3].vNormal = _float3(0.0f, 0.f, -1.f);

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

HRESULT CVIBuffer_Elevator_L::Initialize(void* pArg)
{

	return S_OK;
}


CVIBuffer_Elevator_L* CVIBuffer_Elevator_L::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_Elevator_L* pInstance = new CVIBuffer_Elevator_L(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CVIBuffer_Elevator_L"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CComponent* CVIBuffer_Elevator_L::Clone(void* pArg)
{
	CVIBuffer_Elevator_L* pInstance = new CVIBuffer_Elevator_L(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CVIBuffer_Elevator_L"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Elevator_L::Free()
{
	__super::Free();


}