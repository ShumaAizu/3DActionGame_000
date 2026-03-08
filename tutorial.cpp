//=============================================================================
//
//	タイトルメニュー処理 [tutorial.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "sound.h"
#include "title.h"
#include "tutorial.h"
#include "input.h"
#include "fade.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TUTORIAL_POSX			(1080.0f)			// タイトルメニューの位置X
#define TUTORIAL_POSY			(440.0f)			// タイトルメニューの位置Y
#define TUTORIAL_WIDTH			(SCREEN_WIDTH / 2)			// タイトルメニューの幅
#define TUTORIAL_HEIGHT			(SCREEN_HEIGHT / 2)				// タイトルメニューの高さ
#define TITLEFADE_TIMER			(300)				// タイマーの秒数

//*****************************************************************************
// タイトルメニュー構造体
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	int nDispCounter;		// カウンター
	bool bDisp;				// 表示状態
}Tutorial;

typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 向き
	float fWidth;			// 幅
	float fHeight;			// 高さ
	float fLength;			// 長さ
	float fAngle;			// 角度
}TutorialInfo;

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_apTextureTutorial[TUTORIAL_MAX] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorial = NULL;				// 頂点バッファへのポインタ
TUTORIAL g_titleMenu = TUTORIAL_START;							// タイトルメニューの状態
Tutorial g_atitleMenu[TUTORIAL_MAX] = {};						// タイトルメニューの情報
int g_nTutorialChangeCounter = 0;								// メニュー切り替えカウンター
bool g_bUpdate_Tutorial = true;									// タイトルメニュー操作可能か

TutorialInfo g_TutorialInfo[] =
{
	{ D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.95f * D3DX_PI), TUTORIAL_WIDTH, TUTORIAL_HEIGHT , 0.0f, 0.0f },
	{ D3DXVECTOR3(960.0f, 620.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.95f * D3DX_PI), 150.0f, 75.0f , 0.0f, 0.0f },
};

//=============================================================================
//	タイトルメニューの初期化処理
//=============================================================================
void InitTutorial(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	const char* pTitleUIPass[TUTORIAL_MAX] =
	{
		"data\\TEXTURE\\tutorial001.jpg",
		"data\\TEXTURE\\tutorial002.png",
	};


	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * TUTORIAL_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorial,
		NULL);

	for (int nCntTutorialTex = 0; nCntTutorialTex < TUTORIAL_MAX; nCntTutorialTex++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, pTitleUIPass[nCntTutorialTex], &g_apTextureTutorial[nCntTutorialTex]);
	}

	// 初期化
	g_titleMenu = TUTORIAL_START;
	g_bUpdate_Tutorial = true;
	g_nTutorialChangeCounter = 18;

	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffTutorial->Lock(0, 0, (void * *)&pVtx, 0);

	for (int nCntTutorial = 0; nCntTutorial < TUTORIAL_MAX; nCntTutorial++)
	{
		g_atitleMenu[nCntTutorial].pos = g_TutorialInfo[nCntTutorial].pos;
		g_atitleMenu[nCntTutorial].nDispCounter = 4;
		g_atitleMenu[nCntTutorial].bDisp = true;

		g_TutorialInfo[nCntTutorial].fLength = SQRTF(g_TutorialInfo[nCntTutorial].fWidth, g_TutorialInfo[nCntTutorial].fHeight);
		g_TutorialInfo[nCntTutorial].fAngle = atan2f(g_TutorialInfo[nCntTutorial].fWidth, g_TutorialInfo[nCntTutorial].fHeight);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_atitleMenu[nCntTutorial].pos.x - g_TutorialInfo[nCntTutorial].fWidth, g_atitleMenu[nCntTutorial].pos.y - g_TutorialInfo[nCntTutorial].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_atitleMenu[nCntTutorial].pos.x + g_TutorialInfo[nCntTutorial].fWidth, g_atitleMenu[nCntTutorial].pos.y - g_TutorialInfo[nCntTutorial].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_atitleMenu[nCntTutorial].pos.x - g_TutorialInfo[nCntTutorial].fWidth, g_atitleMenu[nCntTutorial].pos.y + g_TutorialInfo[nCntTutorial].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_atitleMenu[nCntTutorial].pos.x + g_TutorialInfo[nCntTutorial].fWidth, g_atitleMenu[nCntTutorial].pos.y + g_TutorialInfo[nCntTutorial].fHeight, 0.0f);

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

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffTutorial->Unlock();

	PlaySound(SOUND_LABEL_TUTORIALBGM000);
}

//=============================================================================
//	タイトルメニューの終了処理
//=============================================================================
void UninitTutorial(void)
{
	// テクスチャの破棄
	for (int nCntTutorial = 0; nCntTutorial < TUTORIAL_MAX; nCntTutorial++)
	{
		if (g_apTextureTutorial[nCntTutorial] != NULL)
		{
			g_apTextureTutorial[nCntTutorial]->Release();
			g_apTextureTutorial[nCntTutorial] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffTutorial != NULL)
	{
		g_pVtxBuffTutorial->Release();
		g_pVtxBuffTutorial = NULL;
	}

	StopSound();
}

//=============================================================================
//	タイトルメニューの描画処理
//=============================================================================
void DrawTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice;				// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTutorial, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	
	for (int nCntTutorial = 0; nCntTutorial < TUTORIAL_MAX; nCntTutorial++)
	{
		if (g_atitleMenu[nCntTutorial].bDisp == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureTutorial[nCntTutorial]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTutorial * 4, 2);
		}
	}
	
}

//=============================================================================
//	タイトルメニューの更新処理
//=============================================================================
void UpdateTutorial(void)
{
	TutorialInfo* pTutorialInfo = &g_TutorialInfo[0];

	static int nCounter = 0;

	if (g_bUpdate_Tutorial != false)
	{
		nCounter++;
		if (g_atitleMenu[1].bDisp == false)
		{
			g_atitleMenu[1].bDisp = g_atitleMenu[1].bDisp ? false : true;
		}

		if (nCounter > 60)
		{
			g_atitleMenu[1].bDisp = g_atitleMenu[1].bDisp ? false : true;
			nCounter = 0;
		}
	}

	// 頂点座標の更新
	VERTEX_2D* pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	if ((GetJoypadTrigger(JOYKEY_A) == true || GetKeyboardTrigger(DIK_RETURN) == true) && GetFade() != FADE_OUT)
	{ // 決定キーが押されたら
		if (g_bUpdate_Tutorial == true)
		{
			SetJoypadVibration(13000, 19000, 15);
			PlaySound(SOUND_LABEL_SELECT001);
		}
		g_bUpdate_Tutorial = false;
	}

	if (g_bUpdate_Tutorial == false)
	{
		g_nTutorialChangeCounter--;
		g_atitleMenu[1].nDispCounter++;
		if (g_atitleMenu[1].nDispCounter % 5 == 0 && g_nTutorialChangeCounter >= 0)
		{
			g_atitleMenu[1].bDisp = g_atitleMenu[1].bDisp ? false : true;
		}

		if (g_nTutorialChangeCounter <= 0)
		{
			SetFade(MODE_GAME, COLOR_WHITE, DEFAULT_FADESPEED, DEFAULT_FADESPEED);
		}
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffTutorial->Unlock();
}

//=============================================================================
//	タイトルメニューの設定処理
//=============================================================================
void SetTutorial(TUTORIAL tutorial)
{
	g_titleMenu = tutorial;
}