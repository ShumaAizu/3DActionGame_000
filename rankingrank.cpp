//=============================================================================
//
//	ランキング画面処理 [rankingrank.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "ranking.h"
#include "rankingrank.h"
#include "input.h"
#include "sound.h"
#include "fade.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define RANKINGRANKFADE_TIMER		(300)		// タイマーの秒数

//*****************************************************************************
// ランキング構造体
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;			// 位置
	int nScore;					// スコア
	bool bDisp;					// 表示状態
}RankingRank;

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureRankingRank = NULL;				// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRankingRank = NULL;			// 頂点バッファへのポインタ
RankingRank g_arankingrank[MAX_RANK] = {};						// ランキング情報

//====================================
//	ランキングの初期化処理
//====================================
void InitRankingRank(void)
{

	LPDIRECT3DDEVICE9 pDevice;				// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Rank.png",
		&g_pTextureRankingRank);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_RANK,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRankingRank,
		NULL);

	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffRankingRank->Lock(0, 0, (void * *)&pVtx, 0);

	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{

		g_arankingrank[nCntRank].pos = D3DXVECTOR3(RANKINGRANK_POSX, RANKINGRANK_POSY + (nCntRank * (RANKINGRANK_SIZEY + RANKINGRANK_DIFF)), 0.0f);
		g_arankingrank[nCntRank].bDisp = true;

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_arankingrank[nCntRank].pos.x, g_arankingrank[nCntRank].pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_arankingrank[nCntRank].pos.x + RANKINGRANK_SIZEX, g_arankingrank[nCntRank].pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_arankingrank[nCntRank].pos.x, g_arankingrank[nCntRank].pos.y + RANKINGRANK_SIZEY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_arankingrank[nCntRank].pos.x + RANKINGRANK_SIZEX, g_arankingrank[nCntRank].pos.y + RANKINGRANK_SIZEY, 0.0f);

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
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.2f * nCntRank);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.2f * nCntRank);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 0.2f * (nCntRank + 1));
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.2f * (nCntRank + 1));

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffRankingRank->Unlock();
}

//====================================
//	ランキングの終了処理
//====================================
void UninitRankingRank(void)
{

	// テクスチャの破棄
	if (g_pTextureRankingRank != NULL)
	{
		g_pTextureRankingRank->Release();
		g_pTextureRankingRank = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffRankingRank != NULL)
	{
		g_pVtxBuffRankingRank->Release();
		g_pVtxBuffRankingRank = NULL;
	}
}

//====================================
//	ランキングの描画処理
//====================================
void DrawRankingRank(void)
{

	LPDIRECT3DDEVICE9 pDevice;				// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRankingRank, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureRankingRank);

	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		if (g_arankingrank[nCntRank].bDisp == true)
		{
			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntRank * 4, 2);
		}
	}
}

//====================================
//	ランキングの更新処理
//====================================
void UpdateRankingRank(void)
{

}