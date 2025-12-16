//=============================================================================
//
//	ランキング画面処理 [ranking.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "ranking.h"
#include "game.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "rankingrank.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define RANKINGFADE_TIMER		(300)		// タイマーの秒数

//*****************************************************************************
// ランキング構造体
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;			// 位置
	int nScore;					// スコア
	bool bDisp;					// 表示状態
}Ranking;

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureRanking = NULL;				// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRanking = NULL;			// 頂点バッファへのポインタ
Ranking g_aranking[MAX_RANK] = {};							// ランキング情報
int g_nRankUpdate = -1;										// 更新ランクNo.
int g_nRankingFadeCounter = 0;
bool g_bRanking_Modechange = false;

//====================================
//	ランキングの初期化処理
//====================================
void InitRanking(void)
{
	InitRankingRank();

	LPDIRECT3DDEVICE9 pDevice;				// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\number001.png",
		&g_pTextureRanking);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * (MAX_RANK * MAX_NUM),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRanking,
		NULL);

	g_bRanking_Modechange = false;

	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffRanking->Lock(0, 0, (void * *)&pVtx, 0);

	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		int aTexU[MAX_NUM] = {};

		g_aranking[nCntRank].pos = D3DXVECTOR3(RANKING_POSX, RANKING_POSY + (nCntRank * (RANKING_SIZEY + RANKING_DIFF)), 0.0f);
		g_aranking[nCntRank].bDisp = true;

		aTexU[0] = g_aranking[nCntRank].nScore % 100000 / 10000;
		aTexU[1] = g_aranking[nCntRank].nScore % 10000 / 1000;
		aTexU[2] = g_aranking[nCntRank].nScore % 1000 / 100;
		aTexU[3] = g_aranking[nCntRank].nScore % 100 / 10;
		aTexU[4] = g_aranking[nCntRank].nScore % 10 / 1;

		for (int nCntNum = 0; nCntNum < MAX_NUM; nCntNum++)
		{
			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aranking[nCntRank].pos.x + (RANKING_SIZEX * nCntNum), g_aranking[nCntRank].pos.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aranking[nCntRank].pos.x + (RANKING_SIZEX * (nCntNum + 1)), g_aranking[nCntRank].pos.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aranking[nCntRank].pos.x + (RANKING_SIZEX * nCntNum), g_aranking[nCntRank].pos.y + RANKING_SIZEY, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aranking[nCntRank].pos.x + (RANKING_SIZEX * (nCntNum + 1)), g_aranking[nCntRank].pos.y + RANKING_SIZEY, 0.0f);

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
			pVtx[0].tex = D3DXVECTOR2(0.1f * aTexU[nCntNum], 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f * (aTexU[nCntNum] + 1), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.1f * aTexU[nCntNum], 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f * (aTexU[nCntNum] + 1), 1.0f);

			pVtx += 4;

		}
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffRanking->Unlock();

	g_nRankingFadeCounter = 0;
}

//====================================
//	ランキングの終了処理
//====================================
void UninitRanking(void)
{
	// サウンドを止める
	StopSound();

	UninitRankingRank();

	SetGameState(GAMESTATE_NORMAL, 60);

	g_nRankUpdate = -1;

	// テクスチャの破棄
	if (g_pTextureRanking != NULL)
	{
		g_pTextureRanking->Release();
		g_pTextureRanking = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffRanking != NULL)
	{
		g_pVtxBuffRanking->Release();
		g_pVtxBuffRanking = NULL;
	}
}

//====================================
//	ランキングの描画処理
//====================================
void DrawRanking(void)
{
	DrawRankingRank();

	LPDIRECT3DDEVICE9 pDevice;				// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRanking, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureRanking);

	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		if (g_aranking[nCntRank].bDisp == true)
		{
			for (int nCntNum = 0; nCntNum < MAX_NUM; nCntNum++)
			{
				// ポリゴンの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, ((nCntRank * MAX_NUM) + nCntNum) * 4, 2);
			}
		}
	}
}

//====================================
//	ランキングの更新処理
//====================================
void UpdateRanking(void)
{
	UpdateRankingRank();

	g_nRankingFadeCounter++;
	static int nRankDispCounter = 0;

	if (g_nRankUpdate != -1)
	{
		nRankDispCounter++;
		if (nRankDispCounter % 10 == 0)
		{
			g_aranking[g_nRankUpdate].bDisp = g_aranking[g_nRankUpdate].bDisp ? false : true;
		}
	}

	if ((GetKeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_A) == true) && GetFade() != FADE_OUT)
	{// 決定キーが押された
		// モード設定
		if (g_bRanking_Modechange == false)
		{

		}
		g_bRanking_Modechange = true;
		SetFade(MODE_TITLE, 0.025f, 0.025f);
	}

	if (g_nRankingFadeCounter >= RANKINGFADE_TIMER && GetFade() != FADE_OUT)
	{
		SetFade(MODE_TITLE, 0.025f, 0.025f);
	}
}

//====================================
//	ランキングのリセット処理
//====================================
void ResetRanking(void)
{
	int nScoreRank[MAX_RANK] = {};

	FILE* pFile;
#ifdef _DEBUG
	pFile = fopen("data\\Rankingtest.bin", "rb");
#else
	pFile = fopen("data\\Ranking001.bin", "rb");
#endif

	if (pFile == NULL)
	{
		for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
		{
			nScoreRank[nCntRank] = (nCntRank + 1) * 2500;
		}

		qsort(&nScoreRank[0], MAX_RANK, sizeof(int), Sort_desc);		// ソート

		for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
		{
			g_aranking[nCntRank].nScore = nScoreRank[nCntRank];
		}
		return;
	}

	fread(&nScoreRank[0], sizeof(int), MAX_RANK, pFile);

	fclose(pFile);

	for (int nCntScoreRank = 0; nCntScoreRank < MAX_RANK; nCntScoreRank++)
	{
		g_aranking[nCntScoreRank].nScore = nScoreRank[nCntScoreRank];
	}
}

//====================================
//	ランキングの設定処理
//====================================
void SetRanking(int nScore)
{
	int nCntScoreRank = 0;
	int nScoreRank[MAX_RANK + 1] = {};		// 新しいスコアを含めたコピー配列

	// 全部コピー
	for (nCntScoreRank = 0; nCntScoreRank < MAX_RANK; nCntScoreRank++)
	{
		nScoreRank[nCntScoreRank] = g_aranking[nCntScoreRank].nScore;
	}

	nScoreRank[nCntScoreRank] = nScore;		// 最後に今回のスコア

	qsort(&nScoreRank[0], MAX_RANK + 1, sizeof(int), Sort_desc);		// ソート

	// ランキングの情報に戻す
	for (nCntScoreRank = 0; nCntScoreRank < MAX_RANK; nCntScoreRank++)
	{
		g_aranking[nCntScoreRank].nScore = nScoreRank[nCntScoreRank];
		if (g_aranking[nCntScoreRank].nScore == nScore && g_nRankUpdate == -1)
		{// 今回のスコアがランクインしていたら
			g_nRankUpdate = nCntScoreRank;		// ナンバーを記録
		}
	}

	FILE* pFile;		// ファイルへのポインタ

#ifdef _DEBUG
	pFile = fopen("data\\Rankingtest.bin", "wb");
#else
	// ファイルを書き込みモードで開く
	pFile = fopen("data\\Ranking001.bin", "wb");
#endif
	if (pFile == NULL)
	{// ファイルを開けなかったら
		return;
	}

	// 書き込み
	fwrite(&nScoreRank[0], sizeof(int), MAX_RANK, pFile);

	fclose(pFile);			// ファイルを閉じる

}

//====================================
//	スコアのソート処理
//====================================
int Sort_desc(const void* pCastScore1, const void* pCastScore2)
{
	//int型のポインタを宣言
	const int* pScore1;
	const int* pScore2;

	//引数をint型にキャストして代入
	pScore1 = (const int*)pCastScore1;
	pScore2 = (const int*)pCastScore2;

	//変数の値を比較
	return *pScore2 - *pScore1;
}