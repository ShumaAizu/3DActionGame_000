//=============================================================================
//
//	リアクション処理 [reaction.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _REACTION_H_
#define _REACTION_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define REACTION_WIDTH				(25.0f)			// リアクションの幅
#define REACTION_HEIGHT				(25.0f)			// リアクションの高さ

//*****************************************************************************
// リアクションの種類
//*****************************************************************************
typedef enum
{
	REACTIONTYPE_000 = 0,
	REACTIONTYPE_001,
	REACTIONTYPE_002,
	REACTIONTYPE_MAX
}REACTIONTYPE;

//*****************************************************************************
// リアクションの構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;							// 位置
	D3DXVECTOR3 move;							// 移動量
	float fRadius;								// 半径
	D3DXMATRIX mtxWorld;						// ワールドマトリックス
	REACTIONTYPE type;							// 種類
	int nIdxShadow;								// 影のインデックス
	int nLife;									// 寿命
	bool bUse;									// 使用状態
}Reaction;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitReaction(void);
void UninitReaction(void);
void UpdateReaction(void);
void DrawReaction(void);
void SetReaction(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fRadius, REACTIONTYPE type);

#endif