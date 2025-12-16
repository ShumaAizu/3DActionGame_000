//=============================================================================
//
//	ポーズ処理 [pause.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "pause.h"
#include "input.h"
#include "game.h"
#include "time.h"
#include "fade.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PAUSE_MENU_POSX		(920.0f)
#define PAUSE_MENU_POSY		(100.0f)
#define PAUSE_MENU_SIZEX	(380.0f)
#define PAUSE_MENU_SIZEY	(120.0f)

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_apTexturePause[PAUSE_MENU_MAX] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;					// 頂点バッファへのポインタ
PAUSE_MENU g_pauseMenu = PAUSE_MENU_CONTINUE;					// ポーズメニューの状態
bool g_bPauseDisp = true;

//=============================================================================
//	ポーズメニューの初期化処理
//=============================================================================
void InitPause(void)
{
	const char* pPauseMenuFileName[] =
	{
		"data\\TEXTURE\\continue000.png",
		"data\\TEXTURE\\restart000.png",
		"data\\TEXTURE\\quit000.png"
	};

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntPause = 0; nCntPause < PAUSE_MENU_MAX; nCntPause++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			pPauseMenuFileName[nCntPause],
			&g_apTexturePause[nCntPause]);
	}


	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * PAUSE_MENU_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause,
		NULL);

	// 初期化

	g_pauseMenu = PAUSE_MENU_CONTINUE;
	g_bPauseDisp = true;

	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void * *)&pVtx, 0);

	for (int nCntPause = 0; nCntPause < PAUSE_MENU_MAX; nCntPause++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(PAUSE_MENU_POSX, PAUSE_MENU_POSY + (nCntPause * PAUSE_MENU_SIZEY), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(PAUSE_MENU_POSX + PAUSE_MENU_SIZEX, PAUSE_MENU_POSY + (nCntPause * PAUSE_MENU_SIZEY), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(PAUSE_MENU_POSX, PAUSE_MENU_POSY + (nCntPause * PAUSE_MENU_SIZEY) + PAUSE_MENU_SIZEY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(PAUSE_MENU_POSX + PAUSE_MENU_SIZEX, PAUSE_MENU_POSY + (nCntPause * PAUSE_MENU_SIZEY) + PAUSE_MENU_SIZEY, 0.0f);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		if (nCntPause == g_pauseMenu)
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
	g_pVtxBuffPause->Unlock();
}

//=============================================================================
//	ポーズメニューの終了処理
//=============================================================================
void UninitPause(void)
{
	// テクスチャの破棄
	for (int nCntPause = 0; nCntPause < PAUSE_MENU_MAX; nCntPause++)
	{
		if (g_apTexturePause[nCntPause] != NULL)
		{
			g_apTexturePause[nCntPause]->Release();
			g_apTexturePause[nCntPause] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}
}

//=============================================================================
//	ポーズメニューの描画処理
//=============================================================================
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;				// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_bPauseDisp == true)
	{
		for (int nCntPause = 0; nCntPause < PAUSE_MENU_MAX; nCntPause++)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_apTexturePause[nCntPause]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPause * 4, 2);
		}
	}
}

//=============================================================================
//	ポーズメニューの更新処理
//=============================================================================
void UpdatePause(void)
{
	// 頂点座標の更新
	VERTEX_2D* pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	if (GetJoypadRepeat(JOYKEY_UP) == true || GetKeyboardRepeat(DIK_W) == true || GetJoypadStickRepeat(JOYSTICK_UP) == true)
	{ // 上方向キーが押されたら
		// 現在のモードに合わせて変更
		switch (g_pauseMenu)
		{
		case PAUSE_MENU_CONTINUE:
			g_pauseMenu = PAUSE_MENU_QUIT;
			break;

		case PAUSE_MENU_RESTART:
			g_pauseMenu = PAUSE_MENU_CONTINUE;
			break;

		case PAUSE_MENU_QUIT:
			g_pauseMenu = PAUSE_MENU_RESTART;
			break;
		}
	}

	if (GetJoypadRepeat(JOYKEY_DOWN) == true || GetKeyboardRepeat(DIK_S) == true || GetJoypadStickRepeat(JOYSTICK_DOWN) == true)
	{ // 下方向キーが押されたら
		// 現在のモードに合わせて変更
		switch (g_pauseMenu)
		{
		case PAUSE_MENU_CONTINUE:
			g_pauseMenu = PAUSE_MENU_RESTART;
			break;

		case PAUSE_MENU_RESTART:
			g_pauseMenu = PAUSE_MENU_QUIT;
			break;

		case PAUSE_MENU_QUIT:
			g_pauseMenu = PAUSE_MENU_CONTINUE;
			break;
		}
	}

	for (int nCntPause = 0; nCntPause < PAUSE_MENU_MAX; nCntPause++)
	{
		if (nCntPause == g_pauseMenu)
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

	if (GetJoypadTrigger(JOYKEY_A) == true || GetKeyboardTrigger(DIK_RETURN) == true)
	{ // 決定キーが押されたら
		// ポーズを解除
		SetEnablePause(false);
		// 現在のモードに合わせて変更
		switch (g_pauseMenu)
		{
		case PAUSE_MENU_CONTINUE:
			break;

		case PAUSE_MENU_RESTART:
			SetFade(MODE_GAME, 0.025f, 0.025f);
			break;

		case PAUSE_MENU_QUIT:
			SetFade(MODE_TITLE, 0.025f, 0.025f);
			break;
		}
	}

	if (GetKeyboardTrigger(DIK_F5) == true)
	{
		g_bPauseDisp = g_bPauseDisp ? false : true;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffPause->Unlock();
}

//=============================================================================
//	ポーズメニューの設定処理
//=============================================================================
void SetPauseMenu(PAUSE_MENU pause_menu)
{
	g_pauseMenu = pause_menu;
}

//=============================================================================
//	ポーズメニューの表示状態取得処理
//=============================================================================
bool GetPauseDisp(void)
{
	return g_bPauseDisp;
}