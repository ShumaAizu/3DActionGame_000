//=============================================================================
//
//	アイテム処理 [item.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _ITEM_H_
#define _ITEM_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ITEM_WIDTH				(5.0f)			// アイテムの幅
#define ITEM_HEIGHT				(5.0f)			// アイテムの高さ
#define MAX_ITEMTEX				(16)			// 読み込むテクスチャの最大数
#define ITEM_FLOATINGSPEED		(0.04f)			// ぷかぷか速度

//*****************************************************************************
// アイテムの種類
//*****************************************************************************
typedef enum
{
	ITEMTYPE_000 = 0,
	ITEMTYPE_001,
	ITEMTYPE_002,
	ITEMTYPE_MAX
}ITEMTYPE;

//*****************************************************************************
// アイテムデータの構造体定義
//*****************************************************************************
typedef struct
{
	LPDIRECT3DTEXTURE9 apTexture[MAX_ITEMTEX];	// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;			// 頂点バッファへのポインタ
	LPD3DXMESH pMesh;							// メッシュへのポインタ
	LPD3DXBUFFER pBuffMat;						// マテリアルへのポインタ
	DWORD dwNumMat;								// マテリアルの数
	D3DXVECTOR3 vtxMin;							// 頂点位置の最小値
	D3DXVECTOR3 vtxMax;							// 頂点位置の最大値
	DWORD dwSizeFVF;							// 頂点フォーマットのサイズ
	int nNumVtx;								// 頂点数
	int nNumIdx;								// インデックス数
}Itemdata;

//*****************************************************************************
// アイテムの構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;							// 位置
	D3DXVECTOR3 rot;							// 向き
	D3DXVECTOR3 scale;							// 大きさ
	D3DXMATRIX mtxWorld;						// ワールドマトリックス
	ITEMTYPE itemtype;							// アイテムの種類
	float fRadius;								// 半径
	float fAngle;								// ぷかぷかで使う角度
	bool bCollision;							// 当たり判定するかどうか
	bool bUse;									// 使用状態
}Item;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);
void CollisionItem(D3DXVECTOR3* pPos, float fRadius);
void LoadItemData(const char* pItemFile);
void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 rot, ITEMTYPE itemtype, bool bCollision);
void SetCurrentItem(void);
ITEMTYPE GetItem(void);

#endif