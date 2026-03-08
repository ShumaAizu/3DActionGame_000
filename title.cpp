//=============================================================================
//
//	タイトル画面処理 [title.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "title.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "loadscript.h"

#include "titlemenu.h"
#include "titlelogo.h"

#include "model.h"
#include "meshfield.h"
#include "meshcylinder.h"
#include "meshdome.h"

#include "player.h"
#include "friends.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
TITLESTATE g_titleState = TITLESTATE_OP;							// タイトルの状態
int g_nCounterTitleState = 0;										// 状態管理カウンター

//====================================
//	タイトルの初期化処理
//====================================
void InitTitle(void)
{
	// タイトルロゴの初期化処理
	InitTitleLogo();

	// タイトルメニューの初期化処理
	InitTitleMenu();

	// プレイヤーの初期化処理
	InitPlayer(MODE_TITLE);

	// 仲間の初期化処理
	InitFriends();

	// モデルの再利用処理
	ReuseModel(MODE_TITLE);

	// BGM再生
	PlaySound(SOUND_LABEL_TITLEBGM000);

	g_titleState = TITLESTATE_WAIT;

	// タイトルスクリプト読み込み
	LoadScript(TITLE_SCRIPT);

	SetMotion(MOTIONTYPE_BUTREACTION, true, false, 0);
}

//====================================
//	タイトルの終了処理
//====================================
void UninitTitle(void)
{
	// タイトルロゴの終了処理
	UninitTitleLogo();

	// タイトルメニューの終了処理
	UninitTitleMenu();

	// プレイヤーの終了処理
	UninitPlayer();

	// 仲間の終了処理
	UninitFriends();

	// モデルの終了処理
	UninitModeModel(MODE_TITLE);

	// BGM終了
	StopSound();
}

//====================================
//	タイトルの描画処理
//====================================
void DrawTitle(void)
{
	// プレイヤーの描画処理
	DrawPlayer();

	// 仲間の描画処理
	DrawFriends();

	// モデルの描画処理
	DrawModel();

	// メッシュフィールドの描画処理
	DrawMeshField();

	// メッシュシリンダーの描画処理
	DrawMeshCylinder();

	// メッシュドームの描画処理
	DrawMeshDome();

	if (g_titleState == TITLESTATE_OP || g_titleState == TITLESTATE_MENU)
	{
		// タイトルロゴの描画処理
		DrawTitleLogo();
	}

	if (g_titleState == TITLESTATE_MENU)
	{
		// タイトルメニューの描画処理
		DrawTitleMenu();
	}
}

//====================================
//	タイトルの更新処理
//====================================
void UpdateTitle(void)
{
	// プレイヤーの更新処理
	UpdatePlayer(MODE_TITLE);

	// 仲間の更新処理
	UpdateFriends();

	// モデルの更新処理
	UpdateModel();

	// メッシュフィールドの更新処理
	UpdateMeshField();

	// メッシュシリンダーの更新処理
	UpdateMeshCylinder();

	// メッシュドームの更新処理
	UpdateMeshDome();

	if (g_titleState == TITLESTATE_MENU)
	{
		// タイトルメニューの更新処理
		UpdateTitleMenu();
	}

	if (g_titleState == TITLESTATE_OP || g_titleState == TITLESTATE_MENU)
	{
		// タイトルロゴの更新処理
		UpdateTitleLogo();
	}
}

//========================================
//	タイトルの状態設定
//========================================
void SetTitleState(TITLESTATE titlestate)
{
	g_titleState = titlestate;
}

//========================================
//	タイトルの状態取得
//========================================
TITLESTATE GetTitleState(void)
{
	return g_titleState;
}