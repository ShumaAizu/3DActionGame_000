//=============================================================================
//
//	タイトルメニュー処理 [titlemenu.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "sound.h"
#include "title.h"
#include "titlemenu.h"
#include "input.h"
#include "fade.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TITLEMENU_POSX			(1080.0f)			// タイトルメニューの位置X
#define TITLEMENU_POSY			(440.0f)			// タイトルメニューの位置Y
#define TITLEMENU_WIDTH			(240.0f)			// タイトルメニューの幅
#define TITLEMENU_HEIGHT		(80.0f)				// タイトルメニューの高さ
#define TITLEFADE_TIMER			(300)				// タイマーの秒数

//*****************************************************************************
// タイトルメニュー構造体
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	int nDispCounter;		// カウンター
	bool bDisp;				// 表示状態
}TitleMenu;

typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 向き
	float fWidth;			// 幅
	float fHeight;			// 高さ
	float fLength;			// 長さ
	float fAngle;			// 角度
}TitleMenuInfo;

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_apTextureTitleMenu[TITLEMENU_MAX] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitleMenu = NULL;					// 頂点バッファへのポインタ
TITLEMENU g_titleMenu = TITLEMENU_START;							// タイトルメニューの状態
TitleMenu g_atitleMenu[TITLEMENU_MAX] = {};							// タイトルメニューの情報
int g_nTitleMenuChangeCounter = 0;									// メニュー切り替えカウンター
bool g_bUpdate_TitleMenu = true;									// タイトルメニュー操作可能か

TitleMenuInfo g_TitleMenuInfo[] =
{
	{ D3DXVECTOR3(1080.0f, 470.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.95f * D3DX_PI), TITLEMENU_WIDTH, TITLEMENU_HEIGHT , 0.0f, 0.0f },
	{ D3DXVECTOR3(1080.0f, 620.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.95f * D3DX_PI), TITLEMENU_WIDTH, TITLEMENU_HEIGHT , 0.0f, 0.0f },
};

//=============================================================================
//	タイトルメニューの初期化処理
//=============================================================================
void InitTitleMenu(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	const char* pTitleUIPass[TITLEMENU_MAX] =
	{
		"data\\TEXTURE\\start000.png",
		"data\\TEXTURE\\exit000.png",
	};


	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * TITLEMENU_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitleMenu,
		NULL);

	for (int nCntTitleMenuTex = 0; nCntTitleMenuTex < TITLEMENU_MAX; nCntTitleMenuTex++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, pTitleUIPass[nCntTitleMenuTex], &g_apTextureTitleMenu[nCntTitleMenuTex]);
	}

	// 初期化
	g_titleMenu = TITLEMENU_START;
	g_bUpdate_TitleMenu = true;
	g_nTitleMenuChangeCounter = 18;

	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffTitleMenu->Lock(0, 0, (void * *)&pVtx, 0);

	for (int nCntTitleMenu = 0; nCntTitleMenu < TITLEMENU_MAX; nCntTitleMenu++)
	{
		g_atitleMenu[nCntTitleMenu].pos = g_TitleMenuInfo[nCntTitleMenu].pos;
		g_atitleMenu[nCntTitleMenu].nDispCounter = 4;
		g_atitleMenu[nCntTitleMenu].bDisp = true;

		g_TitleMenuInfo[nCntTitleMenu].fLength = SQRTF(g_TitleMenuInfo[nCntTitleMenu].fWidth, g_TitleMenuInfo[nCntTitleMenu].fHeight);
		g_TitleMenuInfo[nCntTitleMenu].fAngle = atan2f(g_TitleMenuInfo[nCntTitleMenu].fWidth, g_TitleMenuInfo[nCntTitleMenu].fHeight);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_atitleMenu[nCntTitleMenu].pos.x - g_TitleMenuInfo[nCntTitleMenu].fWidth, g_atitleMenu[nCntTitleMenu].pos.y - g_TitleMenuInfo[nCntTitleMenu].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_atitleMenu[nCntTitleMenu].pos.x + g_TitleMenuInfo[nCntTitleMenu].fWidth, g_atitleMenu[nCntTitleMenu].pos.y - g_TitleMenuInfo[nCntTitleMenu].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_atitleMenu[nCntTitleMenu].pos.x - g_TitleMenuInfo[nCntTitleMenu].fWidth, g_atitleMenu[nCntTitleMenu].pos.y + g_TitleMenuInfo[nCntTitleMenu].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_atitleMenu[nCntTitleMenu].pos.x + g_TitleMenuInfo[nCntTitleMenu].fWidth, g_atitleMenu[nCntTitleMenu].pos.y + g_TitleMenuInfo[nCntTitleMenu].fHeight, 0.0f);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		if (nCntTitleMenu == g_titleMenu)
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
	g_pVtxBuffTitleMenu->Unlock();
}

//=============================================================================
//	タイトルメニューの終了処理
//=============================================================================
void UninitTitleMenu(void)
{
	// テクスチャの破棄
	for (int nCntTitleMenu = 0; nCntTitleMenu < TITLEMENU_MAX; nCntTitleMenu++)
	{
		if (g_apTextureTitleMenu[nCntTitleMenu] != NULL)
		{
			g_apTextureTitleMenu[nCntTitleMenu]->Release();
			g_apTextureTitleMenu[nCntTitleMenu] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffTitleMenu != NULL)
	{
		g_pVtxBuffTitleMenu->Release();
		g_pVtxBuffTitleMenu = NULL;
	}
}

//=============================================================================
//	タイトルメニューの描画処理
//=============================================================================
void DrawTitleMenu(void)
{
	LPDIRECT3DDEVICE9 pDevice;				// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTitleMenu, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	
	for (int nCntTitleMenu = 0; nCntTitleMenu < TITLEMENU_MAX; nCntTitleMenu++)
	{
		if (g_atitleMenu[nCntTitleMenu].bDisp == true)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureTitleMenu[nCntTitleMenu]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTitleMenu * 4, 2);
		}
	}
	
}

//=============================================================================
//	タイトルメニューの更新処理
//=============================================================================
void UpdateTitleMenu(void)
{
	TitleMenuInfo* pTitleMenuInfo = &g_TitleMenuInfo[0];

	// 頂点座標の更新
	VERTEX_2D* pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffTitleMenu->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTitleMenu = 0; nCntTitleMenu < TITLEMENU_MAX; nCntTitleMenu++, pTitleMenuInfo++)
	{
		if (nCntTitleMenu == g_titleMenu)
		{ // 選択されていれば不透明度を戻す

			pVtx[0].pos.x = g_atitleMenu[nCntTitleMenu].pos.x + sinf(pTitleMenuInfo->rot.z + pTitleMenuInfo->fAngle) * pTitleMenuInfo->fLength;
			pVtx[0].pos.y = g_atitleMenu[nCntTitleMenu].pos.y + cosf(pTitleMenuInfo->rot.z + pTitleMenuInfo->fAngle) * pTitleMenuInfo->fLength;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_atitleMenu[nCntTitleMenu].pos.x + sinf(pTitleMenuInfo->rot.z - pTitleMenuInfo->fAngle) * pTitleMenuInfo->fLength;
			pVtx[1].pos.y = g_atitleMenu[nCntTitleMenu].pos.y + cosf(pTitleMenuInfo->rot.z - pTitleMenuInfo->fAngle) * pTitleMenuInfo->fLength;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_atitleMenu[nCntTitleMenu].pos.x + sinf(pTitleMenuInfo->rot.z + D3DX_PI - pTitleMenuInfo->fAngle) * pTitleMenuInfo->fLength;
			pVtx[2].pos.y = g_atitleMenu[nCntTitleMenu].pos.y + cosf(pTitleMenuInfo->rot.z + D3DX_PI - pTitleMenuInfo->fAngle) * pTitleMenuInfo->fLength;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_atitleMenu[nCntTitleMenu].pos.x + sinf(pTitleMenuInfo->rot.z + D3DX_PI +pTitleMenuInfo->fAngle) * pTitleMenuInfo->fLength;
			pVtx[3].pos.y = g_atitleMenu[nCntTitleMenu].pos.y + cosf(pTitleMenuInfo->rot.z + D3DX_PI +pTitleMenuInfo->fAngle) * pTitleMenuInfo->fLength;
			pVtx[3].pos.z = 0.0f;

			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else
		{ // 選択されていなければ不透明度を下げる
			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_atitleMenu[nCntTitleMenu].pos.x - g_TitleMenuInfo[nCntTitleMenu].fWidth, g_atitleMenu[nCntTitleMenu].pos.y - g_TitleMenuInfo[nCntTitleMenu].fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_atitleMenu[nCntTitleMenu].pos.x + g_TitleMenuInfo[nCntTitleMenu].fWidth, g_atitleMenu[nCntTitleMenu].pos.y - g_TitleMenuInfo[nCntTitleMenu].fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_atitleMenu[nCntTitleMenu].pos.x - g_TitleMenuInfo[nCntTitleMenu].fWidth, g_atitleMenu[nCntTitleMenu].pos.y + g_TitleMenuInfo[nCntTitleMenu].fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_atitleMenu[nCntTitleMenu].pos.x + g_TitleMenuInfo[nCntTitleMenu].fWidth, g_atitleMenu[nCntTitleMenu].pos.y + g_TitleMenuInfo[nCntTitleMenu].fHeight, 0.0f);

			// 頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		}

		pVtx += 4;
	}

	if (g_bUpdate_TitleMenu == true)
	{
		if (GetJoypadRepeat(JOYKEY_UP) == true || GetKeyboardRepeat(DIK_W) == true || GetJoypadStickRepeat(JOYSTICKL_UP) == true)
		{ // 上方向キーが押されたら
			// 現在のモードに合わせて変更
			PlaySound(SOUND_LABEL_SELECT000);
			switch (g_titleMenu)
			{
			case TITLEMENU_START:
				g_titleMenu = TITLEMENU_EXIT;
				break;

			case TITLEMENU_EXIT:
				g_titleMenu = TITLEMENU_START;
				break;
			}
		}

		if (GetJoypadRepeat(JOYKEY_DOWN) == true || GetKeyboardRepeat(DIK_S) == true || GetJoypadStickRepeat(JOYSTICKL_DOWN) == true)
		{ // 下方向キーが押されたら
			// 現在のモードに合わせて変更
			PlaySound(SOUND_LABEL_SELECT000);
			switch (g_titleMenu)
			{
			case TITLEMENU_START:
				g_titleMenu = TITLEMENU_EXIT;
				break;

			case TITLEMENU_EXIT:
				g_titleMenu = TITLEMENU_START;
				break;
			}
		}
	}

	if ((GetJoypadTrigger(JOYKEY_A) == true || GetKeyboardTrigger(DIK_RETURN) == true) && GetFade() != FADE_OUT)
	{ // 決定キーが押されたら
		if (g_bUpdate_TitleMenu == true)
		{
			PlaySound(SOUND_LABEL_SELECT001);
		}
		g_bUpdate_TitleMenu = false;
	}

	if (g_bUpdate_TitleMenu == false)
	{
		g_nTitleMenuChangeCounter--;
		g_atitleMenu[g_titleMenu].nDispCounter++;
		if (g_atitleMenu[g_titleMenu].nDispCounter % 5 == 0 && g_nTitleMenuChangeCounter >= 0)
		{
			g_atitleMenu[g_titleMenu].bDisp = g_atitleMenu[g_titleMenu].bDisp ? false : true;
		}

		if (g_nTitleMenuChangeCounter <= 0)
		{
			// 現在のモードに合わせて変更
			switch (g_titleMenu)
			{

			case TITLEMENU_START:
				SetFade(MODE_TUTORIAL, COLOR_BLACK, 0.025f, 0.025f);
				break;

			case TITLEMENU_EXIT:
				PostQuitMessage(0);
				break;
			}
			
		}
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffTitleMenu->Unlock();
}

//=============================================================================
//	タイトルメニューの設定処理
//=============================================================================
void SetTitleMenu(TITLEMENU titlemenu)
{
	g_titleMenu = titlemenu;
}