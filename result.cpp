//=============================================================================
//
//	リザルト画面処理 [result.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "result.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "game.h"
#include "resultmenu.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_RESULT			(2)			// リザルトテクスチャの枚数
#define RESULTFADE_TIMER	(300)		// タイマーの秒数

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureResult[MAX_RESULT] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;		// 頂点バッファへのポインタ
int g_nResultFadeCounter = NULL;

//====================================
//	リザルトの初期化処理
//====================================
void InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;				// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\gameoverbg.jpg",
		&g_pTextureResult[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\gameoverbg.jpg",
		&g_pTextureResult[1]);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResult,
		NULL);

	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void * *)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 720.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

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

	// 頂点バッファをアンロックする
	g_pVtxBuffResult->Unlock();

	g_nResultFadeCounter = 0;

	InitResultMenu();
}

//====================================
//	リザルトの終了処理
//====================================
void UninitResult(void)
{
	// サウンドを止める
	StopSound();

	for (int nCntResult = 0; nCntResult < MAX_RESULT; nCntResult++)
	{
		// テクスチャの破棄
		if (g_pTextureResult[nCntResult] != NULL)
		{
			g_pTextureResult[nCntResult]->Release();
			g_pTextureResult[nCntResult] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}

	UninitResultMenu();
}

//====================================
//	リザルトの描画処理
//====================================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;				// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	switch (GetNextGameState())
	{
	case GAMESTATE_CLEAR:
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureResult[0]);
		break;

	case GAMESTATE_GAMEOVER:
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureResult[1]);
		break;
	}

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	DrawResultMenu();
}

//====================================
//	リザルトの更新処理
//====================================
void UpdateResult(void)
{

	UpdateResultMenu();

}