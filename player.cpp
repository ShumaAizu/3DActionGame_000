//=============================================================================
//
//	プレイヤー処理 [player.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "fade.h"
#include "sound.h"
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
#include "friends.h"
#include "item.h"

#include "result.h"

#include "snow.h"

#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_PLAYERTEX				(16)										// 読み込むテクスチャの最大数
#define GRAVITY						(-0.25f)									// 重力
#define PLAYER_JUMP					(5.0f)										// ジャンプ力
#define PLAYER_INIT_POS				(D3DXVECTOR3(0.0f, 8.2f, -750.0f))			// プレイヤーの初期位置
#define PLAYER_INIT_ROT				(D3DXVECTOR3(0.0f, 0.0f, 0.0f))				// プレイヤーの初期向き

#define PLAYER_INIT_POSTITLE		(D3DXVECTOR3(-1745.0f, 725.0f, -3925.0f))	// プレイヤーの初期位置 (タイトル)
#define PLAYER_INIT_ROTTITLE		(D3DXVECTOR3(0.0f, -0.75f * D3DX_PI, 0.0f))	// プレイヤーの初期向き (タイトル)

#define PLAYER_INIT_POSRESULT		(D3DXVECTOR3(0.0f, -6.25f, 0.0f))			// プレイヤーの初期位置 (リザルト)
#define PLAYER_INIT_ROTRESULT		(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f))			// プレイヤーの初期位置 (リザルト)
#define PLAYER_ROTDEST_RESULT		(0.0f)										// プレイヤーの目的向き (リザルト)
#define PLAYER_MOVESETIMER			(30)										// 足音がなるまでのタイマー

//*****************************************************************************
// グローバル変数
//*****************************************************************************
Player g_player;						// プレイヤーの情報
D3DXVECTOR3 g_posLog[MAX_LOG] = {};		// 過去の位置の記録
bool g_bFallFlag = false;				// 落下フラグ

//=============================================================================
//	プレイヤーの初期化処理
//=============================================================================
void InitPlayer(MODE mode)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 初期化
	memset(&g_player, NULL, sizeof(Player));

	g_bFallFlag = false;

	// 各種モードに合わせて
	switch (mode)
	{
	case MODE_GAME:
		g_player.pos = PLAYER_INIT_POS;
		g_player.rot = PLAYER_INIT_ROT;
		g_player.move = INIT_D3DXVEC3;
		g_player.fRadius = PLAYER_RADIUS;
		g_player.fSpeed = PLAYER_SPEED;
		g_player.nIdxShadow = -1;
		g_player.nIdxPosLog = 0;
		break;

	case MODE_TITLE:
		g_player.pos = PLAYER_INIT_POSTITLE;
		g_player.rot = PLAYER_INIT_ROTTITLE;
		g_player.fRadius = PLAYER_RADIUS;
		g_player.fSpeed = PLAYER_SPEED;
		g_player.nIdxShadow = -1;
		g_player.nIdxPosLog = 0;
		break;

	case MODE_RESULT:
		g_player.pos = PLAYER_INIT_POSRESULT;
		g_player.rot = PLAYER_INIT_ROTRESULT;
		g_player.fRadius = PLAYER_RADIUS;
		g_player.fSpeed = PLAYER_SPEED;
		g_player.nIdxShadow = -1;
		g_player.nIdxPosLog = 0;
		break;
	}

	// 影の設定
	g_player.nIdxShadow = SetShadow();
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
			g_player.aOffSetModel[nCntOffSetModel].pMesh = NULL;
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

	D3DXMATRIX mtxShadow;		// シャドウマトリックス
	D3DLIGHT9 light;			// ライトの情報
	D3DXVECTOR4 posLight;		// ライトの位置
	D3DXVECTOR3 pos, normal;	// 平面の点, 法線ベクトル
	D3DXPLANE plane;			// 平面の情報

	D3DXMATERIAL mat;

	// ライトの位置を設定
	pDevice->GetLight(1, &light);
	posLight = D3DXVECTOR4(-light.Direction.x, -light.Direction.y, -light.Direction.z, 1.0f);

	// 平面情報を生成
	pos = D3DXVECTOR3(0.0f, 100.0f, 0.0f);
	normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXPlaneFromPointNormal(&plane, &pos, &normal);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// 全モデル(パーツ)の描画
	for (int nCntOffSetModel = 0; nCntOffSetModel < g_player.nNumOffSetModel; nCntOffSetModel++)
	{
		D3DXMATRIX mtxRotOffSetModel, mtxTransOffSetModel;	// 計算用マトリックス
		D3DXMATRIX mtxParent;								// 親のマトリックス

		// ワールドマトリックスの初期化(デフォルトの値にする)
		D3DXMatrixIdentity(&g_player.mtxWorld);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_player.rot.y, g_player.rot.x, g_player.rot.z);
		D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_player.pos.x, g_player.pos.y, g_player.pos.z);
		D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxTrans);

		// ワールドマトリックスの初期化(デフォルトの値にする)
		D3DXMatrixIdentity(&mtxShadow);

		// シャドウマトリックスの生成
		D3DXMatrixShadow(&mtxShadow, &posLight, &plane);
		D3DXMatrixMultiply(&mtxShadow, &g_player.mtxWorld, &mtxShadow);

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

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// 全モデル(パーツ)の描画
	for (int nCntOffSetModel = 0; nCntOffSetModel < g_player.nNumOffSetModel; nCntOffSetModel++)
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
void UpdatePlayer(MODE mode)
{
	if (mode == MODE_TITLE || mode == MODE_RESULT)
	{
		UpdateMotion();

		switch (mode)
		{
		case MODE_RESULT:
			UpdateResultPlayer();
			break;
		}
		return;
	}

	static int nFrameCounter = 0;	// フレームカウント変数
	static int nMoveSECounter = PLAYER_MOVESETIMER - 1;

	g_player.posOld = g_player.pos;		// 過去の位置更新

	if (g_player.state == PLAYERSTATE_MOVE || g_player.state == PLAYERSTATE_JUMP)
	{
		nFrameCounter++;

		if (nFrameCounter > POSLOGCOUNT)
		{
			if (g_player.pRideField != NULL)
			{
				g_posLog[g_player.nIdxPosLog] = g_player.pos;

				nFrameCounter = 0;
			}
		}
	}

	PrintDebugProc("Player.pos = { %.2f, %.2f, %.2f }\n", g_player.pos.x, g_player.pos.y, g_player.pos.z);
	PrintDebugProc("Player.posOld = { %.2f, %.2f, %.2f }\n", g_player.posOld.x, g_player.posOld.y, g_player.posOld.z);
	PrintDebugProc("向き %.2f\n", g_player.rot.y);

	Camera* pCamera = GetCamera();

	if (g_player.event != PLAYEREVENT_FRIENDS)
	{
		static float fRotMove = 0.0f, fRotDest = 0.0f, fRotDiff = 0.0f;

		D3DXVECTOR3 KeyboardMove = { 0.0f, 0.0f, 0.0f };

		if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true)
		{// Aキーが押された
			KeyboardMove.x += -1;
		}

		if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true)
		{// Dキーが押された
			KeyboardMove.x += 1;
		}

		if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true)
		{// Wキーが押された
			KeyboardMove.z += 1;
		}

		if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true)
		{// Sキーが押された
			KeyboardMove.z += -1;
		}

		// 移動ベクトルを正規化
		D3DXVec3Normalize(&KeyboardMove, &KeyboardMove);

		// スティックの値をもらう(使っていたら優先)
		GetJoypadStickLeft(&KeyboardMove.x, &KeyboardMove.z);

		g_player.fMoveKeyboard = SQRTF(KeyboardMove.x, KeyboardMove.z);

		if (g_player.fMoveKeyboard != 0 && g_bFallFlag == false)
		{// 移動していたら
			fRotMove = g_player.rot.y;												// 今の向き
			fRotDest = atan2f(KeyboardMove.x, KeyboardMove.z) + pCamera->rot.y;		// 目的地への向き

			if (g_player.state != PLAYERSTATE_MOVE && g_player.state != PLAYERSTATE_JUMP)
			{
				SetMotion(MOTIONTYPE_MOVE, true, true, 10);
				g_player.state = PLAYERSTATE_MOVE;
			}

			// 目的向きに移動方向を合わせる
			KeyboardMove.x = sinf(fRotDest) * g_player.fMoveKeyboard;
			KeyboardMove.z = cosf(fRotDest) * g_player.fMoveKeyboard;

			// 移動量の更新
			g_player.move.x += (g_player.fSpeed * -KeyboardMove.x);
			g_player.move.z += (g_player.fSpeed * -KeyboardMove.z);
		}
		else
		{
			if (g_player.state == PLAYERSTATE_MOVE)
			{
				SetMotion(MOTIONTYPE_NEUTRAL, true, true, 10);
				g_player.state = PLAYERSTATE_NUETRAL;
			}
		}

		if (g_player.state == PLAYERSTATE_MOVE)
		{
			nMoveSECounter++;

			if (nMoveSECounter % 5 == 0)
			{
				SetEffect(g_player.pos + D3DXVECTOR3((float)(rand() % 5 - 3), 0.0f, (float)(rand() % 5 - 3)), g_player.move * -1.0f, D3DXCOLOR(0.68f, 0.85f, 0.9f, 1.0f), (float)(rand() % 5 + 3), 0.0025f, 0.0025f, 5);
			}
			if (nMoveSECounter % PLAYER_MOVESETIMER == 0)
			{
				PlaySound(SOUND_LABEL_WALKSE000);
				nMoveSECounter = 0;
			}

		}
		else
		{
			nMoveSECounter = PLAYER_MOVESETIMER - 1;
		}

		// 補正
		fRotMove = AngleNormalize(fRotMove);

		fRotDest = AngleNormalize(fRotDest);

		fRotDiff = fRotDest - fRotMove;		// 差分

		fRotDiff = AngleNormalize(fRotDiff);

		fRotMove += fRotDiff * 0.175f;

		fRotDiff = AngleNormalize(fRotDiff);

		g_player.rot.y = fRotMove;

		g_player.rot.y = AngleNormalize(g_player.rot.y);

		if ((GetKeyboardTrigger(DIK_SPACE) == true || GetJoypadTrigger(JOYKEY_A) == true) && g_player.bJump == false && g_bFallFlag == false)
		{// スペースキーが押されたかつジャンプ状態じゃない
			SetMotion(MOTIONTYPE_JUMP, true, false, 10);
			SetJoypadVibration(6000, 8000, 18);
			g_player.state = PLAYERSTATE_JUMP;
			g_player.move.y = PLAYER_JUMP;
			g_player.bJump = true;
			PlaySound(SOUND_LABEL_JUMPSE000);
		}

		g_player.move.y += GRAVITY;			// 重力をかける
		g_player.pos += g_player.move;		// 位置を更新

		// 慣性を更新
		g_player.move.x += (0.0f - g_player.move.x) * g_player.fInertia;
		g_player.move.z += (0.0f - g_player.move.z) * g_player.fInertia;

		// 壁との当たり判定
		CollisionMeshWall(&g_player.pos, &g_player.posOld, &g_player.move);

		// モデルとの当たり判定
		CollisionModel(&g_player.pos, &g_player.posOld, &g_player.move, D3DXVECTOR3(10.0f, 10.0f, 10.0f), D3DXVECTOR3(10.0f, 10.0f, 10.0f));

		if (g_bFallFlag != true)
		{
			// フィールドとの当たり判定
			g_player.pRideField = CollisionMeshField(&g_player.pos, &g_player.posOld, &g_player.move);
		}

		if (g_player.pRideField != NULL)
		{// もしフィールド上にいたら
			//　慣性を設定
			switch (g_player.pRideField->type)
			{
			case MESHFIELDTYPE_ICE:
				g_player.fInertia = ICE_INERTIA;
				break;

			case MESHFIELDTYPE_SNOW:
				g_player.fInertia = SNOW_INERTIA;
				break;

			case MESHFIELDTYPE_SEA:
				if (g_bFallFlag == false)
				{
					g_player.move.y += -5.0f;			// 重力をかける
					CameraUpdateswitch(false);
					ReleaseShadow(g_player.nIdxShadow);
					PlaySound(SOUND_LABEL_FALLSE000);
					g_bFallFlag = true;
				}
				break;

			case MESHFIELDTYPE_MODEL:
				g_player.fInertia = SNOW_INERTIA;
				break;
			}

			if (g_player.pRideField->type != MESHFIELDTYPE_SEA)
			{// 海以外に乗っていれば
				// 最後の着地点を記録
				g_player.posOldRand = g_player.pos;
			}

			// 影の位置を更新
			SetPositionShadow(g_player.nIdxShadow, D3DXVECTOR3(g_player.pos.x, g_player.pos.y, g_player.pos.z));

			if (g_player.state == PLAYERSTATE_JUMP || g_player.bJump == true)
			{// もしジャンプ中だったら
				SetMotion(MOTIONTYPE_LANDING, false, true, 10);		// 着地モーション
				g_player.state = PLAYERSTATE_NUETRAL;				// 状態更新
				g_player.bJump = false;								// ジャンプ状態更新
			}
		}
		else
		{
			// 影の位置を更新
			SetPositionShadow(g_player.nIdxShadow, D3DXVECTOR3(g_player.pos.x, g_player.posOldRand.y, g_player.pos.z));
		}
	}

	// 仲間との当たり判定
	CollisionFriends(&g_player.pos, g_player.fRadius);

	// アイテムとの当たり判定
	CollisionItem(&g_player.pos, g_player.fRadius);

	// イベント管理
	PlayerEventController();

	// 全モデル更新
	UpdateMotion();
}

//=============================================================================
//	プレイヤーの更新処理 (リザルト)
//=============================================================================
void UpdateResultPlayer(void)
{
	static float fRotDiff = 0.0f;

	fRotDiff = -PLAYER_ROTDEST_RESULT - g_player.rot.y;

	fRotDiff = AngleNormalize(fRotDiff);

	g_player.rot.y += fRotDiff * 0.0275f;

	g_player.rot.y = AngleNormalize(g_player.rot.y);

	if (GetResultState() == RESULTSTATE_RESULT)
	{
		g_player.rot.y = PLAYER_ROTDEST_RESULT;
	}

	g_player.move.y += GRAVITY;			// 重力をかける
	g_player.pos += g_player.move;		// 位置を更新

	// フィールドとの当たり判定
	g_player.pRideField = CollisionMeshField(&g_player.pos, &g_player.posOld, &g_player.move);

	if (g_player.pRideField != NULL)
	{// もしフィールド上にいたら
		//　慣性を設定
		switch (g_player.pRideField->type)
		{
		case MESHFIELDTYPE_ICE:
			g_player.fInertia = ICE_INERTIA;
			break;

		case MESHFIELDTYPE_SNOW:
			g_player.fInertia = SNOW_INERTIA;
			break;

		case MESHFIELDTYPE_SEA:
			if (g_bFallFlag == false)
			{
				g_player.move.y += -5.0f;			// 重力をかける
				CameraUpdateswitch(false);
				PlaySound(SOUND_LABEL_FALLSE000);
				g_bFallFlag = true;
			}
			break;

		case MESHFIELDTYPE_MODEL:
			g_player.fInertia = SNOW_INERTIA;
			break;
		}

		if (g_player.pRideField->type != MESHFIELDTYPE_SEA)
		{
			// 最後の着地点を記録
			g_player.posOldRand = g_player.pos;
		}

		// 影の位置を更新
		SetPositionShadow(g_player.nIdxShadow, D3DXVECTOR3(g_player.pos.x, g_player.pos.y, g_player.pos.z));
	}
	else
	{
		// 影の位置を更新
		SetPositionShadow(g_player.nIdxShadow, D3DXVECTOR3(g_player.pos.x, g_player.pos.y - g_player.pos.y, g_player.pos.z));
	}

	if (GetResultState() == RESULTSTATE_RESULT && g_player.motiontypeBlend != MOTIONTYPE_REACTION && g_player.motiontypeBlend != MOTIONTYPE_BUTREACTION)
	{
		if (GetNumResultFriends() <= 0)
		{
			SetMotion(MOTIONTYPE_BUTREACTION, true, true, 10);
		}
		else
		{
			SetMotion(MOTIONTYPE_REACTION, true, true, 10);
		}
	}
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
		g_player.nNumKeyBlend = g_player.aMotionInfo[motiontype].nNumKey;
		g_player.nKeyBlend = 0;
		g_player.nCounterMotionBlend = 0;
	}
	else
	{
		g_player.nNumKey = g_player.aMotionInfo[motiontype].nNumKey;
		g_player.motiontype = motiontype;
		g_player.bLoopMotion = bLoopMotion;
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
	float fDiffKey;

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

			// 現在の値格納用
			float fPosXCurrent, fRotXCurrent,	// 位置・向きX
				fPosYCurrent, fRotYCurrent, 	// 位置・向きY
				fPosZCurrent, fRotZCurrent;		// 位置・向きZ

			// ブレンド用
			float fPosXBlend, fRotXBlend,
				fPosYBlend, fRotYBlend, 
				fPosZBlend, fRotZBlend;	

			float fDiffKeyBlend, fDiffBlend;	// 差分

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
			// キーを進める
			g_player.nCounterMotionBlend = 0;	// カウンターリセット
			if ((g_player.nKeyBlend = (g_player.nKeyBlend + 1) % g_player.nNumKeyBlend) == 0 && g_player.bLoopMotionBlend == false)
			{// キーが終着点まで来たかつループ状態じゃなければ
				SetMotion(MOTIONTYPE_NEUTRAL, true, true, 10);
			}
		}

		// ブレンドカウンターを更新
		g_player.nCounterBlend++;
		if (g_player.nCounterBlend >= g_player.nFrameBlend)
		{// カウンターが回り切ったら
			g_player.nCounterBlend = 0;									// カウンターリセット
			g_player.motiontype = g_player.motiontypeBlend;				// 次のモーションに切り替える
			g_player.nCounterMotion = g_player.nCounterMotionBlend;		// モーションカウンターをブレンドと同期
			g_player.nNumKey = g_player.nNumKeyBlend;					// キーの総数をブレンドと同期
			g_player.nKey = g_player.nKeyBlend;							// キー番号をブレンドと同期
			g_player.bLoopMotion = g_player.bLoopMotionBlend;			// ループ状態をブレンドと同期
			g_player.bBlendMotion = false;								// ブレンド状態を終了
		}
	}
	else
	{// ブレンドなし
		g_player.nCounterMotion++;

		if (g_player.nCounterMotion >= g_player.aMotionInfo[g_player.motiontype].aKeyInfo[g_player.nKey].nFrame)
		{
			// キーを進める
			g_player.nCounterMotion = 0;	// カウンターリセット
			if((g_player.nKey = (g_player.nKey + 1) % g_player.nNumKey) == 0 && g_player.bLoopMotion == false)
			{// キーが終着点まで来たかつループ状態じゃなければ
				SetMotion(MOTIONTYPE_NEUTRAL, true, true, 10);
			}
		}
	}
}

//=============================================================================
//	プレイヤーのパーツ設定処理
//=============================================================================
void LoadPartsPlayer(const char* pPartsFile)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	D3DXMATERIAL* pMat;

	OffSetModel *pOffSetModel = &g_player.aOffSetModel[g_player.nNumOffSetModel];

	// Xファイルの読み込み
	D3DXLoadMeshFromX(pPartsFile,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&pOffSetModel->pBuffMat,
		NULL,
		&pOffSetModel->dwNumMat,
		&pOffSetModel->pMesh);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)pOffSetModel->pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)pOffSetModel->dwNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{// テクスチャファイルが存在する
			// テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,
				&pOffSetModel->apTexture[nCntMat]);
		}
	}

	g_player.nNumOffSetModel++;
}

//=============================================================================
//	プレイヤーのオフセット読み込み処理
//=============================================================================
void LoadCharacterPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nIdxModel, int nIdxModelParent)
{
	OffSetModel* pOffSetModel = &g_player.aOffSetModel[nIdxModel];

	pOffSetModel->nIdxModel = nIdxModel;
	pOffSetModel->nIdxModelParent = nIdxModelParent;
	pOffSetModel->pos = pOffSetModel->OffPos = pos;
	pOffSetModel->rot = pOffSetModel->OffRot = rot;
}

//=============================================================================
//	プレイヤーのモーション読み込み処理
//=============================================================================
void LoadMotion(bool bLoop, int nNumKey, KEY_INFO* pKeyInfo, int nMotion)
{
	MOTION_INFO* pMotionInfo = &g_player.aMotionInfo[nMotion];

	pMotionInfo->bLoop = bLoop;
	pMotionInfo->nNumKey = nNumKey;

	for (int nCntKeyInfo = 0; nCntKeyInfo < nNumKey; nCntKeyInfo++, pKeyInfo++)
	{
		pMotionInfo->aKeyInfo[nCntKeyInfo] = *pKeyInfo;
	}

	g_player.nNumMotion++;
}

//=============================================================================
//	プレイヤーの状態管理処理
//=============================================================================
void PlayerStateController(Player* pPlayer, PLAYERSTATE state)
{

}

//=============================================================================
//	仲間増加処理
//=============================================================================
int FriendsAdd(D3DXVECTOR3 **posDest)
{
	*posDest = &g_posLog[g_player.nNumFriends];
	g_player.nNumFriends++;
	return g_player.nNumFriends;
}

//=============================================================================
//	プレイヤーイベント処理
//=============================================================================
void SetPlayerEvent(PLAYEREVENT event)
{

	static int nCntFall = 0;
	g_player.event = event;

	switch (g_player.event)
	{
	case PLAYEREVENT_FRIENDS:
		SetMotion(MOTIONTYPE_REACTION, false, true, 10);
		g_player.move = INIT_D3DXVEC3;
		break;

	case PLAYEREVENT_FALL:
		SetNoneFade(COLOR_BLACK, DEFAULT_FADESPEED, DEFAULT_FADESPEED);
		break;
	}

	if (g_bFallFlag == true)
	{
		nCntFall++;
		if (nCntFall >= 60)
		{
			SetNoneFade(COLOR_BLACK, DEFAULT_FADESPEED, DEFAULT_FADESPEED);
		}
	}
}

//=============================================================================
//	プレイヤーイベント処理
//=============================================================================
void PlayerEventController(void)
{
	static int nCntFall = 0;

	if (g_bFallFlag == true)
	{
		nCntFall++;
		if (nCntFall >= 60)
		{
			nCntFall = 0;
			SetNoneFade(COLOR_BLACK, DEFAULT_FADESPEED, DEFAULT_FADESPEED);
		}
	}

	if (g_bFallFlag == true && GetFade() == FADE_IN)
	{
		g_player.pos = g_player.posOldRand;
		g_player.move = INIT_D3DXVEC3;
		g_player.nIdxShadow = SetShadow();
		CameraUpdateswitch(true);
		g_bFallFlag = false;
	}

	switch (g_player.event)
	{
	case PLAYEREVENT_FRIENDS:
		if (g_player.motiontypeBlend != MOTIONTYPE_REACTION)
		{
			g_player.event = PLAYEREVENT_NONE;
			g_player.state = PLAYERSTATE_NUETRAL;
		}
		break;
	}
}

//=============================================================================
//	プレイヤーイベント取得処理
//=============================================================================
PLAYEREVENT GetPlayerEvent(void)
{
	return g_player.event;
}

//=============================================================================
//	ログ更新処理
//=============================================================================
void UpdatePosLog(int nIdxPosLog, D3DXVECTOR3 pos)
{
	g_posLog[nIdxPosLog] = pos;
}

//=============================================================================
//	SE管理処理
//=============================================================================
void PlayerSEController(SOUND_LABEL SE)
{


}