//=============================================================================
//
//	カメラ処理 [camera.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "camera.h"
#include "input.h"
#include "player.h"

#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define AROUNDCOUNTER			(60)	// 回り込みタイマー

//*****************************************************************************
// グローバル変数
//*****************************************************************************
Camera g_camera;				// カメラの情報
float fCameraDif;				// カメラの高さ
bool g_bEdiet;
bool g_bCameraChange;

//=============================================================================
//	カメラの初期化処理
//=============================================================================
void InitCamera(void)
{
	Player* pPlayer = GetPlayer();

	// 初期化
	g_camera.posV = D3DXVECTOR3(pPlayer->pos.x + 0.0f, 100.0f, pPlayer->pos.z + -200.0f);
	g_camera.posR = D3DXVECTOR3(pPlayer->pos.x + 0.0f, 50.0f, pPlayer->pos.z + 0.0f);
	g_camera.posU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	g_camera.posVDest = D3DXVECTOR3(pPlayer->pos.x + 0.0f, 100.0f, pPlayer->pos.z + -200.0f);
	g_camera.posRDest = D3DXVECTOR3(pPlayer->pos.x + 0.0f, 50.0f, pPlayer->pos.z + 0.0f);
	g_camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_camera.posRDest.x = pPlayer->pos.x - sinf(pPlayer->rot.y) * 15.0f;
	g_camera.posRDest.z = pPlayer->pos.z - cosf(pPlayer->rot.y) * 15.0f;

	g_camera.posVDest.x = g_camera.posRDest.x + sinf(g_camera.rot.y) * CAMERAPOSR_DIS;
	g_camera.posVDest.z = g_camera.posRDest.z + cosf(g_camera.rot.y) * CAMERAPOSR_DIS;

	g_camera.rot.y = pPlayer->rot.y - D3DX_PI;

	fCameraDif = 7500.0f;
	g_bEdiet = false;
	g_bCameraChange = true;
}

//=============================================================================
//	カメラの終了処理
//=============================================================================
void UninitCamera(void)
{

}

//=============================================================================
//	カメラの更新処理
//=============================================================================
void UpdateCamera(void)
{
	static int nAroundCounter = AROUNDCOUNTER;			// 回り込むまでのカウント
	float fRotMove = 0.0f, fRotDest = 0.0f, fRotDiff = 0.0f;

	PrintDebugProc("エディット : %d\n", g_bEdiet);
	PrintDebugProc("視点 = { %.2f, %.2f, %.2f }\n", g_camera.posV.x, g_camera.posV.y, g_camera.posV.z);

	if (GetKeyboardPress(DIK_Z) == true)
	{
		g_camera.rot.y += 0.05f;
	}

	if (GetKeyboardPress(DIK_C) == true)
	{
		g_camera.rot.y += -0.05f;
	}

	if (GetKeyboardTrigger(DIK_F9) == true)
	{
		g_bEdiet = g_bEdiet ? false : true;
		g_camera.posR = INIT_D3DXVEC3;
	}

	if (g_bEdiet == true)
	{
		if (GetKeyboardTrigger(DIK_8) == true)
		{
			g_bCameraChange = g_bCameraChange ? false : true;
		}

		if (g_bCameraChange == true)
		{
			g_camera.rot.y = -D3DX_PI;
			g_camera.posV = D3DXVECTOR3(0.0f, fCameraDif, -1.0f);
		}
		else
		{
			g_camera.posV = D3DXVECTOR3(sinf(g_camera.rot.y) * fCameraDif, 50.0f, cosf(g_camera.rot.y) * fCameraDif);
		}


		if (GetKeyboardPress(DIK_F8) == true)
		{
			fCameraDif += 100.0f;
		}

		if (GetKeyboardPress(DIK_F7) == true)
		{
			fCameraDif += -100.0f;
		}

		return;
	}

	g_camera.fCameraAngleVertical = atan2f(g_camera.posR.x - g_camera.posV.x, g_camera.posR.y - g_camera.posV.y);
	g_camera.fCameraAngleHorizonttal = atan2f(g_camera.posR.x - g_camera.posV.x, g_camera.posR.z - g_camera.posV.z);

	//PrintDebugProc("カメラアングル = { %.2f %.2f }\n", g_camera.fCameraAngleVertical, g_camera.fCameraAngleHorizonttal);

	// 角度を補正
	g_camera.rot.y = AngleNormalize(g_camera.rot.y);

	Player* pPlayer = GetPlayer();

	//g_camera.rot.y = pPlayer->rot.y - D3DX_PI;

	// 前方表示
	g_camera.posRDest.x = pPlayer->pos.x - sinf(pPlayer->rot.y) * 25.0f;
	//g_camera.posRDest.y = pPlayer->pos.y;
	g_camera.posRDest.z = pPlayer->pos.z - cosf(pPlayer->rot.y) * 25.0f;

	// 目的視点を目的注視点から離す
	g_camera.posVDest.x = g_camera.posRDest.x + sinf(g_camera.rot.y) * CAMERAPOSR_DIS;
	//g_camera.posVDest.y = g_camera.posRDest.y;
	g_camera.posVDest.z = g_camera.posRDest.z + cosf(g_camera.rot.y) * CAMERAPOSR_DIS;

	if (GetKeyboardPress(DIK_Q) == true)
	{
		g_camera.posRDest.y += 1.0f;
	}

	if (GetKeyboardPress(DIK_B) == true)
	{
		g_camera.posRDest.y += -1.0f;
	}

	if (GetKeyboardPress(DIK_E) == true)
	{
		g_camera.posVDest.y += 1.0f;
	}

	if (GetKeyboardPress(DIK_N) == true)
	{
		g_camera.posVDest.y += -1.0f;
	}

	// 注視点を目的注視点に移動
	g_camera.posR.x += (g_camera.posRDest.x - g_camera.posR.x) * CAMERA_INERTIA;
	g_camera.posR.y += (g_camera.posRDest.y - g_camera.posR.y) * CAMERA_INERTIA;
	g_camera.posR.z += (g_camera.posRDest.z - g_camera.posR.z) * CAMERA_INERTIA;

	// 視点を目的視点に移動
	g_camera.posV.x += (g_camera.posVDest.x - g_camera.posV.x) * CAMERA_INERTIA;
	g_camera.posV.y += (g_camera.posVDest.y - g_camera.posV.y) * CAMERA_INERTIA;
	g_camera.posV.z += (g_camera.posVDest.z - g_camera.posV.z) * CAMERA_INERTIA;

	if (pPlayer->fMoveKeyboard == 0)
	{
		nAroundCounter--;
		//if (nAroundCounter <= 0)
		//{
		//	AngleNormalize(g_camera.rot.y);

		//	AngleNormalize(pPlayer->rot.y);
		//	
		//	float ftest = (pPlayer->rot.y - g_camera.rot.y);

		//	AngleNormalize(ftest);

		//	g_camera.rot.y += ftest * CAMERA_INERTIA;

		//	//if (pPlayer->rot.y - g_camera.rot.y < 0.25f)
		//	//{
		//	//	g_camera.rot.y = pPlayer->rot.y;
		//	//}

		//	AngleNormalize(g_camera.rot.y);
		//}
	}
	else
	{
		nAroundCounter = AROUNDCOUNTER;
	}

}

//=============================================================================
//	カメラの設定処理
//=============================================================================
void SetCamera(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// プロジェクションマトリックスを初期化
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	// プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&g_camera.mtxProjection,
							D3DXToRadian(45.0f),
							(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
							10.0f,
							10000.0f);

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);

	// ビューマトリックスを初期化
	D3DXMatrixIdentity(&g_camera.mtxView);

	// ビューマトリックスを作成
	D3DXMatrixLookAtLH(&g_camera.mtxView,
					   &g_camera.posV,
					   &g_camera.posR,
					   &g_camera.posU);
						
	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);
}

//=============================================================================
//	カメラの取得処理
//=============================================================================
Camera *GetCamera(void)
{
	return &g_camera;
}