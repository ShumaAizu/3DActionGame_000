//=============================================================================
//
//	モデル処理 [model.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODEL_WIDTH				(5.0f)			// モデルの幅
#define MODEL_HEIGHT			(5.0f)			// モデルの高さ
#define MAX_MODELTEX			(16)			// 読み込むテクスチャの最大数
#define MAX_IDX					(512)		// 

//*****************************************************************************
// モデルの種類
//*****************************************************************************
typedef enum
{
	MODELTYPE_000 = 0,
	MODELTYPE_001,
	MODELTYPE_002,
	MODELTYPE_MAX
}MODELTYPE;

//*****************************************************************************
// モデルデータの構造体定義
//*****************************************************************************
typedef struct
{
	LPDIRECT3DTEXTURE9 apTexture[MAX_MODELTEX];	// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;			// 頂点バッファへのポインタ
	LPD3DXMESH pMesh;							// メッシュへのポインタ
	LPD3DXBUFFER pBuffMat;						// マテリアルへのポインタ
	DWORD dwNumMat;								// マテリアルの数
	D3DXVECTOR3 vtxMin;							// 頂点位置の最小値
	D3DXVECTOR3 vtxMax;							// 頂点位置の最大値
	DWORD dwSizeFVF;							// 頂点フォーマットのサイズ
	int nNumVtx;								// 頂点数
	int nNumIdx;								// インデックス数
}Modeldata;

//*****************************************************************************
// モデルの構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;							// 位置
	D3DXVECTOR3 rot;							// 向き
	D3DXVECTOR3 scale;							// 大きさ
	D3DXMATRIX mtxWorld;						// ワールドマトリックス
	MODELTYPE modeltype;						// モデルの種類
	bool bCollision;							// 当たり判定するかどうか
	bool bUse;									// 使用状態
}Model;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);
void CollisionModel(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax);
void CollisionMeshModelTest(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld);
void SetModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, MODELTYPE modeltype);

#endif