//=============================================================================
//
//	雪処理 [snow.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "snow.h"
#include "input.h"
#include "shadow.h"
#include "camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_SNOW				(1024)			// 雪の最大数

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureSnow = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSnow = NULL;	// 頂点バッファへのポインタ
Snow g_asnow[MAX_SNOW];							// 雪の情報

//=============================================================================
//	雪の初期化処理
//=============================================================================
void InitSnow(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\snow000.png",
		&g_pTextureSnow);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_SNOW * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSnow,
		NULL);

	// 初期化
	VERTEX_3D *pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffSnow->Lock(0, 0, (void * *)&pVtx, 0);

	for (int nCntSnow = 0; nCntSnow < MAX_SNOW; nCntSnow++)
	{
		g_asnow[nCntSnow].pos = D3DXVECTOR3(0.0f, 100.0f, 300.0f);
		g_asnow[nCntSnow].fRadius = 0.0f;
		g_asnow[nCntSnow].bUse = false;

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-g_asnow[nCntSnow].fRadius, g_asnow[nCntSnow].fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_asnow[nCntSnow].fRadius, g_asnow[nCntSnow].fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_asnow[nCntSnow].fRadius, -g_asnow[nCntSnow].fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_asnow[nCntSnow].fRadius, -g_asnow[nCntSnow].fRadius, 0.0f);

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
	g_pVtxBuffSnow->Unlock();

	SetSnow(D3DXVECTOR3(-50.0f, 30.0f, 10.0f), 30.0f);
	SetSnow(D3DXVECTOR3(50.0f, 10.0f, 10.0f), 10.0f);
}

//=============================================================================
//	雪の終了処理
//=============================================================================
void UninitSnow(void)
{
	// テクスチャの破棄
	if (g_pTextureSnow != NULL)
	{
		g_pTextureSnow->Release();
		g_pTextureSnow = NULL;
	}
	

	// 頂点バッファの破棄
	if (g_pVtxBuffSnow != NULL)
	{
		g_pVtxBuffSnow->Release();
		g_pVtxBuffSnow = NULL;
	}
}

//=============================================================================
//	雪の描画処理
//=============================================================================
void DrawSnow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス
	D3DXMATRIX mtxView;							// ビューマトリックス

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// 比較方法(基準値より大きければ描画)
	pDevice->SetRenderState(D3DRS_ALPHAREF, 175);				// アルファテストの参照値を設定(～以上で描画, intで設定)

	for (int nCntSnow = 0; nCntSnow < MAX_SNOW; nCntSnow++)
	{
		if (g_asnow[nCntSnow].bUse == false)
		{
			continue;
		}

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_asnow[nCntSnow].mtxWorld);

		// ビューマトリックスの取得
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		// ポリゴンとカメラに対して正面を向ける
		D3DXMatrixInverse(&g_asnow[nCntSnow].mtxWorld, NULL, &mtxView);	// 逆行列を求める
		g_asnow[nCntSnow].mtxWorld._41 = 0.0f;
		g_asnow[nCntSnow].mtxWorld._42 = 0.0f;
		g_asnow[nCntSnow].mtxWorld._43 = 0.0f;

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_asnow[nCntSnow].pos.x, g_asnow[nCntSnow].pos.y, g_asnow[nCntSnow].pos.z);
		D3DXMatrixMultiply(&g_asnow[nCntSnow].mtxWorld, &g_asnow[nCntSnow].mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_asnow[nCntSnow].mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffSnow, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureSnow);

		// 雪の描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntSnow * 4, 2);
	}

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// アルファテストを無効化
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);	// 比較方法(すべて描画)
	pDevice->SetRenderState(D3DRS_ALPHAREF, 255);				// 基準値を設定(すべてを描画している)
}

//=============================================================================
//	雪の更新処理
//=============================================================================
void UpdateSnow(void)
{
	for (int nCntSnow = 0; nCntSnow < MAX_SNOW; nCntSnow++)
	{
		if (g_asnow[nCntSnow].bUse == false)
		{
			continue;
		}

		SetPositionShadow(g_asnow[nCntSnow].nIdxShadow, D3DXVECTOR3(g_asnow[nCntSnow].pos.x, g_asnow[nCntSnow].pos.y - g_asnow[nCntSnow].pos.y, g_asnow[nCntSnow].pos.z));

	}

}

//=============================================================================
//	雪の設定処理
//=============================================================================
void SetSnow(D3DXVECTOR3 pos, float fRadius)
{
	VERTEX_3D* pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffSnow->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntSnow = 0; nCntSnow < MAX_SNOW; nCntSnow++, pVtx += 4)
	{
		if (g_asnow[nCntSnow].bUse == true)
		{
			continue;
		}

		g_asnow[nCntSnow].pos = pos;
		g_asnow[nCntSnow].fRadius = fRadius;

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-g_asnow[nCntSnow].fRadius, g_asnow[nCntSnow].fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_asnow[nCntSnow].fRadius, g_asnow[nCntSnow].fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_asnow[nCntSnow].fRadius, -g_asnow[nCntSnow].fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_asnow[nCntSnow].fRadius, -g_asnow[nCntSnow].fRadius, 0.0f);

		g_asnow[nCntSnow].nIdxShadow = SetShadow();

		g_asnow[nCntSnow].bUse = true;
		break;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffSnow->Unlock();
}