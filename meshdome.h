//=============================================================================
//
//	メッシュドーム処理 [meshdome.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _MESHDOME_H_
#define _MESHDOME_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MESHDOME_RADIUS			(3000.0f)			// メッシュドームの半径
#define MAX_MESHDOME			(4)					// メッシュドームの最大数

//*****************************************************************************
// メッシュドームの構造体定義
//*****************************************************************************
typedef struct MeshDome
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;			// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 pIdxBuff;			// インデックスバッファへのポインタ
	D3DXVECTOR3 pos;							// 位置
	D3DXVECTOR3 rot;							// 向き
	D3DXVECTOR2 tex;							// テクスチャ座標
	D3DXMATRIX mtxWorld;						// ワールドマトリックス
	float fRadius;								// 半径
	int nVertical;								// 縦軸の分割数
	int nHorizontal;							// 横軸の分割数
	bool bUse;									// 使用状態
}MeshDome;

typedef struct MeshDome* PMESHDOME;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitMeshDome(void);
void UninitMeshDome(void);
void UpdateMeshDome(void);
void DrawMeshDome(void);
void SetMeshDome(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, int nVertical, int nHorizontal);

#endif