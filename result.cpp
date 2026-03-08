//=============================================================================
//
//	リザルト画面処理 [result.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "result.h"
#include "loadscript.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "game.h"
#include "camera.h"
#include "resultmenu.h"
#include "resultui.h"
#include "player.h"
#include "friends.h"

#include "model.h"
#include "meshfield.h"
#include "meshcylinder.h"
#include "meshdome.h"
#include "camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_RESULT			(2)			// リザルトテクスチャの枚数
#define RESULTFADE_TIMER	(300)		// タイマーの秒数

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureResult[MAX_RESULT] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;		// 頂点バッファへのポインタ
int g_nResultFadeCounter = NULL;						// 画面遷移までのカウント
int g_bResultEvent = true;								// リザルトイベントを行っているか
RESULTSTATE g_ResultState;								// リザルトの状態
ResultFriendsInfo g_aResultFriendsInfo[MAX_FRIENDS];	// 仲間の情報
int g_nNumResultFriendsInfo;							// 仲間の情報数
int g_nGetNumFriends;									// 仲間の数

//====================================
//	リザルトの初期化処理
//====================================
void InitResult(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_nResultFadeCounter = 0;

	g_nNumResultFriendsInfo = 0;

	g_nGetNumFriends = GetNumFriends();

	g_bResultEvent = true;

	// リザルトUIの初期化処理
	InitResultUI();

	// プレイヤーの初期化処理
	InitPlayer(MODE_RESULT);

	// 仲間の初期化処理
	InitFriends();

	// リザルトスクリプト読み込み
	LoadScript(RESULT_SCRIPT);

	for (int nCntFriends = 0; nCntFriends < GetNumResultFriends(); nCntFriends++)
	{
		SetFriendsResult(&g_aResultFriendsInfo[nCntFriends]);
	}

	SetMotion(MOTIONTYPE_NEUTRAL, true, false, 0);

	g_ResultState = RESULTSTATE_NONE;

	CameraUpdateswitch(true);
}

//====================================
//	リザルトの終了処理
//====================================
void UninitResult(void)
{
	// サウンドを止める
	StopSound();

	// リザルトUIの終了処理
	UninitResultUI();

	// プレイヤーの終了処理
	UninitPlayer();

	// 仲間の終了処理
	UninitFriends();
}

//====================================
//	リザルトの描画処理
//====================================
void DrawResult(void)
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

	// リザルトUIの描画処理
	DrawResultUI();
}

//====================================
//	リザルトの更新処理
//====================================
void UpdateResult(void)
{
	// リザルトUIの更新処理
	UpdateResultUI();

	// プレイヤーの更新処理
	UpdatePlayer(MODE_RESULT);

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

	if ((GetKeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_A) == true) && g_ResultState == RESULTSTATE_RESULT)
	{
		SetFade(MODE_TITLE, COLOR_WHITE, DEFAULT_FADESPEED, DEFAULT_FADESPEED);
	}
}

//=============================================================================
//	リザルトイベント取得処理
//=============================================================================
RESULTSTATE GetResultState(void)
{
	return g_ResultState;
}

//=============================================================================
//	リザルトイベント設定処理
//=============================================================================
void SetResultState(RESULTSTATE state)
{
	g_ResultState = state;
}

//=============================================================================
//	リザルトでの仲間の情報設定処理
//=============================================================================
void SetResultFriendsInfo(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	g_aResultFriendsInfo[g_nNumResultFriendsInfo].pos = pos;
	g_aResultFriendsInfo[g_nNumResultFriendsInfo].rot = rot * D3DX_PI;

	g_nNumResultFriendsInfo++;
}