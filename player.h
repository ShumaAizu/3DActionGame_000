//=============================================================================
//
//	プレイヤー処理 [player.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "meshfield.h"
#include "offsetmodel.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PLAYER_RADIUS			(15.0f)				// プレイヤーの半径
#define PLAYER_SPEED			(0.115f)			// プレイヤーの速さ
#define MAX_LOG					(15)				// 過去の位置の記録の最大数
#define POSLOGCOUNT				(15)				// ログを保存するまでのカウント

//*****************************************************************************
// プレイヤーの状態
//*****************************************************************************
typedef enum
{
	PLAYERSTATE_NUETRAL = 0,		// 通常状態
	PLAYERSTATE_MOVE,				// 移動状態
	PLAYERSTATE_JUMP,				// ジャンプ状態
	PLAYERSTATE_ACTIONMOVE,			// アクション移動状態
	PLAYERSTATE_EVENT,				// イベント状態
	PLAYERSTATE_MAX
}PLAYERSTATE;

//*****************************************************************************
// プレイヤーのイベント
//*****************************************************************************
typedef enum
{
	PLAYEREVENT_NONE = 0,
	PLAYEREVENT_FRIENDS,
	PLAYEREVENT_FALL,
	PLAYEREVENT_MAX
}PLAYEREVENT;

//*****************************************************************************
// プレイヤーの構造体定義
//*****************************************************************************
typedef struct
{
	OffSetModel aOffSetModel[MAX_OFFSETMODEL];	// モデル(パーツ)
	int nNumOffSetModel;						// モデル(パーツ)の総数
	MOTION_INFO aMotionInfo[MAX_MOTION];		// モーション情報
	int nNumMotion;								// モーションの総数
	MOTIONTYPE motiontype;						// 現在のモーションの種類
	bool bLoopMotion;							// 現在のループするかどうか
	int nNumKey;								// 現在のキーの総数
	int nKey;									// 現在の現在のキーNo.
	int nCounterMotion;							// 現在のモーションカウンター
	bool bFinishMotion;

	bool bBlendMotion;							// モーションブレンドがあるかどうか
	MOTIONTYPE motiontypeBlend;					// モーションブレンドのモーションの種類
	bool bLoopMotionBlend;						// モーションブレンドのループするかどうか
	int nNumKeyBlend;							// モーションブレンドのキーの総数
	int nKeyBlend;								// モーションブレンドの現在のキーNo.
	int nCounterMotionBlend;					// モーションブレンドのモーションカウンター

	int nFrameBlend;							// ブレンドフレーム数
	int nCounterBlend;							// ブレンドカウンター

	D3DXVECTOR3 pos;							// 現在の位置
	D3DXVECTOR3 posOld;							// 過去の位置
	D3DXVECTOR3 posOldRand;						// 最後の地上位置
	D3DXVECTOR3 posShadow;						// 影の位置
	D3DXVECTOR3 posOldShadow;					// 影の過去の位置
	D3DXVECTOR3 rot;							// 向き
	D3DXVECTOR3 move;							// 移動量
	PLAYERSTATE	state;							// 状態
	PLAYEREVENT event;							// イベント
	float fRadius;								// 半径
	int nNumFriends;							// 仲間の数
	int nChangeCounter;							// モード切替カウンター
	float fSpeed;								// 速度
	float fInertia;								// 慣性
	bool bJump;									// ジャンプ状態
	float fMoveKeyboard;						// キーボード操作による移動量
	int nIdxShadow;								// 影のインデックス
	int nIdxPosLog;								// 過去の位置のログのインデックス
	PMESHFIELD pRideField;						// 乗っているフィールド

	D3DXMATRIX mtxWorld;						// ワールドマトリックス
}Player;

//*****************************************************************************
// ログの構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	bool bJump;				// ジャンプ状態
}Log;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitPlayer(MODE mode);
void UninitPlayer(void);
void UpdatePlayer(MODE mode);
void UpdateResultPlayer(void);
void DrawPlayer(void);
Player* GetPlayer(void);
void SetMotion(MOTIONTYPE motiontype, bool bLoopMotion, bool bBlendMotion, int nFrameBlend);
void UpdateMotion(void);
void LoadPartsPlayer(const char* pPartsFile);
void LoadCharacterPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nIdxModel, int nIdxModelParent);
void LoadMotion(bool bLoop, int nNumKey, KEY_INFO* pKeyInfo, int nMotion);
void SetPlayerEvent(PLAYEREVENT event);
void PlayerEventController(void);
PLAYEREVENT GetPlayerEvent(void);
int FriendsAdd(D3DXVECTOR3** posDest);
void UpdatePosLog(int nIdxPosLog, D3DXVECTOR3 pos);
void PlayerSEController(SOUND_LABEL SE);

#endif