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

#include "meshfield.h"
#include "meshwall.h"
#include "meshring.h"
#include "meshcylinder.h"
#include "meshdome.h"
#include "meshsphere.h"

//*****************************************************************************
// グローバル変数
//*****************************************************************************
GAMESTATE g_gameState = GAMESTATE_NORMAL;				// ゲームの状態
GAMESTATE g_nextgameState = GAMESTATE_NORMAL;			// 次のゲームの状態
GAMEMODE g_gamemode = GAMEMODE_MAX;						// ゲームモードの状態
int g_nCounterGameState = 0;							// 状態管理カウンター
bool g_bPause = false;									// ポーズ中かどうか

//========================================
//	ゲーム画面の初期化処理
//========================================
void InitGame(void)
{
	g_gameState = GAMESTATE_NORMAL;

	g_nextgameState = GAMESTATE_NORMAL;

	g_nCounterGameState = 0;

	// ポーズ状態の初期化
	g_bPause = false;

	// メッシュフィールドの初期化
	InitMeshField();

	// 影の初期化
	InitShadow();

	// プレイヤーの初期化
	InitPlayer();

	// 仲間の初期化処理
	InitFriends();

	InitBullet();

	// 雪の初期化
	InitSnow();

	// メッシュウォールの初期化
	InitMeshWall();

	// メッシュシリンダーの初期化
	InitMeshCylinder();

	// メッシュリングの初期化
	InitMeshRing();

	// メッシュドームの初期化
	InitMeshDome();

	// メッシュスフィアの初期化
	InitMeshSphere();

	// エフェクトの初期化処理
	InitEffect();

	// パーティクルの初期化処理
	InitParticle();

	// ポーズメニューの初期化
	InitPause();

	LoadScript(GAME_SCRIPT);

	LoadObject(OBJECT_SCRIPT);

	SetMotion(MOTIONTYPE_NEUTRAL, true, false, 0);
}

//========================================
//	ゲーム画面の終了処理
//========================================
void UninitGame(void)
{
	// メッシュフィールドの終了処理
	UninitMeshField();

	// プレイヤーの終了処理
	UninitPlayer();

	// 仲間の終了処理
	UninitFriends();

	UninitBullet();

	// 影の終了処理
	UninitShadow();

	// 雪の終了処理
	UninitSnow();

	// メッシュウォールの終了処理
	UninitMeshWall();

	// メッシュシリンダーの終了処理
	UninitMeshCylinder();

	// メッシュリングの終了処理
	UninitMeshRing();

	// メッシュドームの終了処理
	UninitMeshDome();

	// メッシュスフィアの終了処理
	UninitMeshSphere();

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

	if (GetKeyboardTrigger(DIK_BACK) == true)
	{
		SetFade(MODE_TITLE, DEFAULT_FADESPEED, DEFAULT_FADESPEED);
	}

	if ((GetKeyboardTrigger(DIK_P) == true || GetJoypadTrigger(JOYKEY_START) == true) && g_gameState == GAMESTATE_NORMAL)
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
	else if(GetFade() != FADE_OUT && g_gameState == GAMESTATE_NORMAL)
	{
		// メッシュフィールドの更新処理
		UpdateMeshField();

		// プレイヤーの更新処理
		UpdatePlayer();

		// 仲間の更新処理
		UpdateFriends();

		UpdateBullet();

		// モデルの更新処理
		UpdateModel();

		// 影の更新処理
		UpdateShadow();

		// 雪の更新処理
		UpdateSnow();

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

	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		SetFade(MODE_RESULT, DEFAULT_FADESPEED, DEFAULT_FADESPEED);
	}

	if (g_nextgameState == GAMESTATE_CLEAR || g_nextgameState == GAMESTATE_GAMEOVER)
	{ // 次の状態がクリアかゲームオーバーになったら
		g_nCounterGameState--;			// カウントを減らす
		if (g_nCounterGameState <= 0)
		{ // 回り切ったら
			// ゲーム状態を次の状態に移す
			g_gameState = g_nextgameState;
		}

		switch (g_gameState)
		{
			// クリア状態
		case GAMESTATE_CLEAR:

			// 

			break;

			// ゲームオーバー状態
		case GAMESTATE_GAMEOVER:
			SetMode(MODE_RESULT);
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

	DrawBullet();

	// モデルの描画処理
	DrawModel();

	// エフェクトの描画処理
	DrawEffect();

	// 雪の描画処理
	DrawSnow();

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

	// 影の描画処理
	DrawShadow();

	if (g_bPause == true)
	{ // ポーズ中なら

		// ポーズメニューの描画処理
		DrawPause();
	}

}

//========================================
//	ゲームの状態設定
//========================================
void SetGameState(GAMESTATE state, int nCounter)
{
	g_gameState = GAMESTATE_NONE;

	g_nextgameState = state;

	g_nCounterGameState = nCounter;
}

//========================================
//	ゲームモードの設定
//========================================
void SetGameMode(GAMEMODE gamemode)
{
	g_gamemode = gamemode;
}

//========================================
//	ゲームの状態の取得
//========================================
GAMESTATE GetGameState(void)
{
	return g_gameState;
}

//========================================
//	次のゲームの状態の取得
//========================================
GAMESTATE GetNextGameState(void)
{
	return g_nextgameState;
}

//========================================
//	ゲームモードの取得
//========================================
GAMEMODE GetGameMode(void)
{
	return g_gamemode;
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