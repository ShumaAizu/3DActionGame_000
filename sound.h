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
	SOUND_LABEL_TITLEBGM000 = 0,				// タイトルBGM
	SOUND_LABEL_TUTORIALBGM000,					// チュートリアルBGM
	SOUND_LABEL_GAMEBGM000,						// ゲームBGM
	SOUND_LABEL_RESULTBGM000,					// リザルトBGM
	SOUND_LABEL_WALKSE000,						// 歩行音
	SOUND_LABEL_JUMPSE000,						// ジャンプ音
	SOUND_LABEL_SELECT000,						// 選択音
	SOUND_LABEL_SELECT001,						// 選択決定音
	SOUND_LABEL_FALLSE000,						// 落下音
	SOUND_LABEL_REACTIONSE000,					// リアクション
	SOUND_LABEL_BUTREACTIONSE001,				// バットリアクション
	SOUND_LABEL_GETSE000,						// ゲット
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
