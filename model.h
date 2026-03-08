//=============================================================================
//
//	モデル処理 [model.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"
#include "meshfield.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODEL_WIDTH				(5.0f)			// モデルの幅
#define MODEL_HEIGHT			(5.0f)			// モデルの高さ
#define MAX_MODELTEX			(16)			// 読み込むテクスチャの最大数

//*****************************************************************************
// モデルの種類
//*****************************************************************************
typedef enum
{
	MODELTYPE_000 = 0,
	MODELTYPE_001,
	MODELTYPE_002,
	MODELTYPE_003,
	MODELTYPE_004,
	MODELTYPE_005,
	MODELTYPE_006,
	MODELTYPE_007,
	MODELTYPE_008,
	MODELTYPE_009,
	MODELTYPE_010,
	MODELTYPE_011,
	MODELTYPE_012,
	MODELTYPE_013,
	MODELTYPE_014,
	MODELTYPE_015,
	MODELTYPE_016,
	MODELTYPE_017,
	MODELTYPE_018,
	MODELTYPE_019,
	MODELTYPE_020,
	MODELTYPE_021,
	MODELTYPE_MAX
}MODELTYPE;

//*****************************************************************************
// モデルデータの構造体定義
//*****************************************************************************
typedef struct
{
	LPDIRECT3DTEXTURE9 apTexture[MAX_MODELTEX];	// テクスチャへのポインタ
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
typedef struct Model
{
	D3DXVECTOR3 pos;							// 位置
	D3DXVECTOR3 rot;							// 向き
	D3DXVECTOR3 scale;							// 大きさ
	D3DXMATRIX mtxWorld;						// ワールドマトリックス
	MODELTYPE modeltype;						// モデルの種類
	bool bCollision;							// 当たり判定するかどうか
	bool bUse;									// 使用状態
	MODE mode;									// どのモードで使用しているか
}Model;

typedef struct Model* PMODEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitModel(void);
void UninitModel(void);
void UninitModeModel(MODE mode);
void UpdateModel(void);
void DrawModel(void);
void ReuseModel(MODE mode);
void CollisionModel(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax);
void CollisionMeshModelTest(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld);
void LoadModelData(const char* pModelFile);
void SetModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, MODELTYPE modeltype, MODE mode, bool bCollision, const char* pMeshFile, MESHFIELDTYPE meshtype);

#endif