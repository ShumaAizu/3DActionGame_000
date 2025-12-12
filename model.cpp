//=============================================================================
//
//	モデル処理 [model.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "model.h"
#include "input.h"

#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_MODEL		(128)		// モデルの最大数

//*****************************************************************************
// グローバル変数
//*****************************************************************************
Modeldata g_amodeldata[MODELTYPE_MAX];							// モデルデータの情報
Model g_amodel[MAX_MODEL];										// モデルの情報
int g_nSelectModel;
int g_nNumModel;
int g_nSelectType;
float g_fModelMove;

const char* g_pModelFileName[MODELTYPE_MAX] =
{
	"data\\MODEL\\Iceberg000.x",
	"data\\MODEL\\snow001.x",
	"data\\MODEL\\field001.x",
};

//=============================================================================
//	モデルの初期化処理
//=============================================================================
void InitModel(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	BYTE* pVtxBuff;								// 頂点バッファへのポインタ
	BYTE* pIdxBuff;								// インデックスバッファへのポインタ

	for (int nCntModeldata = 0; nCntModeldata < MODELTYPE_MAX; nCntModeldata++)
	{
		// Xファイルの読み込み
		D3DXLoadMeshFromX(g_pModelFileName[nCntModeldata],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_amodeldata[nCntModeldata].pBuffMat,
			NULL,
			&g_amodeldata[nCntModeldata].dwNumMat,
			&g_amodeldata[nCntModeldata].pMesh);

		D3DXMATERIAL* pMat;

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_amodeldata[nCntModeldata].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_amodeldata[nCntModeldata].dwNumMat; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{// テクスチャファイルが存在する
				// テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_amodeldata[nCntModeldata].apTexture[nCntMat]);
			}
		}

		// 頂点数を取得
		g_amodeldata[nCntModeldata].nNumVtx = g_amodeldata[nCntModeldata].pMesh->GetNumVertices();

		// インデックス数を取得
		g_amodeldata[nCntModeldata].nNumIdx = g_amodeldata[nCntModeldata].pMesh->GetNumFaces();

		// 頂点フォーマットのサイズを取得
		g_amodeldata[nCntModeldata].dwSizeFVF = D3DXGetFVFVertexSize(g_amodeldata[nCntModeldata].pMesh->GetFVF());

		// 頂点バッファをロック
		g_amodeldata[nCntModeldata].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < g_amodeldata[nCntModeldata].nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// 頂点情報の代入

			if (vtx.x > g_amodeldata[nCntModeldata].vtxMax.x)
			{
				g_amodeldata[nCntModeldata].vtxMax.x = vtx.x;
			}

			if (vtx.y > g_amodeldata[nCntModeldata].vtxMax.y)
			{
				g_amodeldata[nCntModeldata].vtxMax.y = vtx.y;
			}

			if (vtx.z > g_amodeldata[nCntModeldata].vtxMax.z)
			{
				g_amodeldata[nCntModeldata].vtxMax.z = vtx.z;
			}

			if (vtx.x < g_amodeldata[nCntModeldata].vtxMin.x)
			{
				g_amodeldata[nCntModeldata].vtxMin.x = vtx.x;
			}

			if (vtx.y < g_amodeldata[nCntModeldata].vtxMin.y)
			{
				g_amodeldata[nCntModeldata].vtxMin.y = vtx.y;
			}

			if (vtx.z < g_amodeldata[nCntModeldata].vtxMin.z)
			{
				g_amodeldata[nCntModeldata].vtxMin.z = vtx.z;
			}

			pVtxBuff += g_amodeldata[nCntModeldata].dwSizeFVF;		// 頂点フォーマットのサイズ分ポインタを進める
		}

		// 頂点バッファのアンロック
		g_amodeldata[nCntModeldata].pMesh->UnlockVertexBuffer();

		//// インデックスバッファをロック
		//g_amodeldata[nCntModeldata].pMesh->LockIndexBuffer(D3DLOCK_READONLY, (void**)&pIdxBuff);

		////for (int nCntIdx = 0; nCntIdx < nNumIdx * 3; nCntIdx++)
		////{
		////	g_amodeldata->nIdx[nCntIdx] = *pIdxBuff;	// 頂点情報の代入

		////	pIdxBuff += 2;
		////}

		//// 頂点インデックスバッファのアンロック
		//g_amodeldata[nCntModeldata].pMesh->UnlockIndexBuffer();

	}


	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		g_amodel[nCntModel].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_amodel[nCntModel].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_amodel[nCntModel].bCollision = false;
		g_amodel[nCntModel].bUse = false;
	}

	g_nNumModel = -1;
	g_nSelectModel = -1;
	g_nSelectType = 0;
	g_fModelMove = 1.0f;

	SetModel(D3DXVECTOR3(-2350.0f, -205.0f, 1850.0f), D3DXVECTOR3(0.0f, 1.18f, 0.0f), D3DXVECTOR3(3.25f, 3.25f, 3.25f), MODELTYPE_000);
	//SetModel(D3DXVECTOR3(-120.0f, -120.0f, 3380.0f), D3DXVECTOR3(0.0f, -1.83f, 0.0f), D3DXVECTOR3(5.0f, 5.0f, 5.0f), MODELTYPE_000);
	//SetModel(D3DXVECTOR3(2200.0f, -240.0f, 1850.0f), D3DXVECTOR3(0.0f, 0.52f, 0.0f), D3DXVECTOR3(1.5f, 1.5f, 1.5f), MODELTYPE_000);
	//SetModel(D3DXVECTOR3(-1025.0f, 200.0f, -1250.0f), D3DXVECTOR3(0.0f, 3.08f, 0.0f), D3DXVECTOR3(0.30f, 0.30f, 0.30f), MODELTYPE_002);
	//SetModel(D3DXVECTOR3(-875.0f, 375.0f, 825.0f), D3DXVECTOR3(0.0f, -1.08f, 0.0f), D3DXVECTOR3(0.60f, 0.60f, 0.60f), MODELTYPE_002);
	//SetModel(D3DXVECTOR3(775.0f, 0.0f, -1275.0f), D3DXVECTOR3(0.0f, 2.10f, 0.0f), D3DXVECTOR3(0.1f, 0.1f, 0.1f), MODELTYPE_002);
	//SetModel(D3DXVECTOR3(1050.0f, 200.0f, 75.0f), D3DXVECTOR3(0.0f, 1.40f, 0.0f), D3DXVECTOR3(0.3f, 0.3f, 0.3f), MODELTYPE_002);
	SetModel(D3DXVECTOR3(-870.0f, 0.0f, -1215.0f), D3DXVECTOR3(0.0f, -1.30f, 0.0f), D3DXVECTOR3(7.70f, 7.70f, 7.70f), MODELTYPE_001);
	SetModel(D3DXVECTOR3(1290.0f, 0.0f, -165.0f), D3DXVECTOR3(0.0f, -0.50f, 0.0f), D3DXVECTOR3(6.5f, 6.5f, 6.5f), MODELTYPE_001);
	SetModel(D3DXVECTOR3(-960.0f, 0.0f, 930.0f), D3DXVECTOR3(0.0f, 0.40f, 0.0f), D3DXVECTOR3(8.4f, 8.4f, 8.4f), MODELTYPE_001);
	SetModel(D3DXVECTOR3(645.0f, 0.0f, -1680.0f), D3DXVECTOR3(0.0f, -1.30f, 0.0f), D3DXVECTOR3(2.3f, 2.3f, 2.3f), MODELTYPE_001);
	SetModel(D3DXVECTOR3(840.0f, 0.0f, 1380.0f), D3DXVECTOR3(0.0f, 0.10f, 0.0f), D3DXVECTOR3(4.2f, 4.2f, 4.2f), MODELTYPE_001);

	//SetModel(D3DXVECTOR3(400.0f, 0.0f, 450.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), MODELTYPE_001);
	//SetModel(D3DXVECTOR3(0.0f, 75.0f, 100.0f), D3DXVECTOR3(0.0f, 0.75f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), MODELTYPE_002);
	SetModel(D3DXVECTOR3(0.0f, 0.0f, -1000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), MODELTYPE_001);

}

//=============================================================================
//	モデルの終了処理
//=============================================================================
void UninitModel(void)
{
	for (int nCntModeldata = 0; nCntModeldata < MODELTYPE_MAX; nCntModeldata++)
	{
		// テクスチャの破棄
		for (int nCntTex = 0; nCntTex < MAX_MODELTEX; nCntTex++)
		{
			if (g_amodeldata[nCntModeldata].apTexture[nCntTex] != NULL)
			{
				g_amodeldata[nCntModeldata].apTexture[nCntTex]->Release();
				g_amodeldata[nCntModeldata].apTexture[nCntTex] = NULL;
			}
		}

		// メッシュの破棄
		if (g_amodeldata[nCntModeldata].pMesh != NULL)
		{
			g_amodeldata[nCntModeldata].pMesh->Release();
			g_amodeldata[nCntModeldata].pMesh = NULL;
		}

		// マテリアルの破棄
		if (g_amodeldata[nCntModeldata].pBuffMat != NULL)
		{
			g_amodeldata[nCntModeldata].pBuffMat->Release();
			g_amodeldata[nCntModeldata].pBuffMat = NULL;
		}
	}
}

//=============================================================================
//	モデルの描画処理
//=============================================================================
void DrawModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;		// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用
	D3DXMATERIAL* pMat;							// マテリアルデータへのポインタ

	Model* pModel = &g_amodel[0];

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++, pModel++)
	{
		if (pModel->bUse == false)
		{
			continue;
		}

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pModel->mtxWorld);

		D3DXMatrixScaling(&mtxScale, pModel->scale.y, pModel->scale.x, pModel->scale.z);
		D3DXMatrixMultiply(&pModel->mtxWorld, &pModel->mtxWorld, &mtxScale);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pModel->rot.y, pModel->rot.x, pModel->rot.z);
		D3DXMatrixMultiply(&pModel->mtxWorld, &pModel->mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, pModel->pos.x, pModel->pos.y, pModel->pos.z);
		D3DXMatrixMultiply(&pModel->mtxWorld, &pModel->mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &pModel->mtxWorld);

		// 現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_amodeldata[pModel->modeltype].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_amodeldata[pModel->modeltype].dwNumMat; nCntMat++)
		{
			// マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_amodeldata[pModel->modeltype].apTexture[nCntMat]);

			// モデル(パーツ)の描画
			g_amodeldata[pModel->modeltype].pMesh->DrawSubset(nCntMat);
		}

		// 保存していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}
}

//=============================================================================
//	モデルの更新処理
//=============================================================================
void UpdateModel(void)
{
	if (GetKeyboardTrigger(DIK_F2) == true)
	{
		g_nSelectModel = (g_nSelectModel + 1) % (g_nNumModel + 1);
	}

	if (GetKeyboardTrigger(DIK_F3) == true)
	{
		g_nSelectType = (g_nSelectType + 1) % MODELTYPE_MAX;
	}

	if (GetKeyboardRepeat(DIK_F4) == true)
	{
		g_fModelMove += -1.0f;
	}

	if (GetKeyboardRepeat(DIK_F5) == true)
	{
		g_fModelMove += 1.0f;
	}

	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		SetModel(INIT_D3DXVEC3, INIT_D3DXVEC3, D3DXVECTOR3(1.0f, 1.0f, 1.0f), (MODELTYPE)g_nSelectType);
	}

	if (GetKeyboardPress(DIK_T) == true)
	{
		g_amodel[g_nSelectModel].pos.z += g_fModelMove;
	}

	if (GetKeyboardPress(DIK_F) == true)
	{
		g_amodel[g_nSelectModel].pos.x += -g_fModelMove;
	}

	if (GetKeyboardPress(DIK_G) == true)
	{
		g_amodel[g_nSelectModel].pos.z += -g_fModelMove;
	}

	if (GetKeyboardPress(DIK_H) == true)
	{
		g_amodel[g_nSelectModel].pos.x += g_fModelMove;
	}

	if (GetKeyboardPress(DIK_X) == true)
	{
		g_amodel[g_nSelectModel].pos.y += g_fModelMove;
	}

	if (GetKeyboardPress(DIK_V) == true)
	{
		g_amodel[g_nSelectModel].pos.y += -g_fModelMove;
	}

	if (GetKeyboardPress(DIK_Y) == true)
	{
		g_amodel[g_nSelectModel].rot.y += 0.1f;
		g_amodel[g_nSelectModel].rot.y = AngleNormalize(g_amodel[g_nSelectModel].rot.y);
	}

	if (GetKeyboardPress(DIK_R) == true)
	{
		g_amodel[g_nSelectModel].rot.y += -0.1f;
		g_amodel[g_nSelectModel].rot.y = AngleNormalize(g_amodel[g_nSelectModel].rot.y);
	}

	if (GetKeyboardPress(DIK_UPARROW) == true)
	{
		g_amodel[g_nSelectModel].scale.x += 0.1f;
		g_amodel[g_nSelectModel].scale.y += 0.1f;
		g_amodel[g_nSelectModel].scale.z += 0.1f;
	}

	if (GetKeyboardPress(DIK_DOWNARROW) == true)
	{
		g_amodel[g_nSelectModel].scale.x += -0.1f;
		g_amodel[g_nSelectModel].scale.y += -0.1f;
		g_amodel[g_nSelectModel].scale.z += -0.1f;
	}

	PrintDebugProc("選択モデル = { %d }\n", g_nSelectModel);
	PrintDebugProc("モデル数 = { %d }\n", g_nNumModel);
	PrintDebugProc("選択タイプ = { %d }\n", g_nSelectType);
	PrintDebugProc("モデル移動量 = { %.2f }\n", g_fModelMove);
	PrintDebugProc("ModelPos = { %.2f %.2f %.2f }\n", g_amodel[g_nSelectModel].pos.x, g_amodel[g_nSelectModel].pos.y, g_amodel[g_nSelectModel].pos.z);
	PrintDebugProc("ModelRot = { %.2f %.2f %.2f }\n", g_amodel[g_nSelectModel].rot.x, g_amodel[g_nSelectModel].rot.y, g_amodel[g_nSelectModel].rot.z);
	PrintDebugProc("ModelScale = { %.2f %.2f %.2f }\n", g_amodel[g_nSelectModel].scale.x, g_amodel[g_nSelectModel].scale.y, g_amodel[g_nSelectModel].scale.z);
}

//=============================================================================
//	モデルの当たり判定処理
//=============================================================================
void CollisionModel(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 * pMove, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax)
{
	Model* pModel = &g_amodel[0];					// 先頭アドレス
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;			// 計算用マトリックス
	D3DXVECTOR3 posMax, posMin;						// 
	D3DXVECTOR3 posA, posB, posC, posD;

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++, pModel++)
	{
		if (g_amodel[nCntModel].bUse == false)
		{// 使用していなかったら戻る
			continue;
		}

		Modeldata* pModelData = &g_amodeldata[pModel->modeltype];	// モデルタイプ

		posA = D3DXVECTOR3(pModelData->vtxMin.x, 0.0f, pModelData->vtxMax.z);
		posB = D3DXVECTOR3(pModelData->vtxMax.x, 0.0f, pModelData->vtxMax.z);
		posC = D3DXVECTOR3(pModelData->vtxMax.x, 0.0f, pModelData->vtxMin.z);
		posD = D3DXVECTOR3(pModelData->vtxMin.x, 0.0f, pModelData->vtxMin.z);

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pModel->mtxWorld);

		D3DXMatrixScaling(&mtxScale, pModel->scale.y, pModel->scale.x, pModel->scale.z);
		D3DXMatrixMultiply(&pModel->mtxWorld, &pModel->mtxWorld, &mtxScale);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pModel->rot.y, pModel->rot.x, pModel->rot.z);
		D3DXMatrixMultiply(&pModel->mtxWorld, &pModel->mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, pModel->pos.x, pModel->pos.y, pModel->pos.z);
		D3DXMatrixMultiply(&pModel->mtxWorld, &pModel->mtxWorld, &mtxTrans);

		// 位置と向きを反映した頂点座標を入れる
		D3DXVec3TransformCoord(&posA, &posA, &pModel->mtxWorld);
		D3DXVec3TransformCoord(&posB, &posB, &pModel->mtxWorld);
		D3DXVec3TransformCoord(&posC, &posC, &pModel->mtxWorld);
		D3DXVec3TransformCoord(&posD, &posD, &pModel->mtxWorld);

		// 当たり判定
		if (pModel->bCollision == true)
		{
			CrossCollision(pPos, pPosOld, posB, posA, true, false);
			CrossCollision(pPos, pPosOld, posC, posB, true, false);
			CrossCollision(pPos, pPosOld, posD, posC, true, false);
			CrossCollision(pPos, pPosOld, posA, posD, true, false);
		}

		// モデルの範囲内か判定
		if (pPos->x + vtxMax.x > g_amodel[nCntModel].pos.x + g_amodeldata[g_amodel[nCntModel].modeltype].vtxMin.x && pPos->x + vtxMin.x < g_amodel[nCntModel].pos.x + g_amodeldata[g_amodel[nCntModel].modeltype].vtxMax.x &&
			pPos->y + vtxMax.y > g_amodel[nCntModel].pos.y +  g_amodeldata[g_amodel[nCntModel].modeltype].vtxMin.y && pPos->y + vtxMin.y < g_amodel[nCntModel].pos.y +  g_amodeldata[g_amodel[nCntModel].modeltype].vtxMax.y &&
			pPos->z + vtxMax.z > g_amodel[nCntModel].pos.z +  g_amodeldata[g_amodel[nCntModel].modeltype].vtxMin.z && pPos->z + vtxMin.z < g_amodel[nCntModel].pos.z +  g_amodeldata[g_amodel[nCntModel].modeltype].vtxMax.z)
		{
			//// 左から
			//if (pPosOld->x + vtxMax.x <= g_amodel[nCntModel].pos.x + g_amodeldata[g_amodel[nCntModel].modeltype].vtxMin.x)
			//{
			//	pPos->x = g_amodel[nCntModel].pos.x + g_amodeldata[g_amodel[nCntModel].modeltype].vtxMin.x - vtxMax.x;
			//}

			//// 右から
			//if (pPosOld->x + vtxMin.x >= g_amodel[nCntModel].pos.x + g_amodeldata[g_amodel[nCntModel].modeltype].vtxMax.x)
			//{
			//	pPos->x = g_amodel[nCntModel].pos.x + g_amodeldata[g_amodel[nCntModel].modeltype].vtxMax.x - vtxMin.x;
			//}

			// 上から
			if (pPosOld->y + vtxMin.y >= g_amodel[nCntModel].pos.y + g_amodeldata[g_amodel[nCntModel].modeltype].vtxMax.y)
			{
				pPos->y = g_amodel[nCntModel].pos.y + g_amodeldata[g_amodel[nCntModel].modeltype].vtxMax.y - vtxMin.y;
			}

			// 下から
			if (pPosOld->y + vtxMax.y <= g_amodel[nCntModel].pos.y + g_amodeldata[g_amodel[nCntModel].modeltype].vtxMin.y)
			{
				pPos->y = g_amodel[nCntModel].pos.y + g_amodeldata[g_amodel[nCntModel].modeltype].vtxMin.y - vtxMax.y;
			}

			//// 手前から
			//if (pPosOld->z + vtxMax.z <= g_amodel[nCntModel].pos.z + g_amodeldata[g_amodel[nCntModel].modeltype].vtxMin.z)
			//{
			//	pPos->z = g_amodel[nCntModel].pos.z + g_amodeldata[g_amodel[nCntModel].modeltype].vtxMin.z - vtxMax.z;
			//}

			//// 奥から
			//if (pPosOld->z + vtxMin.z >= g_amodel[nCntModel].pos.z + g_amodeldata[g_amodel[nCntModel].modeltype].vtxMax.z)
			//{
			//	pPos->z = g_amodel[nCntModel].pos.z + g_amodeldata[g_amodel[nCntModel].modeltype].vtxMax.z - vtxMin.z;
			//}
		}
	}
}

//=============================================================================
//	モデルの設定処理
//=============================================================================
void SetModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, MODELTYPE modeltype)
{
	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		if (g_amodel[nCntModel].bUse == true)
		{
			continue;
		}

		g_nNumModel++;
		g_nSelectModel++;

		g_amodel[nCntModel].pos = pos;
		g_amodel[nCntModel].rot = rot;
		g_amodel[nCntModel].scale = scale;
		g_amodel[nCntModel].modeltype = modeltype;
		g_amodel[nCntModel].bUse = true;
		break;
	}
}

void CollisionMeshModelTest(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld)
{
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;		// 計算用マトリックス
	D3DXVECTOR3 vecNor, VecMove;				// 各種ベクトル
	Model* pModel = &g_amodel[0];

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++, pModel++)
	{
		if (pModel->bUse == false)
		{
			continue;
		}

		BYTE* pVtxBuff;								// 頂点バッファへのポインタ
		BYTE* pIdxBuff;								// インデックスバッファへのポインタ

		// 頂点バッファをロック
		g_amodeldata[pModel->modeltype].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		// インデックスバッファをロック
		g_amodeldata[pModel->modeltype].pMesh->LockIndexBuffer(D3DLOCK_READONLY, (void**)&pIdxBuff);

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pModel->mtxWorld);

		D3DXMatrixScaling(&mtxScale, pModel->scale.y, pModel->scale.x, pModel->scale.z);
		D3DXMatrixMultiply(&pModel->mtxWorld, &pModel->mtxWorld, &mtxScale);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pModel->rot.y, pModel->rot.x, pModel->rot.z);
		D3DXMatrixMultiply(&pModel->mtxWorld, &pModel->mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, pModel->pos.x, pModel->pos.y, pModel->pos.z);
		D3DXMatrixMultiply(&pModel->mtxWorld, &pModel->mtxWorld, &mtxTrans);

		VecMove = *pPos - *pPosOld;

		for (int nCntFaces = 0; nCntFaces < g_amodeldata[pModel->modeltype].nNumIdx; nCntFaces++)
		{
			D3DXVECTOR3 PosA, PosB, PosC;
			PosA = *(D3DXVECTOR3*)(pVtxBuff + (g_amodeldata[pModel->modeltype].dwSizeFVF * *pIdxBuff));		// 頂点情報の代入
			pIdxBuff += 2;

			PosB = *(D3DXVECTOR3*)(pVtxBuff + (g_amodeldata[pModel->modeltype].dwSizeFVF * *pIdxBuff));		// 頂点情報の代入
			pIdxBuff += 2;

			PosC = *(D3DXVECTOR3*)(pVtxBuff + (g_amodeldata[pModel->modeltype].dwSizeFVF * *pIdxBuff));		// 頂点情報の代入
			pIdxBuff += 2;

			// 位置と向きを反映した頂点座標を入れる
			D3DXVec3TransformCoord(&PosA, &PosA, &pModel->mtxWorld);
			D3DXVec3TransformCoord(&PosB, &PosB, &pModel->mtxWorld);
			D3DXVec3TransformCoord(&PosC, &PosC, &pModel->mtxWorld);

			// 3辺の境界線ベクトルを算出
			D3DXVECTOR3 vecLineA = PosB - PosA;
			D3DXVECTOR3 vecLineB = PosC - PosB;
			D3DXVECTOR3 vecLineC = PosA - PosC;

			// それぞれの始点から位置へのベクトルを算出
			D3DXVECTOR3 vecToPosA = *pPos - PosA;
			D3DXVECTOR3 vecToPosB = *pPos - PosB;
			D3DXVECTOR3 vecToPosC = *pPos - PosC;

			// 正規化
			D3DXVec3Normalize(&vecLineA, &vecLineA);
			D3DXVec3Normalize(&vecLineB, &vecLineB);
			D3DXVec3Normalize(&vecLineC, &vecLineC);

			if ((vecLineA.z * vecToPosA.x) - (vecLineA.x * vecToPosA.z) > 0 &&
				(vecLineB.z * vecToPosB.x) - (vecLineB.x * vecToPosB.z) > 0 &&
				(vecLineC.z * vecToPosC.x) - (vecLineC.x * vecToPosC.z) > 0)
			{// もし全ての境界線ベクトルの内側にいたら
				//pVtx[nCntVtxWidht].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				//pVtx[nCntVtxWidht + pMeshField->nSplitWidth].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				//pVtx[nCntVtxWidht + pMeshField->nSplitWidth + 1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

				//SetEffect(PosA, INIT_D3DXVEC3, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 15.0f, 0.0f, 0.0f, 1);
				//SetEffect(PosB, INIT_D3DXVEC3, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 15.0f, 0.0f, 0.0f, 1);
				//SetEffect(PosC, INIT_D3DXVEC3, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 15.0f, 0.0f, 0.0f, 1);

				PrintDebugProc("成功\n");

				// 面の法線
				D3DXVECTOR3 vecNor = { (-vecLineA.y * vecLineB.z) - (-vecLineA.z * vecLineB.y), (-vecLineA.z * vecLineB.x) - (-vecLineA.x * vecLineB.z), (-vecLineA.x * vecLineB.y) - (-vecLineA.y * vecLineB.x) };

				D3DXVec3Normalize(&vecNor, &vecNor);	// 正規化

				// 高さ
				float fPosY = PosA.y - (((vecNor.x * pPos->x) - (vecNor.x * PosA.x)) + ((vecNor.z * pPos->z) - (vecNor.z * PosA.z)) / vecNor.y);

				if (pPos->y <= fPosY)
				{
					pPos->y = fPosY;
				}

				//// 頂点インデックスバッファのアンロック
				//g_amodeldata[pModel->modeltype].pMesh->UnlockIndexBuffer();

				//// 頂点インデックスバッファのアンロック
				//g_amodeldata[pModel->modeltype].pMesh->UnlockIndexBuffer();
				//return;
			}

			if ((vecLineA.z * vecToPosA.x) - (vecLineA.x * vecToPosA.z) < 0 &&
				(vecLineB.z * vecToPosB.x) - (vecLineB.x * vecToPosB.z) < 0 &&
				(vecLineC.z * vecToPosC.x) - (vecLineC.x * vecToPosC.z) < 0)
			{// もし全ての境界線ベクトルの内側にいたら
				//pVtx[nCntVtxWidht].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				//pVtx[nCntVtxWidht + pMeshField->nSplitWidth].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				//pVtx[nCntVtxWidht + pMeshField->nSplitWidth + 1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

				//SetEffect(PosA, INIT_D3DXVEC3, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 15.0f, 0.0f, 0.0f, 1);
				//SetEffect(PosB, INIT_D3DXVEC3, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 15.0f, 0.0f, 0.0f, 1);
				//SetEffect(PosC, INIT_D3DXVEC3, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 15.0f, 0.0f, 0.0f, 1);

				PrintDebugProc("成功\n");

				// 面の法線
				D3DXVECTOR3 vecNor = { (-vecLineA.y * vecLineB.z) - (-vecLineA.z * vecLineB.y), (-vecLineA.z * vecLineB.x) - (-vecLineA.x * vecLineB.z), (-vecLineA.x * vecLineB.y) - (-vecLineA.y * vecLineB.x) };

				D3DXVec3Normalize(&vecNor, &vecNor);	// 正規化

				// 高さ
				float fPosY = PosA.y - (((vecNor.x * pPos->x) - (vecNor.x * PosA.x)) + ((vecNor.z * pPos->z) - (vecNor.z * PosA.z)) / vecNor.y);

				if (pPos->y <= fPosY)
				{
					pPos->y = fPosY;
				}

				//// 頂点インデックスバッファのアンロック
				//g_amodeldata[pModel->modeltype].pMesh->UnlockIndexBuffer();

				//// 頂点インデックスバッファのアンロック
				//g_amodeldata[pModel->modeltype].pMesh->UnlockIndexBuffer();
			}
		}


		// 頂点インデックスバッファのアンロック
		g_amodeldata[pModel->modeltype].pMesh->UnlockIndexBuffer();

		// 頂点インデックスバッファのアンロック
		g_amodeldata[pModel->modeltype].pMesh->UnlockIndexBuffer();
	}
}