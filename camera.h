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
#define CAMERAPOSR_DIS			(175.0f)									// 視点と注視点との距離
#define CAMERA_INERTIA			(0.115f)									// カメラ移動の慣性係数
#define CAMERA_SPEED			(0.005f)									// カメラの移動速度
#define CAMERAINERTIA_TITLE		(0.025f)									// カメラ移動の慣性係数(タイトル)
#define CAMERAINERTIA_RESULT	(0.025f)									// カメラ移動の慣性係数(リザルト)
#define INITCAMERA_POSU			(D3DXVECTOR3(0.0f, 1.0f, 0.0f))				// カメラのベクトル初期値
#define INITCAMERA_TITLEV		(D3DXVECTOR3(-15.0f, 50.0f, -75.0f))		// タイトルの視点の初期値
#define INITCAMERA_TITLER		(D3DXVECTOR3(0.0f, 25.0f, 0.0f))			// タイトルの注視点の初期値
#define INITCAMERA_TITLEDESTV	(D3DXVECTOR3(-1750.0f, 750.0f, -4000.0f))	// タイトルの目的視点の初期値
#define INITCAMERA_TITLEDESTR	(D3DXVECTOR3(0.0f, 100.0f, 0.0f))			// タイトルの目的注視点の初期値
#define INITCAMERA_RESULTV		(D3DXVECTOR3(0.0f, 30.0f, -32.5f))			// リザルトの視点の初期値
#define INITCAMERA_RESULTR		(D3DXVECTOR3(0.0f, 15.0f, 0.0f))			// リザルトの注視点の初期値
#define INITCAMERA_RESULTDESTV	(D3DXVECTOR3(0.0f, 75.0f, -150.0f))			// リザルトの目的視点の初期値
#define INITCAMERA_RESULTDESTR	(D3DXVECTOR3(0.0f, 100.0f, 0.0f))			// リザルトの目的注視点の初期値

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
	D3DXVECTOR3 move;				// 移動量
	D3DXVECTOR3 rot;				// 向き
	D3DXVECTOR3 rotDest;			// 目的の向き
	D3DXVECTOR3 rotOld;				// 過去の向き
	float fRadiusVertical;			// 縦軸距離(基本)
	float fRadiusHorizonttal;		// 横軸距離
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
void UpdateTitleCamera(void);
void UpdateGameCamera(void);
void UpdateResultCamera(void);
void SetCamera(void);
void SetModeCamera(MODE mode);
Camera* GetCamera(void);
void CameraUpdateswitch(bool bFlag);

void* ReturnFunc(MODE mode);
#endif