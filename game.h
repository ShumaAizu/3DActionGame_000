//=============================================================================
//
//	ゲーム画面処理 [game.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_WARD			(256)				// 文字列を読み込める最大

//*****************************************************************************
// ゲームの状態
//*****************************************************************************
typedef enum
{
	GAMESTATE_NONE = 0,				// 何もしていない状態
	GAMESTATE_NORMAL,				// 通常状態
	GAMESTATE_CLEAR,				// クリア状態
	GAMESTATE_GAMEOVER,				// ゲームオーバー状態
	GAMESTATE_MAX
}GAMESTATE;

//*****************************************************************************
// ゲームイベント
//*****************************************************************************
typedef enum
{
	GAMEEVENT_NONE = 0,				// 何もない
	GAMEEVENT_CLEAR,				// クリアイベント
	GAMEEVENT_MAX
}GAMEEVENT;

//*****************************************************************************
// ゲームの難易度
//*****************************************************************************
typedef enum
{
	GAMEMODE_NORMAL = 0,
	GAMEMODE_HARD,
	GAMEMODE_MAX
}GAMEMODE;

//*****************************************************************************
// 移動方向
//*****************************************************************************
typedef enum
{
	MOVEROT_NONE = 0,
	MOVEROT_UP,
	MOVEROT_DOWN,
	MOVEROT_LEFT,
	MOVEROT_RIGHT,
	MOVEROT_MAX
}MOVEROT;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);

void SetGameState(GAMESTATE state, int nCounter);
void SetGameMode(GAMEMODE gamemode);
GAMESTATE GetGameState(void);
GAMESTATE GetNextGameState(void);
GAMEMODE GetGameMode(void);

bool GetPause(void);
void SetEnablePause(bool bPause);

void LoadStage(void);

#endif