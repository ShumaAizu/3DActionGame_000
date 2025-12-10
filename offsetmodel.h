//=============================================================================
//
//	モデル処理 [offsetmodel.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _OFFSETMODEL_H_
#define _OFFSETMODEL_H_

#include "main.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_OFFSETMODELTEX			(16)		// 読み込むテクスチャの最大数
#define MAX_OFFSETMODEL				(2)			// パーツの最大数
#define MAX_KEY						(2)			// キーの最大数
#define MAX_MOTION					(2)			// モーションの最大数

//*****************************************************************************
// モーションの種類
//*****************************************************************************
typedef enum
{
	MOTIONTYPE_NEUTRAL = 0,		// 待機
	MOTIONTYPE_MOVE,			// 移動
	MOTIONTYPE_ACTION,			// アクション
	MOTIONTYPE_JUMP,			// ジャンプ
	MOTIONTYPE_LANDING,			// 着地
	MOTIONTYPE_MAX
}MOTIONTYPE;

//*****************************************************************************
// キー要素の構造体
//*****************************************************************************
typedef struct
{
	float fPosX;				// 位置X
	float fPosY;				// 位置Y
	float fPosZ;				// 位置Z
	float fRotX;				// 向きX
	float fRotY;				// 向きY
	float fRotZ;				// 向きZ
}KEY;

//*****************************************************************************
// キー情報の構造体
//*****************************************************************************
typedef struct
{
	int nFrame;					// 再生フレーム
	KEY aKey[MAX_OFFSETMODEL];	// 各パーツのキー要素
}KEY_INFO;

//*****************************************************************************
// モーション情報の構造体
//*****************************************************************************
typedef struct
{
	bool bLoop;
	int nNumKey;
	KEY_INFO aKeyInfo[MAX_KEY];
}MOTION_INFO;

//*****************************************************************************
// モデルの構造体定義
//*****************************************************************************
typedef struct
{
	LPDIRECT3DTEXTURE9 apTexture[MAX_OFFSETMODELTEX];		// テクスチャへのポインタ
	LPD3DXMESH pMesh;										// メッシュへのポインタ
	LPD3DXBUFFER pBuffMat;									// マテリアルへのポインタ
	DWORD dwNumMat;											// マテリアルの数
	int nIdxModelParent;									// 親モデルのインデックス

	D3DXVECTOR3 pos;										// 位置
	D3DXVECTOR3 OffPos;										// オフセット位置
	D3DXVECTOR3 rot;										// 向き
	D3DXVECTOR3 OffRot;										// オフセット向き
	D3DXMATRIX mtxWorld;									// ワールドマトリックス
}OffSetModel;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitOffSetModel(void);
void UninitOffSetModel(void);
void UpdateOffSetModel(void);
void DrawOffSetModel(void);
void CollisionOffSetModel(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax);

#endif