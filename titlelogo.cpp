//=============================================================================
//
//	タイトルロゴ表示処理 [titlelogo.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "title.h"
#include "titlelogo.h"
#include "effect.h"
#include "particle.h"
#include "input.h"
#include "sound.h"
#include "fade.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TITLELOGO_DEFAULTPOSX		(980.0f)
#define TITLELOGO_DEFAULTPOSY		(180.0f)
#define TITLELOGO_SIZEX				(300.0f)
#define TITLELOGO_SIZEY				(200.0f)
#define TITLELOGO_SPEED				(10.0f)
#define TITLELOGO_ALPHADECREASE		(0.0075f)			// タイトルロゴの不透明度増加速度

//*****************************************************************************
// タイトルロゴ構造体
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXCOLOR col;			// 色
	float fAngle;			// 角度
	bool bDisp;				// 表示状態
}TitleLogo;

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureTitleLogo = NULL;				// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitleLogo = NULL;			// 頂点バッファへのポインタ
TitleLogo g_titleLogo = {};									// タイトルロゴの情報
int g_nStateChangeCounter = 0;

//====================================
//	タイトルの初期化処理
//====================================
void InitTitleLogo(void)
{
	LPDIRECT3DDEVICE9 pDevice;				// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\titlelogo.png",
		&g_pTextureTitleLogo);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitleLogo,
		NULL);

	g_titleLogo.pos = { TITLELOGO_DEFAULTPOSX , TITLELOGO_DEFAULTPOSY , 0.0f };
	g_titleLogo.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	g_titleLogo.fAngle = 0.0f;
	g_nStateChangeCounter = 0;

	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffTitleLogo->Lock(0, 0, (void * *)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_titleLogo.pos.x - TITLELOGO_SIZEX, g_titleLogo.pos.y - TITLELOGO_SIZEY, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_titleLogo.pos.x + TITLELOGO_SIZEX, g_titleLogo.pos.y - TITLELOGO_SIZEY, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_titleLogo.pos.x - TITLELOGO_SIZEX, g_titleLogo.pos.y + TITLELOGO_SIZEY, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_titleLogo.pos.x + TITLELOGO_SIZEX, g_titleLogo.pos.y + TITLELOGO_SIZEY, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = g_titleLogo.col;
	pVtx[1].col = g_titleLogo.col;
	pVtx[2].col = g_titleLogo.col;
	pVtx[3].col = g_titleLogo.col;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffTitleLogo->Unlock();
}

//====================================
//	タイトルの終了処理
//====================================
void UninitTitleLogo(void)
{

	// テクスチャの破棄
	if (g_pTextureTitleLogo != NULL)
	{
		g_pTextureTitleLogo->Release();
		g_pTextureTitleLogo = NULL;
	}
	

	// 頂点バッファの破棄
	if (g_pVtxBuffTitleLogo != NULL)
	{
		g_pVtxBuffTitleLogo->Release();
		g_pVtxBuffTitleLogo = NULL;
	}
}

//====================================
//	タイトルの描画処理
//====================================
void DrawTitleLogo(void)
{
	LPDIRECT3DDEVICE9 pDevice;				// デバイスへのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTitleLogo, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTitleLogo);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//====================================
//	タイトルの更新処理
//====================================
void UpdateTitleLogo(void)
{
	VERTEX_2D* pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffTitleLogo->Lock(0, 0, (void**)&pVtx, 0);

	g_titleLogo.fAngle += 0.05f;		// 位置調整角度を加算

	g_titleLogo.fAngle = AngleNormalize(g_titleLogo.fAngle);	// 補正

	g_titleLogo.pos.y += sinf(g_titleLogo.fAngle) * 0.4f;		// 縦軸にだけ角度分の距離を足す

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_titleLogo.pos.x - TITLELOGO_SIZEX, g_titleLogo.pos.y - TITLELOGO_SIZEY, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_titleLogo.pos.x + TITLELOGO_SIZEX, g_titleLogo.pos.y - TITLELOGO_SIZEY, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_titleLogo.pos.x - TITLELOGO_SIZEX, g_titleLogo.pos.y + TITLELOGO_SIZEY, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_titleLogo.pos.x + TITLELOGO_SIZEX, g_titleLogo.pos.y + TITLELOGO_SIZEY, 0.0f);

	switch (GetTitleState())
	{
	case TITLESTATE_OP:
		// 不透明度をあげる
		g_titleLogo.col.a += TITLELOGO_ALPHADECREASE;

		if (GetKeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_START) == true || GetJoypadTrigger(JOYKEY_A) == true)
		{
			g_titleLogo.col.a = 1.0f;
		}

		// 頂点カラーの設定
		pVtx[0].col = g_titleLogo.col;
		pVtx[1].col = g_titleLogo.col;
		pVtx[2].col = g_titleLogo.col;
		pVtx[3].col = g_titleLogo.col;

		if (g_titleLogo.col.a >= 1.0f)
		{
			g_titleLogo.col.a = 1.0f;
			SetTitleState(TITLESTATE_MENU);
		}

		break;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffTitleLogo->Unlock();
}