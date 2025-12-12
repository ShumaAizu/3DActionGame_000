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
//ObjectInfo g_aobjectInfo[MAX_OBJECT] = {};
//int g_nNumObject;
//
//void InitObject(void)
//{
//	g_nNumObject = -1;
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
//	POBJECTUNFO pObject = &g_aobjectInfo[0];
//
//	int nNumVtx;		// 頂点数
//	DWORD dwSizeFVF;	// 頂点フォーマットのサイズ
//	BYTE* pVtxBuff;		// 頂点バッファへのポインタ
//
//	for (int nCntObjectInfo = 0; nCntObjectInfo < MAX_OBJECT; nCntObjectInfo++, pObject++)
//	{
//		if (pObject->bUse == true)
//		{
//			continue;
//		}
//
//		// Xファイルの読み込み
//		D3DXLoadMeshFromX(pFileName,
//			D3DXMESH_SYSTEMMEM,
//			pDevice,
//			NULL,
//			&pObject->pBuffMat,
//			NULL,
//			&pObject->dwNumMat,
//			&pObject->pMesh);
//
//		D3DXMATERIAL* pMat;
//
//		// マテリアルデータへのポインタを取得
//		pMat = (D3DXMATERIAL*)pObject->pBuffMat->GetBufferPointer();
//
//		for (int nCntMat = 0; nCntMat < (int)pObject->dwNumMat; nCntMat++)
//		{
//			if (pMat[nCntMat].pTextureFilename != NULL)
//			{// テクスチャファイルが存在する
//				// テクスチャの読み込み
//				D3DXCreateTextureFromFile(pDevice,
//					pMat[nCntMat].pTextureFilename,
//					&pObject->apTexture[nCntMat]);
//			}
//		}
//
//		// 頂点数を取得
//		nNumVtx = pObject->pMesh->GetNumVertices();
//
//		// 頂点フォーマットのサイズを取得
//		dwSizeFVF = D3DXGetFVFVertexSize(pObject->pMesh->GetFVF());
//
//		// 頂点バッファをロック
//		pObject->pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);
//
//		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
//		{
//			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// 頂点情報の代入
//
//			if (vtx.x > pObject->vtxMax.x)
//			{
//				pObject->vtxMax.x = vtx.x;
//			}
//
//			if (vtx.y > pObject->vtxMax.y)
//			{
//				pObject->vtxMax.y = vtx.y;
//			}
//
//			if (vtx.z > pObject->vtxMax.z)
//			{
//				pObject->vtxMax.z = vtx.z;
//			}
//
//			if (vtx.x < pObject->vtxMin.x)
//			{
//				pObject->vtxMin.x = vtx.x;
//			}
//
//			if (vtx.y < pObject->vtxMin.y)
//			{
//				pObject->vtxMin.y = vtx.y;
//			}
//
//			if (vtx.z < pObject->vtxMin.z)
//			{
//				pObject->vtxMin.z = vtx.z;
//			}
//
//			pVtxBuff += dwSizeFVF;		// 頂点フォーマットのサイズ分ポインタを進める
//		}
//
//		// 頂点バッファのアンロック
//		pObject->pMesh->UnlockVertexBuffer();
//	}
//}