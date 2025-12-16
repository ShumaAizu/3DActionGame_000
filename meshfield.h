//=============================================================================
//
//	メッシュフィールド処理 [meshfield.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MESHFIELD_WIDTH				(100.0f)			// メッシュフィールドの幅
#define MESHFIELD_DEPTH				(100.0f)			// メッシュフィールドの奥行き
#define ICE_INERTIA					(0.0275f)			// 氷面歩行時の慣性
#define SNOW_INERTIA				(0.0525f)				// 雪面歩行時の慣性

//*****************************************************************************
// メッシュフィールドの種類
//*****************************************************************************
typedef enum
{
	MESHFIELDTYPE_ICE = 0,
	MESHFIELDTYPE_SNOW,
	MESHFIELDTYPE_SEA,
	MESHFIELDTYPE_MAX
}MESHFIELDTYPE;

//*****************************************************************************
// メッシュフィールドの構造体定義
//*****************************************************************************
typedef struct MeshField
{
	D3DXVECTOR3 pos;							// 位置
	D3DXVECTOR3 rot;							// 向き
	MESHFIELDTYPE type;							// 種類
	int nSplitWidth;							// 幅の分割数
	int nSplitDepth;							// 奥行きの分割数
	float fWidth;								// 幅
	float fDepth;								// 奥行き
	bool bUse;									// 使用状態
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;			// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 pIdxBuff;			// インデックスバッファへのポインタ
	D3DXMATRIX mtxWorld;						// ワールドマトリックス
}MeshField;

typedef struct MeshField* PMESHFIELD;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitMeshField(void);
void UninitMeshField(void);
void UpdateMeshField(void);
void DrawMeshField(void);
void SetMeshField(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fDepth, int nSplitWidth, int nSplitDepth, MESHFIELDTYPE type);
PMESHFIELD CollisionMeshField(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove);
void SetMesh(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nVtx, D3DXVECTOR3* VtxPos, int nSplitWidth, int nSplitDepth, MESHFIELDTYPE type);
void LoadMesh(const char* pFileName);

#endif