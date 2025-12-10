//=============================================================================
//
//	メッシュシリンダー処理 [meshcylinder.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _MESHCYLINDER_H_
#define _MESHCYLINDER_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MESHCYLINDER_RADIUS			(4500.0f)			// メッシュシリンダーの半径
#define MESHCYLINDER_HEIGHT			(500.0f)			// メッシュシリンダーの高さ
#define MAX_MESHCYLINDER			(4)					// メッシュシリンダーの最大数

//*****************************************************************************
// メッシュシリンダーの構造体定義
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
}MeshCylinder;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitMeshCylinder(void);
void UninitMeshCylinder(void);
void UpdateMeshCylinder(void);
void DrawMeshCylinder(void);
void SetMeshCylinder(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

#endif