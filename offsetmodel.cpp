////=============================================================================
////
////	モデル処理 [offsetmodel.cpp]
////	Author : SHUMA AIZU
//// 
////=============================================================================
//
//#include "main.h"
//#include "offsetmodel.h"
//#include "input.h"
//
////*****************************************************************************
//// マクロ定義
////*****************************************************************************
//
////*****************************************************************************
//// グローバル変数
////*****************************************************************************
//OffSetModel g_offsetmodel;												// モデルの情報
//
////=============================================================================
////	モデルの初期化処理
////=============================================================================
//void InitOffSetModel(void)
//{
//	// デバイスの取得
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();
//
//	int nNumVtx;		// 頂点数
//	DWORD dwSizeFVF;	// 頂点フォーマットのサイズ
//	BYTE* pVtxBuff;		// 頂点バッファへのポインタ
//
//	g_offsetmodel.pos = D3DXVECTOR3(0.0f, 0.0f, 300.0f);
//	g_offsetmodel.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//
//	// Xファイルの読み込み
//	D3DXLoadMeshFromX("data\\OFFSETMODEL\\clocktower000.x",
//		D3DXMESH_SYSTEMMEM,
//		pDevice,
//		NULL,
//		&g_pBuffMatOffSetModel,
//		NULL,
//		&g_dwNumMatOffSetModel,
//		&g_pMeshOffSetModel);
//
//	D3DXMATERIAL* pMat;
//
//	// マテリアルデータへのポインタを取得
//	pMat = (D3DXMATERIAL*)g_pBuffMatOffSetModel->GetBufferPointer();
//
//	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatOffSetModel; nCntMat++)
//	{
//		if (pMat[nCntMat].pTextureFilename != NULL)
//		{// テクスチャファイルが存在する
//				// テクスチャの読み込み
//			D3DXCreateTextureFromFile(pDevice,
//				pMat[nCntMat].pTextureFilename,
//				&g_apTextureOffSetModel[nCntMat]);
//		}
//	}
//
//	// 頂点数を取得
//	nNumVtx = g_pMeshOffSetModel->GetNumVertices();
//
//	// 頂点フォーマットのサイズを取得
//	dwSizeFVF = D3DXGetFVFVertexSize(g_pMeshOffSetModel->GetFVF());
//
//	// 頂点バッファをロック
//	g_pMeshOffSetModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);
//
//	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
//	{
//		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// 頂点情報の代入
//
//		if (vtx.x > g_vtxMaxOffSetModel.x)
//		{
//			g_vtxMaxOffSetModel.x = vtx.x;
//		}
//
//		if (vtx.y > g_vtxMaxOffSetModel.y)
//		{
//			g_vtxMaxOffSetModel.y = vtx.y;
//		}
//
//		if (vtx.z > g_vtxMaxOffSetModel.z)
//		{
//			g_vtxMaxOffSetModel.z = vtx.z;
//		}
//
//		if (vtx.x < g_vtxMinOffSetModel.x)
//		{
//			g_vtxMinOffSetModel.x = vtx.x;
//		}
//
//		if (vtx.y < g_vtxMinOffSetModel.y)
//		{
//			g_vtxMinOffSetModel.y = vtx.y;
//		}
//
//		if (vtx.z < g_vtxMinOffSetModel.z)
//		{
//			g_vtxMinOffSetModel.z = vtx.z;
//		}
//
//		pVtxBuff += dwSizeFVF;		// 頂点フォーマットのサイズ分ポインタを進める
//	}
//
//	// 頂点バッファのアンロック
//	g_pMeshOffSetModel->UnlockVertexBuffer();
//
//}
//
////=============================================================================
////	モデルの終了処理
////=============================================================================
//void UninitOffSetModel(void)
//{
//	// メッシュの破棄
//	if (g_pMeshOffSetModel != NULL)
//	{
//		g_pMeshOffSetModel->Release();
//		g_pMeshOffSetModel = 0;
//	}
//
//	// マテリアルの破棄
//	if (g_pBuffMatOffSetModel != NULL)
//	{
//		g_pBuffMatOffSetModel->Release();
//		g_pBuffMatOffSetModel = NULL;
//	}
//}
//
////=============================================================================
////	モデルの描画処理
////=============================================================================
//void DrawOffSetModel(void)
//{
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
//	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス
//	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用
//	D3DXMATERIAL* pMat;							// マテリアルデータへのポインタ
//
//	// ワールドマトリックスの初期化
//	D3DXMatrixIdentity(&g_mtxWarldOffSetModel);
//
//	// 向きを反映
//	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_offsetmodel.rot.y, g_offsetmodel.rot.x, g_offsetmodel.rot.z);
//	D3DXMatrixMultiply(&g_mtxWarldOffSetModel, &g_mtxWarldOffSetModel, &mtxRot);
//
//	// 位置を反映
//	D3DXMatrixTranslation(&mtxTrans, g_offsetmodel.pos.x, g_offsetmodel.pos.y, g_offsetmodel.pos.z);
//	D3DXMatrixMultiply(&g_mtxWarldOffSetModel, &g_mtxWarldOffSetModel, &mtxTrans);
//
//	// ワールドマトリックスの設定
//	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWarldOffSetModel);
//
//	// 現在のマテリアルを取得
//	pDevice->GetMaterial(&matDef);
//
//	// マテリアルデータへのポインタを取得
//	pMat = (D3DXMATERIAL*)g_pBuffMatOffSetModel->GetBufferPointer();
//
//	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatOffSetModel; nCntMat++)
//	{
//		// マテリアルの設定
//		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
//
//		// テクスチャの設定
//		pDevice->SetTexture(0, g_apTextureOffSetModel[nCntMat]);
//
//		// モデル(パーツ)の描画
//		g_pMeshOffSetModel->DrawSubset(nCntMat);
//	}
//
//	// 保存していたマテリアルを戻す
//	pDevice->SetMaterial(&matDef);
//}
//
////=============================================================================
////	モデルの更新処理
////=============================================================================
//void UpdateOffSetModel(void)
//{
//
//}
//
////=============================================================================
////	モデルの当たり判定処理
////=============================================================================
//void CollisionOffSetModel(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax)
//{
//	// モデルの範囲内か判定
//	if (pPos->x + vtxMax.x > g_offsetmodel.pos.x + g_vtxMinOffSetModel.x && pPos->x + vtxMin.x < g_offsetmodel.pos.x + g_vtxMaxOffSetModel.x &&
//		pPos->y + vtxMax.y > g_offsetmodel.pos.y + g_vtxMinOffSetModel.y && pPos->y + vtxMin.y < g_offsetmodel.pos.y + g_vtxMaxOffSetModel.y &&
//		pPos->z + vtxMax.z > g_offsetmodel.pos.z + g_vtxMinOffSetModel.z && pPos->z + vtxMin.z < g_offsetmodel.pos.z + g_vtxMaxOffSetModel.z)
//	{
//		// 左から
//		if (pPosOld->x + vtxMax.x <= g_offsetmodel.pos.x + g_vtxMinOffSetModel.x)
//		{
//			pPos->x = g_offsetmodel.pos.x + g_vtxMinOffSetModel.x - vtxMax.x;
//		}
//
//		// 右から
//		if (pPosOld->x + vtxMin.x >= g_offsetmodel.pos.x + g_vtxMaxOffSetModel.x)
//		{
//			pPos->x = g_offsetmodel.pos.x + g_vtxMaxOffSetModel.x - vtxMin.x;
//		}
//
//		// 上から
//		if (pPosOld->y + vtxMin.y >= g_offsetmodel.pos.y + g_vtxMaxOffSetModel.y)
//		{
//			pPos->y = g_offsetmodel.pos.y + g_vtxMaxOffSetModel.y - vtxMin.y;
//		}
//
//		// 下から
//		if (pPosOld->y + vtxMax.y <= g_offsetmodel.pos.y + g_vtxMinOffSetModel.y)
//		{
//			pPos->y = g_offsetmodel.pos.y + g_vtxMinOffSetModel.y - vtxMax.y;
//		}
//
//		// 手前から
//		if (pPosOld->z + vtxMax.z <= g_offsetmodel.pos.z + g_vtxMinOffSetModel.z)
//		{
//			pPos->z = g_offsetmodel.pos.z + g_vtxMinOffSetModel.z - vtxMax.z;
//		}
//
//		// 奥から
//		if (pPosOld->z + vtxMin.z >= g_offsetmodel.pos.z + g_vtxMaxOffSetModel.z)
//		{
//			pPos->z = g_offsetmodel.pos.z + g_vtxMaxOffSetModel.z - vtxMin.z;
//		}
//	}
//}