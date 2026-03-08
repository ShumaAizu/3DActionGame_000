//=============================================================================
//
//	ゲーム画面処理 [game.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "game.h"
#include "loadscript.h"
#include "player.h"
#include "camera.h"
#include "light.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "effect.h"
#include "particle.h"
#include "pause.h"

#include "friends.h"
#include "model.h"
#include "billboard.h"
#include "bullet.h"
#include "shadow.h"
#include "snow.h"
#include "time.h"
#include "request.h"
#include "item.h"
#include "itemui.h"

#include "meshfield.h"
#include "meshwall.h"
#include "meshring.h"
#include "meshcylinder.h"
#include "meshdome.h"
#include "meshsphere.h"

//*****************************************************************************
// グローバル変数
//*****************************************************************************
GAMEFLAG g_gameFlag = GAMEFLAG_NORMAL;					// ゲームの状態
GAMEFLAG g_nextgameFlag = GAMEFLAG_NORMAL;				// 次のゲームの状態
int g_nCounterGameFlag = 0;								// 状態管理カウンター
bool g_bPause = false;									// ポーズ中かどうか

//========================================
//	ゲーム画面の初期化処理
//========================================
void InitGame(void)
{
	g_gameFlag = GAMEFLAG_NORMAL;

	g_nextgameFlag = GAMEFLAG_NORMAL;

	g_nCounterGameFlag = 0;

	// ポーズ状態の初期化
	g_bPause = false;

	// 影の初期化
	InitShadow();

	// プレイヤーの初期化
	InitPlayer(MODE_GAME);

	// リクエストの初期化処理
	InitRequest();

	// 仲間の初期化処理
	InitFriends();

	// 雪の初期化
	InitSnow();

	// アイテムの初期化処理
	InitItem();

	// アイテムUIの初期化
	InitItemUI();

	// 制限時間の初期化
	InitTime();

	// エフェクトの初期化処理
	InitEffect();

	// パーティクルの初期化処理
	InitParticle();

	// ポーズメニューの初期化
	InitPause();

	// ゲームスクリプト読み込み
	LoadScript(GAME_SCRIPT);

	// モーションセット
	SetMotion(MOTIONTYPE_NEUTRAL, true, false, 0);

	PlaySound(SOUND_LABEL_GAMEBGM000);
}

//========================================
//	ゲーム画面の終了処理
//========================================
void UninitGame(void)
{
	// プレイヤーの終了処理
	UninitPlayer();

	// 仲間の終了処理
	UninitFriends();

	// 影の終了処理
	UninitShadow();

	// 雪の終了処理
	UninitSnow();

	// リクエストの終了処理
	UninitRequest();

	// アイテムの終了処理
	UninitItem();

	//
	UninitItemUI();

	// 制限時間の終了処理
	UninitTime();

	// エフェクトの終了処理
	UninitEffect();

	// パーティクルの終了処理
	UninitParticle();

	// ポーズメニューの終了処理
	UninitPause();

	// サウンド終了
	StopSound();
}

//========================================
//	ゲーム画面の更新処理
//========================================
void UpdateGame(void)
{
	if ((GetKeyboardTrigger(DIK_P) == true || GetJoypadTrigger(JOYKEY_START) == true) && GetFade() != FADE_IN && g_gameFlag == GAMEFLAG_NORMAL)
	{ // ポーズキーが押された
		g_bPause = g_bPause ? false : true;		// ポーズ状態を切り替える
		if (g_bPause == true)
		{

		}
		SetPauseMenu(PAUSE_MENU_CONTINUE);
	}

	if (g_bPause == true)
	{ // ポーズ中なら
		// ポーズの更新処理
		UpdatePause();
	}
	else if(GetFade() != FADE_OUT)
	{
		// メッシュフィールドの更新処理
		UpdateMeshField();

		// プレイヤーの更新処理
		UpdatePlayer(MODE_GAME);

		// 仲間の更新処理
		UpdateFriends();

		// モデルの更新処理
		UpdateModel();

		// 影の更新処理
		UpdateShadow();

		// 雪の更新処理
		UpdateSnow();

		// リクエストの更新処理
		UpdateRequest();

		// アイテムの更新処理
		UpdateItem();

		// 
		UpdateItemUI();

		// 制限時間の更新処理
		UpdateTime();

		// メッシュウォールの更新処理
		UpdateMeshWall();

		// メッシュシリンダーの更新処理
		UpdateMeshCylinder();

		// メッシュリングの更新処理
		UpdateMeshRing();

		// メッシュドームの更新処理
		UpdateMeshDome();

		// メッシュスフィアの更新処理
		UpdateMeshSphere();

		// エフェクトの更新処理
		UpdateEffect();

		// パーティクルの更新処理
		UpdateParticle();
	}

#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		//SetFade(MODE_RESULT, COLOR_WHITE, DEFAULT_FADESPEED, DEFAULT_FADESPEED);
	}
#endif

	if (g_nextgameFlag == GAMEFLAG_CLEAR || g_nextgameFlag == GAMEFLAG_GAMEOVER)
	{ // 次の状態がクリアかゲームオーバーになったら
		g_nCounterGameFlag--;			// カウントを減らす
		if (g_nCounterGameFlag <= 0)
		{ // 回り切ったら
			// ゲーム状態を次の状態に移す
			SaveFriends();
			SetNumResultFriends();
			g_gameFlag = g_nextgameFlag;
		}

		switch (g_gameFlag)
		{
			// クリア状態
		case GAMEFLAG_CLEAR: 
			SetFade(MODE_RESULT, COLOR_WHITE, DEFAULT_FADESPEED, DEFAULT_FADESPEED);
			break;
		}
	}
}

//========================================
//	ゲーム画面の描画処理
//========================================
void DrawGame(void)
{
	// メッシュフィールドの描画処理
	DrawMeshField();

	// プレイヤーの描画処理
	DrawPlayer();

	// 仲間の描画処理
	DrawFriends();

	// モデルの描画処理
	DrawModel();

	// エフェクトの描画処理
	DrawEffect();

	// 雪の描画処理
	DrawSnow();

	// アイテムの描画処理
	DrawItem();

	// メッシュウォールの描画処理
	DrawMeshWall();

	// メッシュシリンダーの描画処理
	DrawMeshCylinder();

	// メッシュリングの描画処理
	DrawMeshRing();

	// メッシュドームの描画処理
	DrawMeshDome();

	// メッシュスフィアの描画処理
	DrawMeshSphere();

	// リクエストの描画処理
	DrawRequest();

	// 影の描画処理
	DrawShadow();

	// 制限時間の描画処理
	DrawTime();

	//
	DrawItemUI();

	if (g_bPause == true)
	{ // ポーズ中なら

		// ポーズメニューの描画処理
		DrawPause();
	}

}

//========================================
//	ゲームの状態設定
//========================================
void SetGameFlag(GAMEFLAG state, int nCounter)
{
	g_gameFlag = GAMEFLAG_NONE;

	g_nextgameFlag = state;

	g_nCounterGameFlag = nCounter;
}

//========================================
//	ゲームの状態の取得
//========================================
GAMEFLAG GetGameFlag(void)
{
	return g_gameFlag;
}

//========================================
//	次のゲームの状態の取得
//========================================
GAMEFLAG GetNextGameFlag(void)
{
	return g_nextgameFlag;
}

//========================================
//	ポーズ状態の取得
//========================================
bool GetPause(void)
{
	return g_bPause;
}

//========================================
//	ポーズの有効無効設定
//========================================
void SetEnablePause(bool bPause)
{
	g_bPause = bPause;
}