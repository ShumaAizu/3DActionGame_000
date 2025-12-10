//=============================================================================
//
// サウンド処理 [sound.h]
// Author : SHUMA AIZU
//          AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// サウンド一覧
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM000 = 0,				// BGM0
	SOUND_LABEL_BGM001,					// BGM1
	SOUND_LABEL_BGM002,					// BGM2
	SOUND_LABEL_BEEP000,				// ビープ音
	SOUND_LABEL_JUMPSE000,				// ジャンプSE
	SOUND_LABEL_CLICKSE000,				// クリックSE
	SOUND_LABEL_CLEARSE000,				// クリアSE
	SOUND_LABEL_TROUBLESHOOTINGSE000,	// トラブルシューティングSE
	SOUND_LABEL_PAUSESE000,				// ポーズSE
	SOUND_LABEL_RETURNSE000,			// 決定SE
	SOUND_LABEL_SELECTSE000,			// 選択SE
	SOUND_LABEL_ITEMSE000,				// アイテムSE
	SOUND_LABEL_ENEMYSE000,				// 敵SE
	SOUND_LABEL_PLAYERSE000,			// プレイヤーSE
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);
bool CheckSound(SOUND_LABEL label);

#endif
