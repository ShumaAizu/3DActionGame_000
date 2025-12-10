//=============================================================================
//
//	メッシュウォール処理 [meshwall.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _MESHWALL_H_
#define _MESHWALL_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MESHWALL_WIDTH			(100.0f)			// メッシュウォールの幅
#define MESHWALL_HEIGHT			(100.0f)			// メッシュウォールの高さ
#define MAX_MESHWALL			(64)				// メッシュウォールの最大数

//*****************************************************************************
// メッシュウォールの構造体定義
//*****************************************************************************
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;			// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 pIdxBuff;			// インデックスバッファへのポインタ
	D3DXVECTOR3 pos;							// 位置
	D3DXVECTOR3 rot;							// 向き
	D3DXMATRIX mtxWorld;						// ワールドマトリックス
	int nSplitWidth;							// 横の分割数
	int nSplitHeight;							// 縦の分割数
	bool bUse;									// 使用状態
}MeshWall;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitMeshWall(void);
void UninitMeshWall(void);
void UpdateMeshWall(void);
void DrawMeshWall(void);
void SetMeshWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, int nSplitWidth, int nSplitHeight);
void CollisionMeshWall(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove);
bool CollisionDot(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pRot);

#endif