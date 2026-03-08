//=============================================================================
//
//	タイトルメニュー処理 [itemui.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "sound.h"
#include "title.h"
#include "itemui.h"
#include "input.h"
#include "fade.h"
#include "game.h"
#include "item.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ITEMUI_POSX			(1080.0f)			// タイトルメニューの位置X
#define ITEMUI_POSY			(440.0f)			// タイトルメニューの位置Y
#define ITEMUI_WIDTH		(75.0f)			// タイトルメニューの幅
#define ITEMUI_HEIGHT		(75.0f)				// タイトルメニューの高さ
#define TITLEFADE_TIMER			(300)				// タイマーの秒数

//*****************************************************************************
// タイトルメニュー構造体
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	int nDispCounter;		// カウンター
	bool bDisp;				// 表示状態
}ItemUI;

typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 向き
	float fWidth;			// 幅
	float fHeight;			// 高さ
	float fLength;			// 長さ
	float fAngle;			// 角度
}ItemUIInfo;

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_apTextureItemUI[ITEMUI_MAX] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffItemUI = NULL;					// 頂点バッファへのポインタ
ITEMUI g_ItemUI = ITEMUI_START;							// タイトルメニューの状態
ItemUI g_aItemUI[ITEMUI_MAX] = {};							// タイトルメニューの情報
int g_nItemUIChangeCounter = 0;									// メニュー切り替えカウンター
bool g_bUpdate_ItemUI = true;									// タイトルメニュー操作可能か

ItemUIInfo g_ItemUIInfo[] =
{
	{ D3DXVECTOR3(1180.0f, 100.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.95f * D3DX_PI), ITEMUI_WIDTH, ITEMUI_HEIGHT , 0.0f, 0.0f },
	{ D3DXVECTOR3(1080.0f, 620.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.95f * D3DX_PI), ITEMUI_WIDTH, ITEMUI_HEIGHT , 0.0f, 0.0f },
};

//=============================================================================
//	タイトルメニューの初期化処理
//=============================================================================
void InitItemUI(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	const char* pTitleUIPass[ITEMUI_MAX] =
	{
		"data\\TEXTURE\\snowflake001.png",
	};


	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * ITEMUI_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffItemUI,
		NULL);

	for (int nCntItemUITex = 0; nCntItemUITex < ITEMUI_MAX; nCntItemUITex++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, pTitleUIPass[nCntItemUITex], &g_apTextureItemUI[nCntItemUITex]);
	}

	// 初期化
	g_ItemUI = ITEMUI_START;
	g_bUpdate_ItemUI = true;
	g_nItemUIChangeCounter = 18;

	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffItemUI->Lock(0, 0, (void * *)&pVtx, 0);

	for (int nCntItemUI = 0; nCntItemUI < ITEMUI_MAX; nCntItemUI++)
	{
		g_aItemUI[nCntItemUI].pos = g_ItemUIInfo[nCntItemUI].pos;
		g_aItemUI[nCntItemUI].nDispCounter = 4;
		g_aItemUI[nCntItemUI].bDisp = false;

		g_ItemUIInfo[nCntItemUI].fLength = SQRTF(g_ItemUIInfo[nCntItemUI].fWidth, g_ItemUIInfo[nCntItemUI].fHeight);
		g_ItemUIInfo[nCntItemUI].fAngle = atan2f(g_ItemUIInfo[nCntItemUI].fWidth, g_ItemUIInfo[nCntItemUI].fHeight);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aItemUI[nCntItemUI].pos.x - g_ItemUIInfo[nCntItemUI].fWidth, g_aItemUI[nCntItemUI].pos.y - g_ItemUIInfo[nCntItemUI].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aItemUI[nCntItemUI].pos.x + g_ItemUIInfo[nCntItemUI].fWidth, g_aItemUI[nCntItemUI].pos.y - g_ItemUIInfo[nCntItemUI].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aItemUI[nCntItemUI].pos.x - g_ItemUIInfo[nCntItemUI].fWidth, g_aItemUI[nCntItemUI].pos.y + g_ItemUIInfo[nCntItemUI].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aItemUI[nCntItemUI].pos.x + g_ItemUIInfo[nCntItemUI].fWidth, g_aItemUI[nCntItemUI].pos.y + g_ItemUIInfo[nCntItemUI].fHeight, 0.0f);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

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
	g_pVtxBuffItemUI->Unlock();
}

//=============================================================================
//	タイトルメニューの終了処理
//=============================================================================
void UninitItemUI(void)
{
	// テクスチャの破棄
	for (int nCntItemUI = 0; nCntItemUI < ITEMUI_MAX; nCntItemUI++)
	{
		if (g_apTextureItemUI[nCntItemUI] != NULL)
		{
			g_apTextureItemUI[nCntItemUI]->Release();
			g_apTextureItemUI[nCntItemUI] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffItemUI != NULL)
	{
		g_pVtxBuffItemUI->Release();
		g_pVtxBuffItemUI = NULL;
	}
}

//=============================================================================
//	タイトルメニューの描画処理
//=============================================================================
void DrawItemUI(void)
{
	LPDIRECT3DDEVICE9 pDevice;				// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffItemUI, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	
	for (int nCntItemUI = 0; nCntItemUI < ITEMUI_MAX; nCntItemUI++)
	{
		if (g_aItemUI[nCntItemUI].bDisp == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureItemUI[nCntItemUI]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntItemUI * 4, 2);
		}
	}
	
}

//=============================================================================
//	タイトルメニューの更新処理
//=============================================================================
void UpdateItemUI(void)
{
	ItemUIInfo* pItemUIInfo = &g_ItemUIInfo[0];

	// 頂点座標の更新
	VERTEX_2D* pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffItemUI->Lock(0, 0, (void**)&pVtx, 0);

	if (GetItem() == ITEMTYPE_000)
	{
		g_aItemUI[0].bDisp = true;
	}
	else
	{
		g_aItemUI[0].bDisp = false;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffItemUI->Unlock();
}

//=============================================================================
//	タイトルメニューの設定処理
//=============================================================================
void SetItemUI(ITEMUI itemui)
{
	g_ItemUI = itemui;
}