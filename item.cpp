//=============================================================================
//
//	アイテム処理 [item.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "item.h"
#include "input.h"
#include "particle.h"
#include "sound.h"

#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_ITEM		(16)			// アイテムの最大数
#define ITEM_RADIUS		(45.0f)			// アイテムの半径

//*****************************************************************************
// グローバル変数
//*****************************************************************************
Itemdata g_aitemdata[ITEMTYPE_MAX];	// アイテムデータの情報
Item g_aitem[MAX_ITEM];				// アイテムの情報
ITEMTYPE g_CurrentItem;				// 今の所持アイテム
int g_nNumItemData;					// アイテムデータ数
int g_nNumItem;						// 現在のアイテム数

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
		g_aitem[nCntItem].fRadius = ITEM_RADIUS;
		g_aitem[nCntItem].fAngle = 0.0f;
		g_aitem[nCntItem].bUse = false;
	}

	g_CurrentItem = ITEMTYPE_MAX;
	g_nNumItem = 0;
	g_nNumItemData = 0;
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
	Item* pItem = &g_aitem[0];						// 先頭アドレス

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pItem++)
	{
		if (pItem->bUse == false)
		{
			continue;
		}

		pItem->fAngle += ITEM_FLOATINGSPEED;

		pItem->fAngle = AngleNormalize(pItem->fAngle);

		pItem->pos.y += sinf(pItem->fAngle) * 0.5f;

		pItem->rot.y += ITEM_FLOATINGSPEED;

		pItem->rot.y = AngleNormalize(pItem->rot.y);

		//SetParticle(pItem->pos, COLOR_WHITE, 50.0f, 0.0025f, 0.0025f, 5, D3DX_PI, -D3DX_PI);
	}
}

//=============================================================================
//	アイテムの当たり判定処理
//=============================================================================
void CollisionItem(D3DXVECTOR3* pPos, float fRadius)
{
	Item* pItem = &g_aitem[0];						// 先頭アドレス

	float fDiff = 0.0f;

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pItem++)
	{
		if (pItem->bUse == false)
		{// 使用していなかったら戻る
			continue;
		}

		Itemdata* pItemData = &g_aitemdata[pItem->itemtype];	// アイテムタイプ

		fDiff = powf(pItem->pos.x - pPos->x, 2) + powf(pItem->pos.y - pPos->y, 2) + powf(pItem->pos.z - pPos->z, 2);

		if (fDiff <= powf(fRadius + pItem->fRadius, 2))
		{
			SetJoypadVibration(10000, 16000, 30);
			PlaySound(SOUND_LABEL_GETSE000);
			g_CurrentItem = pItem->itemtype;
			pItem->bUse = false;
		}
	}
}

//=============================================================================
//	アイテムの設定処理
//=============================================================================
void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 rot, ITEMTYPE itemtype, bool bCollision)
{
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aitem[nCntItem].bUse == true)
		{
			continue;
		}

		g_aitem[nCntItem].pos = pos;
		g_aitem[nCntItem].rot = rot;
		g_aitem[nCntItem].itemtype = itemtype;
		g_aitem[nCntItem].bCollision = bCollision;
		g_aitem[nCntItem].bUse = true;

		g_nNumItem++;
		break;
	}
}

//=============================================================================
//	アイテムの取得処理
//=============================================================================
ITEMTYPE GetItem(void)
{
	return g_CurrentItem;
}

//=============================================================================
//	アイテムの設定処理
//=============================================================================
void SetCurrentItem(void)
{
	g_CurrentItem = ITEMTYPE_MAX;
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