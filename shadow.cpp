//=============================================================================
//
//	影の処理 [shadow.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "shadow.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureShadow = NULL;			// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;	// 頂点バッファへのポインタ
Shadow g_ashadow[MAX_SHADOW] = {};					// 影の情報

//=============================================================================
//	影の初期化処理
//=============================================================================
void InitShadow(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\shadow000.jpg",
		&g_pTextureShadow);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		g_ashadow[nCntShadow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_ashadow[nCntShadow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_ashadow[nCntShadow].bUse = false;
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_SHADOW,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffShadow,
		NULL);

	// 初期化
	VERTEX_3D *pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffShadow->Lock(0, 0, (void * *)&pVtx, 0);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_ashadow[nCntShadow].pos.x - SHADOW_WIDTH, g_ashadow[nCntShadow].pos.y, g_ashadow[nCntShadow].pos.z + SHADOW_HEIGHT);
		pVtx[1].pos = D3DXVECTOR3(g_ashadow[nCntShadow].pos.x + SHADOW_WIDTH, g_ashadow[nCntShadow].pos.y, g_ashadow[nCntShadow].pos.z + SHADOW_HEIGHT);
		pVtx[2].pos = D3DXVECTOR3(g_ashadow[nCntShadow].pos.x - SHADOW_WIDTH, g_ashadow[nCntShadow].pos.y, g_ashadow[nCntShadow].pos.z - SHADOW_HEIGHT);
		pVtx[3].pos = D3DXVECTOR3(g_ashadow[nCntShadow].pos.x + SHADOW_WIDTH, g_ashadow[nCntShadow].pos.y, g_ashadow[nCntShadow].pos.z - SHADOW_HEIGHT);

		// 法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}


	// 頂点バッファをアンロックする
	g_pVtxBuffShadow->Unlock();
}

//=============================================================================
//	影の終了処理
//=============================================================================
void UninitShadow(void)
{
	// テクスチャの破棄
	if (g_pTextureShadow != NULL)
	{
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;
	}
	

	// 頂点バッファの破棄
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
}

//=============================================================================
//	影の描画処理
//=============================================================================
void DrawShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス

	// 減算合成の設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//// Zテストを無効にする
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_ashadow[nCntShadow].mtxWorld);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_ashadow[nCntShadow].rot.y, g_ashadow[nCntShadow].rot.x, g_ashadow[nCntShadow].rot.z);
		D3DXMatrixMultiply(&g_ashadow[nCntShadow].mtxWorld, &g_ashadow[nCntShadow].mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_ashadow[nCntShadow].pos.x, g_ashadow[nCntShadow].pos.y, g_ashadow[nCntShadow].pos.z);
		D3DXMatrixMultiply(&g_ashadow[nCntShadow].mtxWorld, &g_ashadow[nCntShadow].mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_ashadow[nCntShadow].mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureShadow);

		if (g_ashadow[nCntShadow].bUse == true)
		{
			// 影の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntShadow * 4, 2);
		}
	}

	// 通常の合成に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//// Zテストを無効にする
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

}

//=============================================================================
//	影の更新処理
//=============================================================================
void UpdateShadow(void)
{

}

//=============================================================================
//	影の設定処理
//=============================================================================
int SetShadow(void)
{
	int nCntShadow = -1;
	for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_ashadow[nCntShadow].bUse == false)
		{
			// 影の設定
			g_ashadow[nCntShadow].bUse = true;
			break;
		}
	}
	return nCntShadow;	// この値が影をつけたいオブジェクトのID
}

//=============================================================================
//	影の処理
//=============================================================================
void SetPositionShadow(int IdxShadow, D3DXVECTOR3 pos)
{
	g_ashadow[IdxShadow].pos = pos;
}