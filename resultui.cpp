//=============================================================================
//
//	リザルトUI処理 [resultui.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "sound.h"
#include "title.h"
#include "resultui.h"
#include "input.h"
#include "fade.h"
#include "game.h"

#include "friends.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define RESULTUI_POSX			(1080.0f)			// リザルトUIの位置X
#define RESULTUI_POSY			(440.0f)			// リザルトUIの位置Y
#define RESULTUI_WIDTH			(240.0f)			// リザルトUIの幅
#define RESULTUI_HEIGHT			(80.0f)				// リザルトUIの高さ
#define RESULTUI_NUM			(2)					// 使う桁数

//*****************************************************************************
// リザルトUI構造体
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXCOLOR col;			// 色
	int nDispCounter;		// カウンター
	bool bDisp;				// 表示状態
}ResultUI;

typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	float fWidth;			// 幅
	float fHeight;			// 高さ
}ResultUIInfo;

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_apTextureResultUI[RESULTUI_MAX] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultUI = NULL;				// 頂点バッファへのポインタ
ResultUI g_aResultUI[RESULTUI_MAX + RESULTUI_NUM - 1] = {};		// リザルトUIの情報

ResultUIInfo g_aResultUIInfo[] =
{
	{ D3DXVECTOR3(SCREEN_WIDTH / 2, 50.0f, 0.0f), 250.0f, 50.0f },
	{ D3DXVECTOR3(470.0f, 580.0f, 0.0f), 50.0f, 100.0f },
	{ D3DXVECTOR3(570.0f, 580.0f, 0.0f), 50.0f, 100.0f },
	{ D3DXVECTOR3(670.0f, 580.0f, 0.0f), 60.0f, 60.0f },
	{ D3DXVECTOR3(770.0f, 580.0f, 0.0f), 60.0f, 60.0f },
};

//=============================================================================
//	リザルトUIの初期化処理
//=============================================================================
void InitResultUI(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	ResultUI* pResultUI = &g_aResultUI[0];
	ResultUIInfo* pResultUIInfo = &g_aResultUIInfo[0];

	const char* pResultUIPass[RESULTUI_MAX] =
	{
		"data\\TEXTURE\\resultUI000.png",
		"data\\TEXTURE\\number004.png",
		"data\\TEXTURE\\resultUI001.png",
		"data\\TEXTURE\\symbol000.png",
	};


	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * (RESULTUI_MAX + RESULTUI_NUM),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResultUI,
		NULL);

	for (int nCntResultUITex = 0; nCntResultUITex < RESULTUI_MAX; nCntResultUITex++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, pResultUIPass[nCntResultUITex], &g_apTextureResultUI[nCntResultUITex]);
	}

	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffResultUI->Lock(0, 0, (void * *)&pVtx, 0);

	for (int nCntResultUI = 0; nCntResultUI < RESULTUI_MAX + RESULTUI_NUM - 1; nCntResultUI++, pResultUI++, pResultUIInfo++)
	{
		pResultUI->pos = pResultUIInfo->pos;
		pResultUI->col = COLOR_WHITE;
		pResultUI->col.a = 0.0f;
		pResultUI->nDispCounter = 4;
		pResultUI->bDisp = true;

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(pResultUI->pos.x - pResultUIInfo->fWidth, pResultUI->pos.y - pResultUIInfo->fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pResultUI->pos.x + pResultUIInfo->fWidth, pResultUI->pos.y - pResultUIInfo->fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pResultUI->pos.x - pResultUIInfo->fWidth, pResultUI->pos.y + pResultUIInfo->fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pResultUI->pos.x + pResultUIInfo->fWidth, pResultUI->pos.y + pResultUIInfo->fHeight, 0.0f);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffResultUI->Unlock();
}

//=============================================================================
//	リザルトUIの終了処理
//=============================================================================
void UninitResultUI(void)
{
	// テクスチャの破棄
	for (int nCntResultUI = 0; nCntResultUI < RESULTUI_MAX; nCntResultUI++)
	{
		if (g_apTextureResultUI[nCntResultUI] != NULL)
		{
			g_apTextureResultUI[nCntResultUI]->Release();
			g_apTextureResultUI[nCntResultUI] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffResultUI != NULL)
	{
		g_pVtxBuffResultUI->Release();
		g_pVtxBuffResultUI = NULL;
	}
}

//=============================================================================
//	リザルトUIの描画処理
//=============================================================================
void DrawResultUI(void)
{
	LPDIRECT3DDEVICE9 pDevice;				// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffResultUI, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	int nCntTex = 0;
	
	for (int nCntResultUI = 0; nCntResultUI < RESULTUI_MAX + RESULTUI_NUM - 1; nCntResultUI++)
	{
		if (g_aResultUI[nCntResultUI].bDisp == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureResultUI[nCntTex]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntResultUI * 4, 2);

			if (nCntResultUI == RESULTUI_NUMBER)
			{
				continue;
			}

			nCntTex++;
		}
	}
	
}

//=============================================================================
//	リザルトUIの更新処理
//=============================================================================
void UpdateResultUI(void)
{
	ResultUI* pResultUI = &g_aResultUI[0];
	ResultUIInfo* pResultUIInfo = &g_aResultUIInfo[0];

	int nNumFriends = GetNumResultFriends();

	int aTexU[RESULTUI_NUM];		// 各桁の数字を格納

	aTexU[0] = nNumFriends % 100 / 10;
	aTexU[1] = nNumFriends % 10 / 1;

	// 頂点座標の更新
	VERTEX_2D* pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffResultUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntResultUI = 0; nCntResultUI < RESULTUI_MAX; nCntResultUI++, pResultUI++)
	{
		if (nCntResultUI == RESULTUI_FRIENDS)
		{
			if (pResultUI->col.a < 1.0f)
			{
				pResultUI->col.a += 0.05f;
			}
		}

		if (nCntResultUI == RESULTUI_NUMBER)
		{
			if (nNumFriends < 10)
			{
				pResultUI->bDisp = false;
			}

			for (int nCntNum = 0; nCntNum < RESULTUI_NUM; nCntNum++)
			{
				// テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2(0.1f * aTexU[nCntNum], 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.1f * (aTexU[nCntNum] + 1), 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.1f * aTexU[nCntNum], 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.1f * (aTexU[nCntNum] + 1), 1.0f);

				// 頂点カラーの設定
				pVtx[0].col = pResultUI->col;
				pVtx[1].col = pResultUI->col;
				pVtx[2].col = pResultUI->col;
				pVtx[3].col = pResultUI->col;

				pVtx += 4;
			}

			pVtx -= 4;

			if (GetResultState() == RESULTSTATE_RESULT && pResultUI->col.a < 1.0f)
			{
				pResultUI->col.a += 0.05f;
			}

			pResultUI++;

			if (GetResultState() == RESULTSTATE_RESULT && pResultUI->col.a < 1.0f)
			{
				pResultUI->col.a += 0.05f;
			}
		}

		if (nCntResultUI == RESULTUI_VALUE)
		{
			if (GetResultState() == RESULTSTATE_RESULT && pResultUI->col.a < 1.0f)
			{
				pResultUI->col.a += 0.05f;
			}
		}

		if (nCntResultUI == RESULTUI_SYMBOL)
		{
			if (nNumFriends <= 0)
			{
				// テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2(0.5f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.75f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.5f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.75f, 1.0f);
			}
			else
			{
				// テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.25f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.25f, 1.0f);
			}

			if (GetResultState() == RESULTSTATE_RESULT && pResultUI->col.a < 1.0f)
			{
				pResultUI->col.a += 0.05f;
			}
		}

		// 頂点カラーの設定
		pVtx[0].col = pResultUI->col;
		pVtx[1].col = pResultUI->col;
		pVtx[2].col = pResultUI->col;
		pVtx[3].col = pResultUI->col;

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffResultUI->Unlock();
}