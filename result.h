//=============================================================================
//
//	リザルト画面処理 [result.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"

//*****************************************************************************
// リザルトの状態
//*****************************************************************************
typedef enum
{
	RESULTSTATE_NONE = 0,		// なにもない
	RESULTSTATE_RESULT,			// 結果発表
	RESULTSTATE_MAX
}RESULTSTATE;

//*****************************************************************************
// リザルトでの仲間の情報
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 向き
}ResultFriendsInfo;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitResult(void);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);
RESULTSTATE GetResultState(void);
void SetResultState(RESULTSTATE state);
void SetResultFriendsInfo(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

#endif