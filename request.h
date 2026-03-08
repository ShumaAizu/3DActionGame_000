//=============================================================================
//
//	リクエスト処理 [request.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _REQUEST_H_
#define _REQUEST_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define REQUEST_WIDTH				(25.0f)			// リクエストの幅
#define REQUEST_HEIGHT				(25.0f)			// リクエストの高さ

//*****************************************************************************
// リクエストの種類
//*****************************************************************************
typedef enum
{
	REQUESTTYPE_000 = 0,
	REQUESTTYPE_TUTORIAL,
	REQUESTTYPE_002,
	REQUESTTYPE_MAX
}REQUESTTYPE;

//*****************************************************************************
// リクエストの構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;							// 位置
	D3DXVECTOR3 move;							// 移動量
	D3DXCOLOR col;								// 色
	float fRadius;								// 半径
	D3DXMATRIX mtxWorld;						// ワールドマトリックス
	REQUESTTYPE type;							// 種類
	int nIdxShadow;								// 影のインデックス
	int nLife;									// 寿命
	bool bDisp;									// 描画状態
	bool bUse;									// 使用状態
}Request;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitRequest(void);
void UninitRequest(void);
void UpdateRequest(void);
void DrawRequest(void);
int SetRequest(D3DXVECTOR3 pos, REQUESTTYPE type);
void SetPositionReqest(int nIdxReqest, D3DXVECTOR3 pos, D3DXVECTOR3 rot);
void ReleaseReqest(int nIdxReqest);
void SetRequestDisp(int nIdxRequest, bool bDisp);

#endif