//=============================================================================
//
//	プレイヤー処理 [player.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "player.h"
#include "bullet.h"
#include "effect.h"
#include "particle.h"
#include "input.h"
#include "shadow.h"
#include "camera.h"
#include "model.h"
#include "meshwall.h"
#include "meshfield.h"

#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_PLAYERTEX		(16)		// 読み込むテクスチャの最大数
#define GRAVITY				(-0.25f)	// 重力
#define PLAYER_JUMP			(5.0f)		// ジャンプ力

//*****************************************************************************
// グローバル変数
//*****************************************************************************
Player g_player;						// プレイヤーの情報

static KEY_INFO g_aKeyNeutral[MAX_KEY] =
{
	{25, 
	{{0.0f, 0.0f, 0.0f, -0.50f, 0.0f, 0.0f}, 
	{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}}},
	{25, 
	{{0.0f, 0.0f, 0.0f, 0.50f, 0.0f, 0.0f},
	{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}}}
};

static KEY_INFO g_aKeyNeutral0[MAX_KEY] =
{
	{25,
	{{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -0.50f},
	{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}}},
	{25,
	{{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.50f},
	{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}}}
};

const char* c_apOffSetModelFileName[MAX_OFFSETMODEL] =
{
	"data\\MODEL\\01_head.x",
	"data\\MODEL\\99_hat.x",
	//"data\\MODEL\\snowman\\00_snowman_body.x",
	//"data\\MODEL\\snowman\\01_snowman_head.x",
	//"data\\MODEL\\snowman\\02_snowman_armR.x",
	//"data\\MODEL\\snowman\\03_snowman_armL.x"
};

//=============================================================================
//	プレイヤーの初期化処理
//=============================================================================
void InitPlayer(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 初期化
	g_player.pos = D3DXVECTOR3(0.0f, 0.0f, -1500.0f);
	g_player.posOld = D3DXVECTOR3(0.0f, 0.0f, -1500.0f);
	g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.fSpeed = PLAYER_SPEED;
	g_player.pRideField = NULL;
	g_player.nIdxShadow = -1;

	g_player.aMotionInfo[0].aKeyInfo[0] = g_aKeyNeutral[0];
	g_player.aMotionInfo[0].aKeyInfo[1] = g_aKeyNeutral[1];

	g_player.aMotionInfo[1].aKeyInfo[0] = g_aKeyNeutral0[0];
	g_player.aMotionInfo[1].aKeyInfo[1] = g_aKeyNeutral0[1];

	SetMotion(MOTIONTYPE_NEUTRAL, true, false, 0);

	// 影の設定
	g_player.nIdxShadow = SetShadow();

	D3DXMATERIAL* pMat;

	for (int nCntOffSetModel = 0; nCntOffSetModel < MAX_OFFSETMODEL; nCntOffSetModel++)
	{
		// Xファイルの読み込み
		D3DXLoadMeshFromX(c_apOffSetModelFileName[nCntOffSetModel],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_player.aOffSetModel[nCntOffSetModel].pBuffMat,
			NULL,
			&g_player.aOffSetModel[nCntOffSetModel].dwNumMat,
			&g_player.aOffSetModel[nCntOffSetModel].pMesh);

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_player.aOffSetModel[nCntOffSetModel].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_player.aOffSetModel[nCntOffSetModel].dwNumMat; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{// テクスチャファイルが存在する
				// テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_player.aOffSetModel[nCntOffSetModel].apTexture[nCntMat]);
			}
		}
	}

	g_player.nNumOffSetModel = MAX_OFFSETMODEL;

	// 各パーツの階層構造設定
	g_player.aOffSetModel[0].nIdxModelParent = -1;						// 親モデルのインデックスを設定
	g_player.aOffSetModel[0].OffPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// パーツ[0]の位置(オフセット)
	g_player.aOffSetModel[0].pos = g_player.aOffSetModel[0].OffPos;		// パーツ[0]の位置(オフセット)
	g_player.aOffSetModel[0].OffRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.aOffSetModel[0].rot = g_player.aOffSetModel[0].OffRot;		// パーツ[0]の向き

	g_player.aOffSetModel[1].nIdxModelParent = 0;						// 親モデルのインデックスを設定
	g_player.aOffSetModel[1].OffPos = D3DXVECTOR3(0.0f, 15.0f, 0.0f);	// パーツ[1]の位置(オフセット)
	g_player.aOffSetModel[1].pos = g_player.aOffSetModel[1].OffPos;		// パーツ[1]の位置(オフセット)
	g_player.aOffSetModel[1].OffRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.aOffSetModel[1].rot = g_player.aOffSetModel[1].OffRot;		// パーツ[1]の向き

	//g_player.aOffSetModel[1].nIdxModelParent = 0;					// 親モデルのインデックスを設定
	//g_player.aOffSetModel[1].pos = D3DXVECTOR3(0.0f, 22.5f, 0.0f);	// パーツ[1]の位置(オフセット)
	//g_player.aOffSetModel[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// パーツ[1]の向き

	//g_player.aOffSetModel[2].nIdxModelParent = 0;					// 親モデルのインデックスを設定
	//g_player.aOffSetModel[2].pos = D3DXVECTOR3(-8.5f, 17.5f, 0.0f);	// パーツ[2]の位置(オフセット)
	//g_player.aOffSetModel[2].rot = D3DXVECTOR3(0.0f, 0.0f, 0.90f);	// パーツ[2]の向き

	//g_player.aOffSetModel[3].nIdxModelParent = 0;					// 親モデルのインデックスを設定
	//g_player.aOffSetModel[3].pos = D3DXVECTOR3(8.5f, 17.5f, 0.0f);	// パーツ[3]の位置(オフセット)
	//g_player.aOffSetModel[3].rot = D3DXVECTOR3(0.0f, 0.0f, -0.90f);	// パーツ[3]の向き
}

//=============================================================================
//	プレイヤーの終了処理
//=============================================================================
void UninitPlayer(void)
{
	for (int nCntOffSetModel = 0; nCntOffSetModel < MAX_OFFSETMODEL; nCntOffSetModel++)
	{
		// メッシュの破棄
		if (g_player.aOffSetModel[nCntOffSetModel].pMesh != NULL)
		{
			g_player.aOffSetModel[nCntOffSetModel].pMesh->Release();
			g_player.aOffSetModel[nCntOffSetModel].pMesh = 0;
		}

		// マテリアルの破棄
		if (g_player.aOffSetModel[nCntOffSetModel].pBuffMat != NULL)
		{
			g_player.aOffSetModel[nCntOffSetModel].pBuffMat->Release();
			g_player.aOffSetModel[nCntOffSetModel].pBuffMat = NULL;
		}
	}
}

//=============================================================================
//	プレイヤーの描画処理
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用
	D3DXMATERIAL* pMat;							// マテリアルデータへのポインタ

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_player.mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_player.rot.y, g_player.rot.x, g_player.rot.z);
	D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_player.pos.x, g_player.pos.y, g_player.pos.z);
	D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_player.mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// 全モデル(パーツ)の描画
	for (int nCntOffSetModel = 0; nCntOffSetModel < MAX_OFFSETMODEL; nCntOffSetModel++)
	{
		D3DXMATRIX mtxRotOffSetModel, mtxTransOffSetModel;	// 計算用マトリックス
		D3DXMATRIX mtxParent;								// 親のマトリックス

		// パーツのワールドマトリックスを初期化
		D3DXMatrixIdentity(&g_player.aOffSetModel[nCntOffSetModel].mtxWorld);

		// パーツの向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRotOffSetModel, g_player.aOffSetModel[nCntOffSetModel].rot.y, g_player.aOffSetModel[nCntOffSetModel].rot.x, g_player.aOffSetModel[nCntOffSetModel].rot.z);
		D3DXMatrixMultiply(&g_player.aOffSetModel[nCntOffSetModel].mtxWorld, &g_player.aOffSetModel[nCntOffSetModel].mtxWorld, &mtxRotOffSetModel);

		// パーツの位置を反映(オフセット)
		D3DXMatrixTranslation(&mtxTransOffSetModel, g_player.aOffSetModel[nCntOffSetModel].pos.x, g_player.aOffSetModel[nCntOffSetModel].pos.y, g_player.aOffSetModel[nCntOffSetModel].pos.z);
		D3DXMatrixMultiply(&g_player.aOffSetModel[nCntOffSetModel].mtxWorld, &g_player.aOffSetModel[nCntOffSetModel].mtxWorld, &mtxTransOffSetModel);

		// パーツの「親のマトリックス」を設定
		if (g_player.aOffSetModel[nCntOffSetModel].nIdxModelParent != -1)
		{// 親モデルがある場合
			mtxParent = g_player.aOffSetModel[g_player.aOffSetModel[nCntOffSetModel].nIdxModelParent].mtxWorld;
		}
		else
		{// 親モデルがない場合
			mtxParent = g_player.mtxWorld;
		}

		// 算出した「パーツのワールドマトリックス」と「親のマトリックス」を掛け合わせる
		D3DXMatrixMultiply(&g_player.aOffSetModel[nCntOffSetModel].mtxWorld,
			&g_player.aOffSetModel[nCntOffSetModel].mtxWorld,
			&mtxParent);

		// パーツのワールドマトリックスを設定
		pDevice->SetTransform(D3DTS_WORLD, &g_player.aOffSetModel[nCntOffSetModel].mtxWorld);

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_player.aOffSetModel[nCntOffSetModel].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_player.aOffSetModel[nCntOffSetModel].dwNumMat; nCntMat++)
		{
			// マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_player.aOffSetModel[nCntOffSetModel].apTexture[nCntMat]);

			// プレイヤー(パーツ)の描画
			g_player.aOffSetModel[nCntOffSetModel].pMesh->DrawSubset(nCntMat);
		}
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
//	プレイヤーの更新処理
//=============================================================================
void UpdatePlayer(void)
{
	g_player.posOld = g_player.pos;

	PrintDebugProc("Player.pos = { %.2f, %.2f, %.2f }\n", g_player.pos.x, g_player.pos.y, g_player.pos.z);
	PrintDebugProc("Player.posOld = { %.2f, %.2f, %.2f }\n", g_player.posOld.x, g_player.posOld.y, g_player.posOld.z);

	Camera* pCamera = GetCamera();

	static float fRotMove = 0.0f, fRotDest = 0.0f, fRotDiff = 0.0f;

	D3DXVECTOR3 KeyboardMove = {0.0f, 0.0f, 0.0f};

	if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true)
	{//Aキーが押された
		KeyboardMove.x += 1;
	}

	if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true)
	{//Dキーが押された
		KeyboardMove.x -= 1;
	}

	if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true)
	{//Wキーが押された
		KeyboardMove.z -= 1;
	}

	if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true)
	{//Sキーが押された
		KeyboardMove.z += 1;
	}

	g_player.fMoveKeyboard = SQRTF(KeyboardMove.x, KeyboardMove.z);

	if (g_player.fMoveKeyboard != 0)
	{
		fRotMove = g_player.rot.y - D3DX_PI;									// 今の向き
		fRotDest = atan2f(KeyboardMove.x, KeyboardMove.z) + pCamera->rot.y;		// 目的地への向き

		// 移動量の更新
		g_player.move.x += sinf(fRotMove) * g_player.fSpeed;
		g_player.move.z += cosf(fRotMove) * g_player.fSpeed;
	}

	// 補正

	fRotMove = AngleNormalize(fRotMove);

	fRotDest = AngleNormalize(fRotDest);

	fRotDiff = fRotDest - fRotMove;		// 差分

	fRotDiff = AngleNormalize(fRotDiff);

	fRotMove += fRotDiff * 0.175f;

	fRotDiff = AngleNormalize(fRotDiff);

	g_player.rot.y = fRotMove - D3DX_PI;

	g_player.rot.y = AngleNormalize(g_player.rot.y);

	// 向き変更
	if (GetKeyboardPress(DIK_LSHIFT) == true)
	{
		g_player.rot.y += -0.1f;
	}

	if (GetKeyboardPress(DIK_RSHIFT) == true)
	{
		g_player.rot.y += 0.1f;
	}

	if (GetKeyboardRepeat(DIK_SPACE) == true && g_player.bJump == false)
	{
		SetBullet(g_player.pos, g_player.rot);
		g_player.move.y = PLAYER_JUMP;
		g_player.bJump = true;
	}

	if (GetKeyboardPress(DIK_5) == true)
	{
		g_player.aOffSetModel[1].rot.y += 0.1f;
		g_player.aOffSetModel[1].rot.y = AngleNormalize(g_player.aOffSetModel[1].rot.y);
	}

	if (GetKeyboardTrigger(DIK_0) == true)
	{
		SetMotion(MOTIONTYPE_NEUTRAL, true, true, 100);
	}
	if (GetKeyboardTrigger(DIK_1) == true)
	{
		SetMotion(MOTIONTYPE_MOVE, true, true, 100);
	}

	PrintDebugProc("現在のモーション : %d\n", g_player.motiontype);
	PrintDebugProc("ブレンドのモーション : %d\n", g_player.motiontypeBlend);

	g_player.move.y += GRAVITY;
	g_player.pos += g_player.move;		// 位置を更新

	if (g_player.pRideField != NULL)
	{
		switch (g_player.pRideField->type)
		{
		case MESHFIELDTYPE_ICE:
			//慣性を更新
			g_player.move.x += (0.0f - g_player.move.x) * ICE_INERTIA;
			g_player.move.z += (0.0f - g_player.move.z) * ICE_INERTIA;
			break;

		case MESHFIELDTYPE_SNOW:
			g_player.move.x += (0.0f - g_player.move.x) * SNOW_INERTIA;
			g_player.move.z += (0.0f - g_player.move.z) * SNOW_INERTIA;
			break;

		case MESHFIELDTYPE_SEA:
			break;
		}
	}

	//g_player.move.x = 0.0f;
	//g_player.move.z = 0.0f;

	if (g_player.pos.y < 0.0f)
	{
		g_player.move.y = GRAVITY;
		g_player.bJump = false;
	}

	// 壁との当たり判定
	CollisionMeshWall(&g_player.pos, &g_player.posOld, &g_player.move);

	g_player.pRideField = CollisionMeshField(&g_player.pos, &g_player.posOld, &g_player.move);

	//// 内積の当たり判定（テスト）
	//CollisionDot(&g_player.pos, &g_player.posOld, &g_player.move, &g_player.rot);

	// モデルとの当たり判定
	CollisionModel(&g_player.pos, &g_player.posOld, &g_player.move, D3DXVECTOR3(10.0f, 10.0f, 10.0f), D3DXVECTOR3(10.0f, 10.0f, 10.0f));

	CollisionMeshModelTest(&g_player.pos, &g_player.posOld);

	// 影の位置を更新
	SetPositionShadow(g_player.nIdxShadow, D3DXVECTOR3(g_player.pos.x, g_player.pos.y - g_player.pos.y, g_player.pos.z));

	// 全モデル更新
	UpdateMotion();

	if (GetKeyboardTrigger(DIK_5) == true)
	{
		SetEffect(D3DXVECTOR3(0.0f, 50.0f, 0.0f), INIT_D3DXVEC3, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 15.0f, 0.05f, 0.0f, 100);
	}

	//SetRainbowParticle(g_player.pos, 30.0f, 5, D3DX_PI, -D3DX_PI);

}

//=============================================================================
//	プレイヤーの取得処理
//=============================================================================
Player* GetPlayer(void)
{
	return &g_player;
}

//=============================================================================
//	モーションの設定処理
//=============================================================================
void SetMotion(MOTIONTYPE motiontype, bool bLoopMotion, bool bBlendMotion, int nFrameBlend)
{

	if (bBlendMotion == true)
	{
		g_player.bBlendMotion = true;
		g_player.nFrameBlend = nFrameBlend;
		g_player.nCounterBlend = 0;

		g_player.motiontypeBlend = motiontype;
		g_player.bLoopMotionBlend = bLoopMotion;
		g_player.nNumKeyBlend = MAX_KEY;
		g_player.nKeyBlend = 0;
		g_player.nCounterMotionBlend = 0;
	}
	else
	{
		g_player.nNumMotion = MAX_MOTION;
		g_player.motiontype = motiontype;
		g_player.bLoopMotion = bLoopMotion;
		g_player.nNumKey = MAX_KEY;
		g_player.nKey = 0;
		g_player.nCounterMotion = 0;

		OffSetModel* pModel = &g_player.aOffSetModel[0];
		KEY_INFO* pKeyInfo = &g_player.aMotionInfo[g_player.motiontype].aKeyInfo[0];

		for (int nCntModel = 0; nCntModel < g_player.nNumOffSetModel; nCntModel++, pModel++, pKeyInfo++)
		{// 全パーツの初期化
			pModel->pos.x = pKeyInfo->aKey[0].fPosX;
			pModel->pos.y = pKeyInfo->aKey[0].fPosY;
			pModel->pos.z = pKeyInfo->aKey[0].fPosZ;

			pModel->rot.x = pKeyInfo->aKey[0].fRotX;
			pModel->rot.y = pKeyInfo->aKey[0].fRotY;
			pModel->rot.z = pKeyInfo->aKey[0].fRotZ;

			pModel->pos += pModel->OffPos;
			pModel->rot += pModel->OffRot;
		}

	}
}

//=============================================================================
//	モーションの更新処理
//=============================================================================
void UpdateMotion(void)
{
	float fPosX, fPosY, fPosZ, fRotX, fRotY, fRotZ;
	float fDiffKey, fRateKey;

	// 全モデル(パーツ)の更新
	for (int nCntModel = 0; nCntModel < g_player.nNumOffSetModel; nCntModel++)
	{
		KEY DiffKey = {};
		KEY RateKey = {};

		// 現在のキー
		KEY* pKey = &g_player.aMotionInfo[g_player.motiontype].aKeyInfo[g_player.nKey].aKey[nCntModel];

		// 次のキー
		KEY *pKeyNext = &g_player.aMotionInfo[g_player.motiontype].aKeyInfo[(g_player.nKey + 1) % g_player.nNumKey].aKey[nCntModel];

		// 対象モデル
		OffSetModel* pModel = &g_player.aOffSetModel[nCntModel];

		// 最大フレームと現在のフレームの比率
		float fRateKey = (float)g_player.nCounterMotion / (float)g_player.aMotionInfo[g_player.motiontype].aKeyInfo[g_player.nKey].nFrame;

		if (g_player.bBlendMotion == true)
		{// ブレンドあり

			KEY DiffKeyBlend = {};
			KEY RateKeyBlend = {};
			float fPosXCurrent, fPosYCurrent, fPosZCurrent, fRotXCurrent, fRotYCurrent, fRotZCurrent;
			float fPosXBlend, fPosYBlend, fPosZBlend, fRotXBlend, fRotYBlend, fRotZBlend;
			float fDiffKeyBlend, fDiffBlend;

			// ブレンドモーションの現在のキー
			KEY* pKeyBlend = &g_player.aMotionInfo[g_player.motiontypeBlend].aKeyInfo[g_player.nKeyBlend].aKey[nCntModel];

			// ブレンドモーションの次のキー
			KEY *pKeyNextBlend = &g_player.aMotionInfo[g_player.motiontypeBlend].aKeyInfo[(g_player.nKeyBlend + 1) % g_player.nNumKeyBlend].aKey[nCntModel];

			// ブレンドモーションの最大フレームと現在のフレームの比率
			float fRateKeyBlend = (float)g_player.nCounterMotionBlend / (float)g_player.aMotionInfo[g_player.motiontypeBlend].aKeyInfo[g_player.nKeyBlend].nFrame;

			// モーションブレンドの最大フレームと現在のフレームの比率
			float fRateBlend = (float)g_player.nCounterBlend / (float)g_player.nFrameBlend;

			// 座標差分
			// X座標
			fDiffKey = pKeyNext->fPosX - pKey->fPosX;
			fPosXCurrent = pKey->fPosX + (fDiffKey * fRateKey);

			fDiffKeyBlend = pKeyNextBlend->fPosX - pKeyBlend->fPosX;
			fPosXBlend = pKeyBlend->fPosX + (fDiffKeyBlend * fRateKeyBlend);

			fDiffBlend = fPosXBlend - fPosXCurrent;

			fPosX = fPosXCurrent + (fDiffBlend * fRateBlend);

			// Y座標
			fDiffKey = pKeyNext->fPosY - pKey->fPosY;
			fPosYCurrent = pKey->fPosY + (fDiffKey * fRateKey);

			fDiffKeyBlend = pKeyNextBlend->fPosY - pKeyBlend->fPosY;
			fPosYBlend = pKeyBlend->fPosY + (fDiffKeyBlend * fRateKeyBlend);

			fDiffBlend = fPosYBlend - fPosYCurrent;

			fPosY = fPosYCurrent + (fDiffBlend * fRateBlend);

			// Z座標
			fDiffKey = pKeyNext->fPosZ - pKey->fPosZ;
			fPosZCurrent = pKey->fPosZ + (fDiffKey * fRateKey);

			fDiffKeyBlend = pKeyNextBlend->fPosZ - pKeyBlend->fPosZ;
			fPosZBlend = pKeyBlend->fPosZ + (fDiffKeyBlend * fRateKeyBlend);

			fDiffBlend = fPosZBlend - fPosZCurrent;

			fPosZ = fPosZCurrent + (fDiffBlend * fRateBlend);

			// 向き差分
			// X軸
			fDiffKey = pKeyNext->fRotX - pKey->fRotX;
			fRotXCurrent = pKey->fRotX + (fDiffKey * fRateKey);

			fDiffKeyBlend = pKeyNextBlend->fRotX - pKeyBlend->fRotX;
			fRotXBlend = pKeyBlend->fRotX + (fDiffKeyBlend * fRateKeyBlend);

			fDiffBlend = fRotXBlend - fRotXCurrent;

			fRotX = fRotXCurrent + (fDiffBlend * fRateBlend);

			fRotX = AngleNormalize(fRotX);

			// Y軸
			fDiffKey = pKeyNext->fRotY - pKey->fRotY;
			fRotYCurrent = pKey->fRotY + (fDiffKey * fRateKey);

			fDiffKeyBlend = pKeyNextBlend->fRotY - pKeyBlend->fRotY;
			fRotYBlend = pKeyBlend->fRotY + (fDiffKeyBlend * fRateKeyBlend);

			fDiffBlend = fRotYBlend - fRotYCurrent;

			fRotY = fRotYCurrent + (fDiffBlend * fRateBlend);

			fRotY = AngleNormalize(fRotY);

			// Z軸
			fDiffKey = pKeyNext->fRotZ - pKey->fRotZ;
			fRotZCurrent = pKey->fRotZ + (fDiffKey * fRateKey);

			fDiffKeyBlend = pKeyNextBlend->fRotZ - pKeyBlend->fRotZ;
			fRotZBlend = pKeyBlend->fRotZ + (fDiffKeyBlend * fRateKeyBlend);

			fDiffBlend = fRotZBlend - fRotZCurrent;

			fRotZ = fRotZCurrent + (fDiffBlend * fRateBlend);

			fRotY = AngleNormalize(fRotY);

			PrintDebugProc("現在 = { %.2f, %.2f %.2f }\n", fRotX, fRotY, fRotZ);
		}
		else
		{
			// 座標差分
			fDiffKey = pKeyNext->fPosX - pKey->fPosX;
			fPosX = pKey->fPosX + (fDiffKey * fRateKey);

			fDiffKey = pKeyNext->fPosY - pKey->fPosY;
			fPosY = pKey->fPosY + (fDiffKey * fRateKey);

			fDiffKey = pKeyNext->fPosZ - pKey->fPosZ;
			fPosZ = pKey->fPosZ + (fDiffKey * fRateKey);

			// 向き差分
			fDiffKey = pKeyNext->fRotX - pKey->fRotX;
			fRotX = AngleNormalize(pKey->fRotX + (fDiffKey * fRateKey));

			fDiffKey = pKeyNext->fRotY - pKey->fRotY;
			fRotY = AngleNormalize(pKey->fRotY + (fDiffKey * fRateKey));

			fDiffKey = pKeyNext->fRotZ - pKey->fRotZ;
			fRotZ = AngleNormalize(pKey->fRotZ + (fDiffKey * fRateKey));
		}

		// パーツの向きと位置を設定
		pModel->pos = D3DXVECTOR3(fPosX, fPosY, fPosZ);
		pModel->rot = D3DXVECTOR3(fRotX, fRotY, fRotZ);

		// オフセット位置加算
		pModel->pos += pModel->OffPos;
		pModel->rot += pModel->OffRot;

		// 補正
		pModel->rot.x = AngleNormalize(pModel->rot.x);
		pModel->rot.y = AngleNormalize(pModel->rot.y);
		pModel->rot.z = AngleNormalize(pModel->rot.z);
	}

	if (g_player.bBlendMotion == true)
	{// ブレンドあり
		// ブレンドモーションのカウンターを更新
		g_player.nCounterMotionBlend++;

		if (g_player.nCounterMotionBlend >= g_player.aMotionInfo[g_player.motiontypeBlend].aKeyInfo[g_player.nKeyBlend].nFrame)
		{
			g_player.nCounterMotionBlend = 0;
			g_player.nKeyBlend = (g_player.nKeyBlend + 1) % g_player.nNumKeyBlend;
		}

		// ブレンドカウンターを更新
		g_player.nCounterBlend++;
		if (g_player.nCounterBlend >= g_player.nFrameBlend)
		{
			g_player.nCounterBlend = 0;
			g_player.motiontype = g_player.motiontypeBlend;
			g_player.nCounterMotion = g_player.nCounterMotionBlend;
			g_player.nKey = g_player.nKeyBlend;
			g_player.bBlendMotion = false;
		}
	}
	else
	{// ブレンドなし
		g_player.nCounterMotion++;

		if (g_player.nCounterMotion >= g_player.aMotionInfo[g_player.motiontype].aKeyInfo[g_player.nKey].nFrame)
		{
			g_player.nCounterMotion = 0;
			g_player.nKey = (g_player.nKey + 1) % g_player.nNumKey;
		}
	}
}