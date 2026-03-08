//=============================================================================
//
//	背景処理 [bg.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "bg.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_BG		(3)								// 背景の最大数

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureBg = NULL;				// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBg = NULL;		// 頂点バッファへのポインタ
Bg g_bg;

//=============================================================================
//	背景の初期化処理
//=============================================================================
void InitBg(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\bg000.jpg",
		&g_pTextureBg);


	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BG,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBg,
		NULL);

	// 初期化

	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffBg->Lock(0, 0, (void * *)&pVtx, 0);

		g_bg.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_bg.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_bg.pos.x, g_bg.pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_bg.pos.x + SCREEN_WIDTH, g_bg.pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_bg.pos.x, g_bg.pos.x + SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_bg.pos.x + SCREEN_WIDTH, g_bg.pos.x + SCREEN_HEIGHT, 0.0f);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);


	// 頂点バッファをアンロックする
	g_pVtxBuffBg->Unlock();
}

//=============================================================================
//	背景の終了処理
//=============================================================================
void UninitBg(void)
{
	// テクスチャの破棄
	if (g_pTextureBg != NULL)
	{
		g_pTextureBg->Release();
		g_pTextureBg = NULL;
	}
	

	// 頂点バッファの破棄
	if (g_pVtxBuffBg != NULL)
	{
		g_pVtxBuffBg->Release();
		g_pVtxBuffBg = NULL;
	}
}

//=============================================================================
//	背景の描画処理
//=============================================================================
void DrawBg(void)
{
	LPDIRECT3DDEVICE9 pDevice;				// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBg, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureBg);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
//	背景の更新処理
//=============================================================================
void UpdateBg(void)
{
	int nCntBg;

	// 頂点座標の更新
	VERTEX_2D* pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffBg->Lock(0, 0, (void**)&pVtx, 0);


	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_bg.pos.x, g_bg.pos.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_bg.pos.x + SCREEN_WIDTH, g_bg.pos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_bg.pos.x, g_bg.pos.y + SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_bg.pos.x + SCREEN_WIDTH, g_bg.pos.y + SCREEN_HEIGHT, 0.0f);


	// 頂点バッファをアンロックする
	g_pVtxBuffBg->Unlock();
}