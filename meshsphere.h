//=============================================================================
//
//	メッシュスフィア処理 [meshsphere.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _MESHSPHERE_H_
#define _MESHSPHERE_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MESHSPHERE_WIDTH			(100.0f)			// メッシュスフィアの幅
#define MESHSPHERE_HEIGHT			(100.0f)			// メッシュスフィアの高さ
#define MAX_MESHSPHERE				(4)					// メッシュスフィアの最大数

//*****************************************************************************
// メッシュスフィアの構造体定義
//*****************************************************************************
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;			// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 pIdxBuff;			// インデックスバッファへのポインタ
	D3DXVECTOR3 pos;							// 位置
	D3DXVECTOR3 rot;							// 向き
	D3DXMATRIX mtxWorld;						// ワールドマトリックス
	float fRadius;								// 半径
	bool bUse;									// 使用状態
}MeshSphere;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitMeshSphere(void);
void UninitMeshSphere(void);
void UpdateMeshSphere(void);
void DrawMeshSphere(void);
void SetMeshSphere(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

#endif