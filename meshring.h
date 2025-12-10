//=============================================================================
//
//	メッシュリング処理 [meshring.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _MESHRING_H_
#define _MESHRING_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MESHRING_WIDTH			(100.0f)			// メッシュリングの幅
#define MESHRING_HEIGHT			(100.0f)			// メッシュリングの高さ
#define MAX_MESHRING			(4)					// メッシュリングの最大数

//*****************************************************************************
// メッシュリングの構造体定義
//*****************************************************************************
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;			// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 pIdxBuff;			// インデックスバッファへのポインタ
	D3DXVECTOR3 pos;							// 位置
	D3DXVECTOR3 rot;							// 向き
	D3DXMATRIX mtxWorld;						// ワールドマトリックス
	float fInRadius;							// 内側の半径
	float fOutRadius;							// 外側の半径
	bool bUse;									// 使用状態
}MeshRing;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitMeshRing(void);
void UninitMeshRing(void);
void UpdateMeshRing(void);
void DrawMeshRing(void);
void SetMeshRing(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

#endif