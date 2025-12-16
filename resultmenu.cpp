//=============================================================================
//
//	リザルトメニュー処理 [resultmenu.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "sound.h"
#include "result.h"
#include "resultmenu.h"
#include "input.h"
#include "fade.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define RESULTMENU_POSX			(SCREEN_WIDTH / 2)	// リザルトメニューの位置X
#define RESULTMENU_POSY			(440.0f)			// リザルトメニューの位置Y
#define RESULTMENU_SIZEX		(180.0f)			// リザルトメニューのサイズX
#define RESULTMENU_SIZEY		(30.0f)				// リザルトメニューのサイズY
#define RESULTFADE_TIMER		(300)				// タイマーの秒数

//*****************************************************************************
// リザルトメニュー構造体
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	int nDispCounter;		// カウンター
	bool bDisp;				// 表示状態
}ResultMenu;

typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	float fWidth;			// 幅
	float fHeight;			// 高さ
}ResultMenuInfo;

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_apTextureResultMenu[RESULTMENU_MAX] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultMenu = NULL;				// 頂点バッファへのポインタ
RESULTMENU g_resultMenu = RESULTMENU_RESTART;						// リザルトメニューの状態
ResultMenu g_aresultMenu[RESULTMENU_MAX] = {};						// リザルトメニューの情報
int g_nResultMenuChangeCounter = 0;									// メニュー切り替えカウンター
bool g_bUpdate_ResultMenu = true;									// リザルトメニュー操作可能か

ResultMenuInfo g_ResultMenuInfo[] =
{
	{ D3DXVECTOR3(320.0f, 670.0f, 0.0f), 150.0f, 50.0f },
	{ D3DXVECTOR3(640.0f, 670.0f, 0.0f), 150.0f, 50.0f },
	{ D3DXVECTOR3(960.0f, 670.0f, 0.0f), 150.0f, 50.0f }
};

//=============================================================================
//	リザルトメニューの初期化処理
//=============================================================================
void InitResultMenu(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	const char* pResultUIPass[RESULTMENU_MAX] =
	{
		"data\\TEXTURE\\pauseui001.png",
		"data\\TEXTURE\\pauseui002.png",
		"data\\TEXTURE\\pauseui003.png",
	};


	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * RESULTMENU_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResultMenu,
		NULL);

	//for (int nCntResultMenuTex = 0; nCntResultMenuTex < RESULTMENU_MAX; nCntResultMenuTex++)
	//{
	//	// テクスチャの読み込み
	//	D3DXCreateTextureFromFile(pDevice, pResultUIPass[nCntResultMenuTex], &g_apTextureResultMenu[nCntResultMenuTex]);
	//}

	// 初期化
	g_resultMenu = RESULTMENU_RESTART;
	g_bUpdate_ResultMenu = true;
	g_nResultMenuChangeCounter = 18;

	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffResultMenu->Lock(0, 0, (void * *)&pVtx, 0);

	for (int nCntResultMenu = 0; nCntResultMenu < RESULTMENU_MAX; nCntResultMenu++)
	{
		g_aresultMenu[nCntResultMenu].pos = g_ResultMenuInfo[nCntResultMenu].pos;
		g_aresultMenu[nCntResultMenu].nDispCounter = 4;
		g_aresultMenu[nCntResultMenu].bDisp = true;

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aresultMenu[nCntResultMenu].pos.x - g_ResultMenuInfo[nCntResultMenu].fWidth, g_aresultMenu[nCntResultMenu].pos.y - g_ResultMenuInfo[nCntResultMenu].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aresultMenu[nCntResultMenu].pos.x + g_ResultMenuInfo[nCntResultMenu].fWidth, g_aresultMenu[nCntResultMenu].pos.y - g_ResultMenuInfo[nCntResultMenu].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aresultMenu[nCntResultMenu].pos.x - g_ResultMenuInfo[nCntResultMenu].fWidth, g_aresultMenu[nCntResultMenu].pos.y + g_ResultMenuInfo[nCntResultMenu].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aresultMenu[nCntResultMenu].pos.x + g_ResultMenuInfo[nCntResultMenu].fWidth, g_aresultMenu[nCntResultMenu].pos.y + g_ResultMenuInfo[nCntResultMenu].fHeight, 0.0f);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		if (nCntResultMenu == g_resultMenu)
		{ // 選択されていれば不透明度を戻す
			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else
		{ // 選択されていなければ不透明度を下げる
			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		}

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffResultMenu->Unlock();
}

//=============================================================================
//	リザルトメニューの終了処理
//=============================================================================
void UninitResultMenu(void)
{
	// テクスチャの破棄
	for (int nCntResultMenu = 0; nCntResultMenu < RESULTMENU_MAX; nCntResultMenu++)
	{
		if (g_apTextureResultMenu[nCntResultMenu] != NULL)
		{
			g_apTextureResultMenu[nCntResultMenu]->Release();
			g_apTextureResultMenu[nCntResultMenu] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffResultMenu != NULL)
	{
		g_pVtxBuffResultMenu->Release();
		g_pVtxBuffResultMenu = NULL;
	}
}

//=============================================================================
//	リザルトメニューの描画処理
//=============================================================================
void DrawResultMenu(void)
{
	LPDIRECT3DDEVICE9 pDevice;				// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffResultMenu, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	
	for (int nCntResultMenu = 0; nCntResultMenu < RESULTMENU_MAX; nCntResultMenu++)
	{
		if (g_aresultMenu[nCntResultMenu].bDisp == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, NULL);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntResultMenu * 4, 2);
		}
	}
	
}

//=============================================================================
//	リザルトメニューの更新処理
//=============================================================================
void UpdateResultMenu(void)
{
	// 頂点座標の更新
	VERTEX_2D* pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffResultMenu->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntResultMenu = 0; nCntResultMenu < RESULTMENU_MAX; nCntResultMenu++)
	{
		if (nCntResultMenu == g_resultMenu)
		{ // 選択されていれば不透明度を戻す
			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else
		{ // 選択されていなければ不透明度を下げる
			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		}

		pVtx += 4;
	}

	if (g_bUpdate_ResultMenu == true)
	{
		if (GetJoypadRepeat(JOYKEY_RIGHT) == true || GetKeyboardRepeat(DIK_D) == true || GetJoypadStickRepeat(JOYSTICK_RIGHT) == true)
		{ // 上方向キーが押されたら
			// 現在のモードに合わせて変更
			switch (g_resultMenu)
			{
			case RESULTMENU_RESTART:
				g_resultMenu = RESULTMENU_QUIT;
				break;

			case RESULTMENU_QUIT:
				g_resultMenu = RESULTMENU_RESTART;
				break;
			}
		}

		if (GetJoypadRepeat(JOYKEY_LEFT) == true || GetKeyboardRepeat(DIK_A) == true || GetJoypadStickRepeat(JOYSTICK_LEFT) == true)
		{ // 下方向キーが押されたら
			// 現在のモードに合わせて変更
			switch (g_resultMenu)
			{
			case RESULTMENU_RESTART:
				g_resultMenu = RESULTMENU_QUIT;
				break;

			case RESULTMENU_QUIT:
				g_resultMenu = RESULTMENU_RESTART;
				break;
			}
		}
	}

	if ((GetJoypadTrigger(JOYKEY_A) == true || GetKeyboardTrigger(DIK_RETURN) == true) && GetFade() != FADE_OUT)
	{ // 決定キーが押されたら
		if (g_bUpdate_ResultMenu == true)
		{
			
		}
		g_bUpdate_ResultMenu = false;
	}

	if (g_bUpdate_ResultMenu == false)
	{
		g_nResultMenuChangeCounter--;
		g_aresultMenu[g_resultMenu].nDispCounter++;
		if (g_aresultMenu[g_resultMenu].nDispCounter % 5 == 0 && g_nResultMenuChangeCounter >= 0)
		{
			g_aresultMenu[g_resultMenu].bDisp = g_aresultMenu[g_resultMenu].bDisp ? false : true;
		}

		if (g_nResultMenuChangeCounter <= 0)
		{
			// 現在のモードに合わせて変更
			switch (g_resultMenu)
			{

			case RESULTMENU_RESTART:
				SetFade(MODE_RANKING, 0.025f, 0.025f);
				break;

			case RESULTMENU_QUIT:
				SetFade(MODE_TITLE, 0.025f, 0.025f);
				break;
			}
			
		}
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffResultMenu->Unlock();
}

//=============================================================================
//	リザルトメニューの設定処理
//=============================================================================
void SetResultMenu(RESULTMENU resultmenu)
{
	g_resultMenu = resultmenu;
}