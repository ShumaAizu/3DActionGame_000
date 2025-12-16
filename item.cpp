//=============================================================================
//
//	アイテム処理 [item.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "item.h"
#include "input.h"

#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_ITEM		(128)			// アイテムの最大数

//*****************************************************************************
// グローバル変数
//*****************************************************************************
Itemdata g_aitemdata[ITEMTYPE_MAX];	// アイテムデータの情報
Item g_aitem[MAX_ITEM];				// アイテムの情報
int g_nSelectItem;					// 選択中のアイテム
int g_nNumItemData;					// アイテムデータ数
int g_nNumItem;						// 現在のアイテム数
int g_nSelectItemType;				// 選択中の種類
float g_fItemMove;					// アイテムの移動量

//=============================================================================
//	アイテムの初期化処理
//=============================================================================
void InitItem(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		g_aitem[nCntItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aitem[nCntItem].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aitem[nCntItem].bCollision = true;
		g_aitem[nCntItem].bUse = false;
	}

	g_nNumItem = -1;
	g_nNumItemData = 0;
	g_nSelectItem = -1;
	g_nSelectItemType = 0;
	g_fItemMove = 1.0f;
}

//=============================================================================
//	アイテムの終了処理
//=============================================================================
void UninitItem(void)
{
	for (int nCntItemdata = 0; nCntItemdata < ITEMTYPE_MAX; nCntItemdata++)
	{
		// テクスチャの破棄
		for (int nCntTex = 0; nCntTex < MAX_ITEMTEX; nCntTex++)
		{
			if (g_aitemdata[nCntItemdata].apTexture[nCntTex] != NULL)
			{
				g_aitemdata[nCntItemdata].apTexture[nCntTex]->Release();
				g_aitemdata[nCntItemdata].apTexture[nCntTex] = NULL;
			}
		}

		// メッシュの破棄
		if (g_aitemdata[nCntItemdata].pMesh != NULL)
		{
			g_aitemdata[nCntItemdata].pMesh->Release();
			g_aitemdata[nCntItemdata].pMesh = NULL;
		}

		// マテリアルの破棄
		if (g_aitemdata[nCntItemdata].pBuffMat != NULL)
		{
			g_aitemdata[nCntItemdata].pBuffMat->Release();
			g_aitemdata[nCntItemdata].pBuffMat = NULL;
		}
	}
}

//=============================================================================
//	アイテムの描画処理
//=============================================================================
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;		// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用
	D3DXMATERIAL* pMat;							// マテリアルデータへのポインタ

	Item* pItem = &g_aitem[0];

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pItem++)
	{
		if (pItem->bUse == false)
		{// 使われていなければ戻る
			continue;
		}
		
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pItem->mtxWorld);

		D3DXMatrixScaling(&mtxScale, pItem->scale.y, pItem->scale.x, pItem->scale.z);
		D3DXMatrixMultiply(&pItem->mtxWorld, &pItem->mtxWorld, &mtxScale);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pItem->rot.y, pItem->rot.x, pItem->rot.z);
		D3DXMatrixMultiply(&pItem->mtxWorld, &pItem->mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, pItem->pos.x, pItem->pos.y, pItem->pos.z);
		D3DXMatrixMultiply(&pItem->mtxWorld, &pItem->mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &pItem->mtxWorld);

		// 現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_aitemdata[pItem->itemtype].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aitemdata[pItem->itemtype].dwNumMat; nCntMat++)
		{
			// マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_aitemdata[pItem->itemtype].apTexture[nCntMat]);

			// アイテム(パーツ)の描画
			g_aitemdata[pItem->itemtype].pMesh->DrawSubset(nCntMat);
		}

		// 保存していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}
}

//=============================================================================
//	アイテムの更新処理
//=============================================================================
void UpdateItem(void)
{
	//if (GetKeyboardTrigger(DIK_F2) == true)
	//{
	//	g_nSelectItem = (g_nSelectItem + 1) % (g_nNumItem + 1);
	//}

	//if (GetKeyboardTrigger(DIK_F3) == true)
	//{
	//	g_nSelectType = (g_nSelectType + 1) % ITEMTYPE_MAX;
	//}

	//if (GetKeyboardRepeat(DIK_F4) == true)
	//{
	//	g_fItemMove += -1.0f;
	//}

	//if (GetKeyboardRepeat(DIK_F5) == true)
	//{
	//	g_fItemMove += 1.0f;
	//}

	//if (GetKeyboardTrigger(DIK_RETURN) == true)
	//{
	//	SetItem(INIT_D3DXVEC3, INIT_D3DXVEC3, D3DXVECTOR3(1.0f, 1.0f, 1.0f), (ITEMTYPE)g_nSelectType);
	//}

	//if (GetKeyboardPress(DIK_T) == true)
	//{
	//	g_aitem[g_nSelectItem].pos.z += g_fItemMove;
	//}

	//if (GetKeyboardPress(DIK_F) == true)
	//{
	//	g_aitem[g_nSelectItem].pos.x += -g_fItemMove;
	//}

	//if (GetKeyboardPress(DIK_G) == true)
	//{
	//	g_aitem[g_nSelectItem].pos.z += -g_fItemMove;
	//}

	//if (GetKeyboardPress(DIK_H) == true)
	//{
	//	g_aitem[g_nSelectItem].pos.x += g_fItemMove;
	//}

	//if (GetKeyboardPress(DIK_X) == true)
	//{
	//	g_aitem[g_nSelectItem].pos.y += g_fItemMove;
	//}

	//if (GetKeyboardPress(DIK_V) == true)
	//{
	//	g_aitem[g_nSelectItem].pos.y += -g_fItemMove;
	//}

	//if (GetKeyboardPress(DIK_Y) == true)
	//{
	//	g_aitem[g_nSelectItem].rot.y += 0.1f;
	//	g_aitem[g_nSelectItem].rot.y = AngleNormalize(g_aitem[g_nSelectItem].rot.y);
	//}

	//if (GetKeyboardPress(DIK_R) == true)
	//{
	//	g_aitem[g_nSelectItem].rot.y += -0.1f;
	//	g_aitem[g_nSelectItem].rot.y = AngleNormalize(g_aitem[g_nSelectItem].rot.y);
	//}

	//if (GetKeyboardPress(DIK_UPARROW) == true)
	//{
	//	g_aitem[g_nSelectItem].scale.x += 0.1f;
	//	g_aitem[g_nSelectItem].scale.y += 0.1f;
	//	g_aitem[g_nSelectItem].scale.z += 0.1f;
	//}

	//if (GetKeyboardPress(DIK_DOWNARROW) == true)
	//{
	//	g_aitem[g_nSelectItem].scale.x += -0.1f;
	//	g_aitem[g_nSelectItem].scale.y += -0.1f;
	//	g_aitem[g_nSelectItem].scale.z += -0.1f;
	//}

	//PrintDebugProc("選択アイテム = { %d }\n", g_nSelectItem);
	//PrintDebugProc("アイテム数 = { %d }\n", g_nNumItem);
	//PrintDebugProc("選択タイプ = { %d }\n", g_nSelectType);
	//PrintDebugProc("アイテム移動量 = { %.2f }\n", g_fItemMove);
	//PrintDebugProc("ItemPos = { %.2f %.2f %.2f }\n", g_aitem[g_nSelectItem].pos.x, g_aitem[g_nSelectItem].pos.y, g_aitem[g_nSelectItem].pos.z);
	//PrintDebugProc("ItemRot = { %.2f %.2f %.2f }\n", g_aitem[g_nSelectItem].rot.x, g_aitem[g_nSelectItem].rot.y, g_aitem[g_nSelectItem].rot.z);
	//PrintDebugProc("ItemScale = { %.2f %.2f %.2f }\n", g_aitem[g_nSelectItem].scale.x, g_aitem[g_nSelectItem].scale.y, g_aitem[g_nSelectItem].scale.z);
}

//=============================================================================
//	アイテムの当たり判定処理
//=============================================================================
void CollisionItem(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 * pMove, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax)
{
	Item* pItem = &g_aitem[0];					// 先頭アドレス
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;			// 計算用マトリックス
	D3DXVECTOR3 posMax, posMin;						// 
	D3DXVECTOR3 posA, posB, posC, posD;

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pItem++)
	{
		if (g_aitem[nCntItem].bUse == false)
		{// 使用していなかったら戻る
			continue;
		}

		Itemdata* pItemData = &g_aitemdata[pItem->itemtype];	// アイテムタイプ

		posA = D3DXVECTOR3(pItemData->vtxMin.x, 0.0f, pItemData->vtxMax.z);
		posB = D3DXVECTOR3(pItemData->vtxMax.x, 0.0f, pItemData->vtxMax.z);
		posC = D3DXVECTOR3(pItemData->vtxMax.x, 0.0f, pItemData->vtxMin.z);
		posD = D3DXVECTOR3(pItemData->vtxMin.x, 0.0f, pItemData->vtxMin.z);

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pItem->mtxWorld);

		D3DXMatrixScaling(&mtxScale, pItem->scale.y, pItem->scale.x, pItem->scale.z);
		D3DXMatrixMultiply(&pItem->mtxWorld, &pItem->mtxWorld, &mtxScale);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pItem->rot.y, pItem->rot.x, pItem->rot.z);
		D3DXMatrixMultiply(&pItem->mtxWorld, &pItem->mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, pItem->pos.x, pItem->pos.y, pItem->pos.z);
		D3DXMatrixMultiply(&pItem->mtxWorld, &pItem->mtxWorld, &mtxTrans);

		// 位置と向きを反映した頂点座標を入れる
		D3DXVec3TransformCoord(&posA, &posA, &pItem->mtxWorld);
		D3DXVec3TransformCoord(&posB, &posB, &pItem->mtxWorld);
		D3DXVec3TransformCoord(&posC, &posC, &pItem->mtxWorld);
		D3DXVec3TransformCoord(&posD, &posD, &pItem->mtxWorld);

		// 当たり判定
		if (pItem->bCollision == true)
		{
			CrossCollision(pPos, pPosOld, posB, posA, true, false);
			CrossCollision(pPos, pPosOld, posC, posB, true, false);
			CrossCollision(pPos, pPosOld, posD, posC, true, false);
			CrossCollision(pPos, pPosOld, posA, posD, true, false);
		}

		// アイテムの範囲内か判定
		if (pPos->x + vtxMax.x > g_aitem[nCntItem].pos.x + g_aitemdata[g_aitem[nCntItem].itemtype].vtxMin.x && pPos->x + vtxMin.x < g_aitem[nCntItem].pos.x + g_aitemdata[g_aitem[nCntItem].itemtype].vtxMax.x &&
			pPos->y + vtxMax.y > g_aitem[nCntItem].pos.y +  g_aitemdata[g_aitem[nCntItem].itemtype].vtxMin.y && pPos->y + vtxMin.y < g_aitem[nCntItem].pos.y +  g_aitemdata[g_aitem[nCntItem].itemtype].vtxMax.y &&
			pPos->z + vtxMax.z > g_aitem[nCntItem].pos.z +  g_aitemdata[g_aitem[nCntItem].itemtype].vtxMin.z && pPos->z + vtxMin.z < g_aitem[nCntItem].pos.z +  g_aitemdata[g_aitem[nCntItem].itemtype].vtxMax.z)
		{

			// 上から
			if (pPosOld->y + vtxMin.y >= g_aitem[nCntItem].pos.y + g_aitemdata[g_aitem[nCntItem].itemtype].vtxMax.y)
			{
				pPos->y = g_aitem[nCntItem].pos.y + g_aitemdata[g_aitem[nCntItem].itemtype].vtxMax.y - vtxMin.y;
			}

			// 下から
			if (pPosOld->y + vtxMax.y <= g_aitem[nCntItem].pos.y + g_aitemdata[g_aitem[nCntItem].itemtype].vtxMin.y)
			{
				pPos->y = g_aitem[nCntItem].pos.y + g_aitemdata[g_aitem[nCntItem].itemtype].vtxMin.y - vtxMax.y;
			}
		}
	}
}

//=============================================================================
//	アイテムの設定処理
//=============================================================================
void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, ITEMTYPE itemtype)
{
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aitem[nCntItem].bUse == true)
		{
			continue;
		}

		g_nNumItem++;
		g_nSelectItem++;

		g_aitem[nCntItem].pos = pos;
		g_aitem[nCntItem].rot = rot;
		g_aitem[nCntItem].scale = scale;
		g_aitem[nCntItem].itemtype = itemtype;
		g_aitem[nCntItem].bUse = true;
		break;
	}
}

//=============================================================================
//	アイテムの読み込み処理
//=============================================================================
void LoadItemData(const char* pItemFile)
{
	Itemdata* pItem = &g_aitemdata[g_nNumItemData];

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	BYTE* pVtxBuff;								// 頂点バッファへのポインタ

	// Xファイルの読み込み
	D3DXLoadMeshFromX(pItemFile,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&pItem->pBuffMat,
		NULL,
		&pItem->dwNumMat,
		&pItem->pMesh);

	D3DXMATERIAL* pMat;

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)pItem->pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)pItem->dwNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{// テクスチャファイルが存在する
			// テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,
				&pItem->apTexture[nCntMat]);
		}
	}

	// 頂点数を取得
	pItem->nNumVtx = pItem->pMesh->GetNumVertices();

	// インデックス数を取得
	pItem->nNumIdx = pItem->pMesh->GetNumFaces();

	// 頂点フォーマットのサイズを取得
	pItem->dwSizeFVF = D3DXGetFVFVertexSize(pItem->pMesh->GetFVF());

	// 頂点バッファをロック
	pItem->pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < pItem->nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// 頂点情報の代入

		if (vtx.x > pItem->vtxMax.x)
		{
			pItem->vtxMax.x = vtx.x;
		}

		if (vtx.y > pItem->vtxMax.y)
		{
			pItem->vtxMax.y = vtx.y;
		}

		if (vtx.z > pItem->vtxMax.z)
		{
			pItem->vtxMax.z = vtx.z;
		}

		if (vtx.x < pItem->vtxMin.x)
		{
			pItem->vtxMin.x = vtx.x;
		}

		if (vtx.y < pItem->vtxMin.y)
		{
			pItem->vtxMin.y = vtx.y;
		}

		if (vtx.z < pItem->vtxMin.z)
		{
			pItem->vtxMin.z = vtx.z;
		}

		pVtxBuff += pItem->dwSizeFVF;		// 頂点フォーマットのサイズ分ポインタを進める
	}

	// 頂点バッファのアンロック
	pItem->pMesh->UnlockVertexBuffer();

	g_nNumItemData++;
}