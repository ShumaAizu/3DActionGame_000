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
// プロトタイプ宣言
//*****************************************************************************
HRESULT LoadScript(const char* pScriptFileName);
HRESULT LoadMotionInfo(const char* pMotionFileName);
void LoadEnableString(char* aStrCpy, char* pStart);

#endif	_LOADSCRIPT_H_