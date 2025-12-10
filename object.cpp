////=============================================================================
////
////	オブジェクト処理 [object.cpp]
////	Author : SHUMA AIZU
//// 
////=============================================================================
//
//#include "object.h"
//
////*****************************************************************************
//// グローバル変数
////*****************************************************************************
//ObjectInfo g_objectInfo[MAX_OBJECT] = {};
//
//void InitObject(void)
//{
//
//}
//
//void UninitObject(void)
//{
//
//}
//
//void UpdateObject(void)
//{
//
//}
//
//void DrawObject(void)
//{
//
//}
//
////=============================================================================
////	オブジェクトの読み込み処理
////=============================================================================
//void LoadObject(const char* pFileName)
//{
//	// デバイスの取得
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();
//
//	int nNumVtx;		// 頂点数
//	DWORD dwSizeFVF;	// 頂点フォーマットのサイズ
//	BYTE* pVtxBuff;		// 頂点バッファへのポインタ
//
//	for (int nCntObjectInfo = 0; nCntObjectInfo < MAX_OBJECT; nCntObjectInfo++)
//	{
//		if (g_objectInfo[nCntObjectInfo].bUse == true)
//		{
//			continue;
//		}
//
//		// Xファイルの読み込み
//		D3DXLoadMeshFromX(pFileName,
//			D3DXMESH_SYSTEMMEM,
//			pDevice,
//			NULL,
//			&g_objectInfo[nCntObjectInfo].pBuffMat,
//			NULL,
//			&g_objectInfo[nCntObjectInfo].dwNumMat,
//			&g_objectInfo[nCntObjectInfo].pMesh);
//
//		D3DXMATERIAL* pMat;
//
//		// マテリアルデータへのポインタを取得
//		pMat = (D3DXMATERIAL*)g_objectInfo[nCntObjectInfo].pBuffMat->GetBufferPointer();
//
//		for (int nCntMat = 0; nCntMat < (int)g_objectInfo[nCntObjectInfo].dwNumMat; nCntMat++)
//		{
//			if (pMat[nCntMat].pTextureFilename != NULL)
//			{// テクスチャファイルが存在する
//				// テクスチャの読み込み
//				D3DXCreateTextureFromFile(pDevice,
//					pMat[nCntMat].pTextureFilename,
//					&g_objectInfo[nCntObjectInfo].apTexture[nCntMat]);
//			}
//		}
//
//		// 頂点数を取得
//		nNumVtx = g_objectInfo[nCntObjectInfo].pMesh->GetNumVertices();
//
//		// 頂点フォーマットのサイズを取得
//		dwSizeFVF = D3DXGetFVFVertexSize(g_objectInfo[nCntObjectInfo].pMesh->GetFVF());
//
//		// 頂点バッファをロック
//		g_objectInfo[nCntObjectInfo].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);
//
//		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
//		{
//			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// 頂点情報の代入
//
//			if (vtx.x > g_objectInfo[nCntObjectInfo].vtxMax.x)
//			{
//				g_objectInfo[nCntObjectInfo].vtxMax.x = vtx.x;
//			}
//
//			if (vtx.y > g_objectInfo[nCntObjectInfo].vtxMax.y)
//			{
//				g_objectInfo[nCntObjectInfo].vtxMax.y = vtx.y;
//			}
//
//			if (vtx.z > g_objectInfo[nCntObjectInfo].vtxMax.z)
//			{
//				g_objectInfo[nCntObjectInfo].vtxMax.z = vtx.z;
//			}
//
//			if (vtx.x < g_objectInfo[nCntObjectInfo].vtxMin.x)
//			{
//				g_objectInfo[nCntObjectInfo].vtxMin.x = vtx.x;
//			}
//
//			if (vtx.y < g_objectInfo[nCntObjectInfo].vtxMin.y)
//			{
//				g_objectInfo[nCntObjectInfo].vtxMin.y = vtx.y;
//			}
//
//			if (vtx.z < g_objectInfo[nCntObjectInfo].vtxMin.z)
//			{
//				g_objectInfo[nCntObjectInfo].vtxMin.z = vtx.z;
//			}
//
//			pVtxBuff += dwSizeFVF;		// 頂点フォーマットのサイズ分ポインタを進める
//		}
//
//		// 頂点バッファのアンロック
//		g_objectInfo[nCntObjectInfo].pMesh->UnlockVertexBuffer();
//	}
//}