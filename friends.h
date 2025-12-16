//=============================================================================
//
//	仲間たちの処理 [friends.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _FRIENDS_H_
#define _FRIENDS_H_

#include "main.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PENGUIN_RADIUS			(15.0f)			// ペンギンの半径
#define SNOWMAN_RADIUS			(15.0f)			// 雪だるまの半径
#define FRIENDS_SPEED			(0.025f)		// 仲間の速度
#define MAX_FRIENDS				(15)			// 仲間の最大数

//*****************************************************************************
// 仲間の状態
//*****************************************************************************
typedef enum
{
	FRIENDSSTATE_NUETRAL = 0,		// 通常状態
	FRIENDSSTATE_EVENT,				// イベント状態
	FRIENDSSTATE_FRIENDS,			// フレンズ状態
	FRIENDSSTATE_MAX
}FRIENDSSTATE;

//*****************************************************************************
// 仲間の種類
//*****************************************************************************
typedef enum
{
	FRIENDSTYPE_000 = 0, 
	FRIENDSTYPE_001,
	FRIENDSTYPE_002,
	FRIENDSTYPE_MAX
}FRIENDSTYPE;

//*****************************************************************************
// 仲間のイベント
//*****************************************************************************
typedef enum
{
	FRIENDEVENT_000 = 0,
	FRIENDEVENT_001,
	FRIENDEVENT_002,
	FRIENDEVENT_MAX
}FRIENDEVENT;

//*****************************************************************************
// 仲間の構造体定義
//*****************************************************************************
typedef struct Friends
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
	D3DXVECTOR3 *posDest;						// 目的位置
	D3DXVECTOR3 rot;							// 向き
	D3DXVECTOR3 rotDest;						// 目的向き
	D3DXVECTOR3 vecDest;						// 目的向きベクトル
	D3DXVECTOR3 move;							// 移動量
	FRIENDSSTATE state;							// 状態
	FRIENDSTYPE type;							// 種類
	FRIENDEVENT event;							// イベント
	float fRadius;								// 半径
	int nChangeCounter;							// モード切替カウンター
	int nPosLogCounter;							// ログを記録するまでのカウント
	float fInertia;								// 慣性
	float fSpeed;								// 速度
	bool bJump;									// ジャンプ状態
	bool bUse;									// 使用状態
	int nIdxShadow;								// 影のインデックス
	int nIdxPosLog;								// 位置記録のインデックス
	PMESHFIELD pRideField;						// 乗っているフィールド

	D3DXMATRIX mtxWorld;						// ワールドマトリックス
}Friends;

typedef struct Friends* PFRIENDS;

//*****************************************************************************
// 仲間の情報構造体定義
//*****************************************************************************
typedef struct FriendsInfo
{
	OffSetModel aOffSetModel[MAX_OFFSETMODEL];	// モデル(パーツ)
	int nNumOffSetModel;						// モデル(パーツ)の総数
	MOTION_INFO aMotionInfo[MAX_MOTION];		// モーション情報
	int nNumMotion;								// モーションの総数

}FriendsInfo;

typedef struct FriendsInfo* PFRIENDSINFO;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitFriends(void);
void UninitFriends(void);
void UpdateFriends(void);
void DrawFriends(void);
Friends* GetFriends(void);
void SetMotionFriends(MOTIONTYPE motiontype, bool bLoopMotion, bool bBlendMotion, int nFrameBlend, Friends* pFriends);
void UpdateMotionFriends(Friends* pFriends);
void SetFriends(D3DXVECTOR3 pos, D3DXVECTOR3 rot, FRIENDSTYPE type, FRIENDEVENT event);
void LoadPartsFriends(const char* pPartsFile);
void LoadCharacterFriends(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nIdxModel, int nIdxModelParent);
void LoadMotionFriends(bool bLoop, int nNumKey, KEY_INFO* pKeyInfo, int nMotion);
void NumFriendsAdd(void);
bool CollisionFriends(D3DXVECTOR3* pPos, float fRadius);

#endif