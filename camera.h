//=============================================================================
//
//	カメラの処理 [camera.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CAMERAPOSR_DIS		(200.0f)		// 視点と注視点との距離
#define CAMERA_INERTIA		(0.115f)		// カメラ移動の慣性係数

//*****************************************************************************
// カメラの構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 posV;				// 視点
	D3DXVECTOR3 posR;				// 注視点
	D3DXVECTOR3 posU;				// 上方向ベクトル
	D3DXVECTOR3 posVDest;			// 目的の視点
	D3DXVECTOR3 posRDest;			// 目的の注視点
	D3DXVECTOR3 rot;				// 向き
	D3DXVECTOR3 rotDest;			// 目的の向き
	float fCameraAngleVertical;		// 縦軸のカメラの向き
	float fCameraAngleHorizonttal;	// 横軸のカメラの向き
	D3DXMATRIX mtxProjection;		// プロジェクションマトリックス
	D3DXMATRIX mtxView;				// ビューマトリックス
}Camera;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(void);
Camera* GetCamera(void);

#endif