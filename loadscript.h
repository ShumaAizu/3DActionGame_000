//=============================================================================
//
//	スクリプト読み込み処理 [loadsclipt.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _LOADSCRIPT_H_
#define _LOADSCRIPT_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TITLE_SCRIPT		"data/SCRIPTS/title.txt"	// タイトル管理スクリプト
#define GAME_SCRIPT			"data/SCRIPTS/game.txt"		// ゲーム管理スクリプト
#define RESULT_SCRIPT		"data/SCRIPTS/result.txt"	// リザルト管理スクリプト
#define STAGE_SCRIPT		"data/SCRIPTS/stage.txt"	// ステージ情報スクリプト
#define OBJECT_SCRIPT		"data/SCRIPTS/object.txt"	// オブジェクト情報スクリプト

//*****************************************************************************
// オブジェクトの種類
//*****************************************************************************
typedef enum
{
	OBJECTTYPE_PLAYER = 0,
	OBJECTTYPE_FRIENDS,
	OBJECTTYPE_MAX
}OBJECTTYPE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT LoadScript(const char* pScriptFileName);
HRESULT LoadMotionInfo(const char* pMotionFileName, OBJECTTYPE type);
HRESULT LoadStage(const char* pStageFileName);
HRESULT LoadObject(const char* pObjectFileName);
void LoadEnableString(char* aStrCpy, char* pStart);

#endif	_LOADSCRIPT_H_