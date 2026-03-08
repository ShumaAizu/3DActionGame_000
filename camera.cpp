//=============================================================================
//
//	カメラ処理 [camera.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "camera.h"
#include "input.h"
#include "player.h"
#include "model.h"
#include "title.h"
#include "result.h"

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
int g_nModeChangeCounter;		// モード切替カウンター
bool g_bEdiet;
bool g_bCameraChange;
bool g_bCameraUpdate;			// カメラ更新可能か

//*****************************************************************************
// 関数ポインタの定義
//*****************************************************************************

// カメラの更新関数
void (*UpdateModeCamera[MODE_MAX])(void) =
{
	&UpdateTitleCamera,
	NULL,
	&UpdateGameCamera,
	&UpdateResultCamera
};

//=============================================================================
//	カメラの初期化処理
//=============================================================================
void InitCamera(void)
{
	memset(&g_camera, NULL, sizeof(Camera));

	g_bCameraUpdate = true;

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
	PrintDebugProc("視点 = { %.2f, %.2f, %.2f }\n", g_camera.posV.x, g_camera.posV.y, g_camera.posV.z);

#ifdef _DEBUG
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
#endif

	// モードに合わせたカメラ更新処理
	if (UpdateModeCamera[GetMode()] != NULL && g_bCameraUpdate == true)
	{
		UpdateModeCamera[GetMode()]();
	}
}

//=============================================================================
//	カメラの更新処理 (タイトル)
//=============================================================================
void UpdateTitleCamera(void)
{
	g_nModeChangeCounter--;		// カウントを回す
	D3DXVECTOR3 posV, posVDest;	// 各ポジションコピー

	if ((GetKeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_START) == true ||
		GetJoypadTrigger(JOYKEY_A) == true) && GetTitleState() == TITLESTATE_WAIT)
	{// スキップ用
		g_nModeChangeCounter = 0;
		g_camera.posV = g_camera.posVDest;
	}

	if (g_nModeChangeCounter <= 0)
	{// カウント終了
		// 視点を目的視点に移動
		g_camera.posV.x += (g_camera.posVDest.x - g_camera.posV.x) * CAMERAINERTIA_TITLE;
		g_camera.posV.y += (g_camera.posVDest.y - g_camera.posV.y) * CAMERAINERTIA_TITLE;
		g_camera.posV.z += (g_camera.posVDest.z - g_camera.posV.z) * CAMERAINERTIA_TITLE;

		// 位置をコピー
		posV = g_camera.posV;
		posVDest = g_camera.posVDest;

		// 絶対値化
		posV.x = fabsf(posV.x);
		posV.y = fabsf(posV.y);
		posV.z = fabsf(posV.z);
		posVDest.x = fabsf(posVDest.x);
		posVDest.y = fabsf(posVDest.y);
		posVDest.z = fabsf(posVDest.z);

		if (posV.x > posVDest.x * 0.95f && posV.y > posVDest.y * 0.95f && posV.z > posVDest.z * 0.95f && GetTitleState() == TITLESTATE_WAIT)
		{// 十分近づいてたらモード切替
			SetTitleState(TITLESTATE_OP);
		}
	}
}

//=============================================================================
//	カメラの更新処理 (ゲーム)
//=============================================================================
void UpdateGameCamera(void)
{
	static int nAroundCounter = AROUNDCOUNTER;			// 回り込むまでのカウント
	float fRotMove = 0.0f, fRotDest = 0.0f, fRotDiff = 0.0f;
	D3DXVECTOR3 move = {};

	g_camera.rotOld = g_camera.rot;

	if (GetKeyboardPress(DIK_Z) == true || GetJoypadStickPress(JOYSTICKR_RIGHT) == true)
	{
		g_camera.rot.y += 0.025f;
	}

	if (GetKeyboardPress(DIK_C) == true || GetJoypadStickPress(JOYSTICKR_LEFT) == true)
	{
		g_camera.rot.y += -0.025f;
	}

	if (GetKeyboardPress(DIK_E) == true || GetJoypadStickPress(JOYSTICKR_UP) == true)
	{
		if (g_camera.rot.x > -D3DX_PI * 0.25f)
		{
			g_camera.rot.x += -0.025f;
		}
	}

	if (GetKeyboardPress(DIK_Q) == true || GetJoypadStickPress(JOYSTICKR_DOWN) == true)
	{
		if (g_camera.rot.x < D3DX_PI * 0.25f)
		{
			g_camera.rot.x += 0.025f;
		}
	}

	// 角度を補正
	g_camera.rot.y = AngleNormalize(g_camera.rot.y);

	Player* pPlayer = GetPlayer();

	D3DXVECTOR3 diff;

	diff = D3DXVECTOR3(sinf(0.0f) * g_camera.fRadiusVertical, cosf(g_camera.rot.x) * g_camera.fRadiusVertical, cosf(0.0f) * g_camera.fRadiusVertical);

	// 球の半径を計算
	g_camera.fRadiusHorizonttal = SQRTF(diff.x, diff.y);

	// 前方表示
	g_camera.posRDest.x = pPlayer->pos.x - sinf(pPlayer->rot.y) * 25.0f;
	g_camera.posRDest.y = pPlayer->pos.y + 50.0f;
	g_camera.posRDest.z = pPlayer->pos.z - cosf(pPlayer->rot.y) * 25.0f;

	// 目的視点を目的注視点から離す
	g_camera.posVDest.x = g_camera.posRDest.x + sinf(g_camera.rot.y) * g_camera.fRadiusHorizonttal;
	g_camera.posVDest.y = g_camera.posRDest.y + sinf(g_camera.rot.x) * g_camera.fRadiusVertical;
	g_camera.posVDest.z = g_camera.posRDest.z + cosf(g_camera.rot.y) * g_camera.fRadiusHorizonttal;

	CollisionModel(&g_camera.posVDest, &g_camera.posV, NULL, INIT_D3DXVEC3, INIT_D3DXVEC3);

	CollisionMeshField(&g_camera.posVDest, &g_camera.posV, &move);

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
//	カメラの更新処理 (リザルト)
//=============================================================================
void UpdateResultCamera(void)
{
	g_nModeChangeCounter--;		// カウントを回す
	D3DXVECTOR3 posV, posVDest;	// 各ポジションコピー

	if ((GetKeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_START) == true || GetJoypadTrigger(JOYKEY_A) == true))
	{// スキップ用
		g_nModeChangeCounter = 0;
		g_camera.posV = g_camera.posVDest;
	}

	if (g_nModeChangeCounter <= 0)
	{// カウント終了
		// 視点を目的視点に移動
		g_camera.posV.x += (g_camera.posVDest.x - g_camera.posV.x) * CAMERAINERTIA_RESULT;
		g_camera.posV.y += (g_camera.posVDest.y - g_camera.posV.y) * CAMERAINERTIA_RESULT;
		g_camera.posV.z += (g_camera.posVDest.z - g_camera.posV.z) * CAMERAINERTIA_RESULT;

		// 位置をコピー
		posV = g_camera.posV;
		posVDest = g_camera.posVDest;

		// 絶対値化
		posV.x = fabsf(posV.x);
		posV.y = fabsf(posV.y);
		posV.z = fabsf(posV.z);
		posVDest.x = fabsf(posVDest.x);
		posVDest.y = fabsf(posVDest.y);
		posVDest.z = fabsf(posVDest.z);

		if (posV.x >= posVDest.x * 0.95f && posV.y >= posVDest.y * 0.95f && posV.z >= posVDest.z * 0.95f)
		{// 十分近づいてたらモード切替
			if (GetResultState() == RESULTSTATE_NONE)
			{
				PlaySound(SOUND_LABEL_RESULTBGM000);
			}
			SetResultState(RESULTSTATE_RESULT);
		}
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
//	カメラのモード毎設定処理
//=============================================================================
void SetModeCamera(MODE mode)
{
	Player* pPlayer = GetPlayer();

	switch (mode)
	{
	case MODE_TITLE:
		// 各種位置を設定
		g_camera.posV = INITCAMERA_TITLEV;
		g_camera.posR = INITCAMERA_TITLER;
		g_camera.posU = INITCAMERA_POSU;
		g_camera.posVDest = INITCAMERA_TITLEDESTV;
		g_camera.posRDest = INITCAMERA_TITLEDESTR;
		g_nModeChangeCounter = 120;
		break;

	case MODE_GAME:
		// 初期位置をプレイヤーに合わせる
		g_camera.posV = D3DXVECTOR3(pPlayer->pos.x + 0.0f, 100.0f, pPlayer->pos.z + -200.0f);
		g_camera.posR = D3DXVECTOR3(pPlayer->pos.x + 0.0f, 50.0f, pPlayer->pos.z + 0.0f);
		g_camera.posU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		g_camera.posVDest = D3DXVECTOR3(pPlayer->pos.x + 0.0f, pPlayer->pos.y + 100.0f, pPlayer->pos.z + -200.0f);
		g_camera.posRDest = D3DXVECTOR3(pPlayer->pos.x + 0.0f, pPlayer->pos.y + 50.0f, pPlayer->pos.z + 0.0f);
		g_camera.fRadiusVertical = CAMERAPOSR_DIS;
		g_camera.fRadiusHorizonttal = CAMERAPOSR_DIS;

		g_camera.posRDest.x = pPlayer->pos.x - sinf(pPlayer->rot.y) * 15.0f;
		g_camera.posRDest.z = pPlayer->pos.z - cosf(pPlayer->rot.y) * 15.0f;

		g_camera.posVDest.x = g_camera.posRDest.x + sinf(g_camera.rot.y) * g_camera.fRadiusHorizonttal;
		g_camera.posVDest.z = g_camera.posRDest.z + cosf(g_camera.rot.y) * g_camera.fRadiusHorizonttal;

		g_camera.rot.y = pPlayer->rot.y - D3DX_PI;
		break;

	case MODE_RESULT:
		g_camera.posV = INITCAMERA_RESULTV;
		g_camera.posR = INITCAMERA_RESULTR;
		g_camera.posU = INITCAMERA_POSU;
		g_camera.posVDest = INITCAMERA_RESULTDESTV;
		g_camera.posRDest = INITCAMERA_RESULTDESTR;
		g_nModeChangeCounter = 60;
		break;
	}
}

//=============================================================================
//	カメラの取得処理
//=============================================================================
Camera *GetCamera(void)
{
	return &g_camera;
}

//=============================================================================
//	カメラの更新切り替え処理
//=============================================================================
void CameraUpdateswitch(bool bFlag)
{
	g_bCameraUpdate = bFlag;
}

void* ReturnFunc(MODE mode)
{
	return &(UpdateModeCamera[mode]);
}