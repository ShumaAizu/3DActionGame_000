//=============================================================================
//
//	タイトル画面処理 [title.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _TITLE_H_
#define _TITLE_H_

#include "main.h"

//*****************************************************************************
// タイトルの状態
//*****************************************************************************
typedef enum
{
	TITLESTATE_OP = 0,			// オープニング
	TITLESTATE_WAIT,			// 待機
	TITLESTATE_MENU,			// メニュー
	TITLESTATE_SELECTLEVEL,		// 難易度選択
	TITLESTATE_MAX
}TITLESTATE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);
void SetTitleState(TITLESTATE state);
TITLESTATE GetTitleState(void);

#endif
