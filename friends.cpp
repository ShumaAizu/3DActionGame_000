//=============================================================================
//
//	仲間たちの処理 [friends.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "friends.h"
#include "bullet.h"
#include "effect.h"
#include "particle.h"
#include "input.h"
#include "shadow.h"
#include "camera.h"
#include "model.h"
#include "meshwall.h"
#include "meshfield.h"

#include "snow.h"

#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_FRIENDSTEX		(16)		// 読み込むテクスチャの最大数
#define GRAVITY				(-0.25f)	// 重力
#define FRIENDS_JUMP		(5.0f)		// ジャンプ力

//*****************************************************************************
// グローバル変数
//*****************************************************************************
Friends g_afriends[MAX_FRIENDS];						// 仲間の情報
FriendsInfo g_afriendsInfo[FRIENDSTYPE_MAX];			// 仲間の各情報保持用
int g_nNumFriends;										// 現在の仲間の使用数
int g_nNumFriendsInfo;									// 現在の情報保持数

//=============================================================================
//	仲間の初期化処理
//=============================================================================
void InitFriends(void)
{
	PFRIENDS pFriends = &g_afriends[0];
	PFRIENDSINFO pFriendsInfo = &g_afriendsInfo[0];

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	memset(pFriends, NULL, sizeof(Friends) * MAX_FRIENDS);
	memset(pFriendsInfo, NULL, sizeof(FriendsInfo) * FRIENDSTYPE_MAX);

	for (int nCntFriends = 0; nCntFriends < MAX_FRIENDS; nCntFriends++, pFriends++)
	{
		// 初期化
		//pFriends->move = D3DXVECTOR3(1.0f, 0.0f, 1.0f);
		pFriends->nIdxShadow = -1;
		pFriends->fSpeed = FRIENDS_SPEED;
		pFriends->nIdxPosLog = -1;
	}

	g_nNumFriendsInfo = 0;
}

//=============================================================================
//	仲間の終了処理
//=============================================================================
void UninitFriends(void)
{
	PFRIENDS pFriends = &g_afriends[0];

	for (int nCntOffSetModel = 0; nCntOffSetModel < MAX_OFFSETMODEL; nCntOffSetModel++, pFriends++)
	{
		// メッシュの破棄
		if (pFriends->aOffSetModel[nCntOffSetModel].pMesh != NULL)
		{
			pFriends->aOffSetModel[nCntOffSetModel].pMesh->Release();
			pFriends->aOffSetModel[nCntOffSetModel].pMesh = NULL;
		}

		// マテリアルの破棄
		if (pFriends->aOffSetModel[nCntOffSetModel].pBuffMat != NULL)
		{
			pFriends->aOffSetModel[nCntOffSetModel].pBuffMat->Release();
			pFriends->aOffSetModel[nCntOffSetModel].pBuffMat = NULL;
		}
	}
}

//=============================================================================
//	仲間の描画処理
//=============================================================================
void DrawFriends(void)
{
	PFRIENDS pFriends = &g_afriends[0];

	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用
	D3DXMATERIAL* pMat;							// マテリアルデータへのポインタ

	for (int nCntFriends = 0; nCntFriends < MAX_FRIENDS; nCntFriends++, pFriends++)
	{
		if (pFriends->bUse == false)
		{
			continue;
		}

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pFriends->mtxWorld);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pFriends->rot.y, pFriends->rot.x, pFriends->rot.z);
		D3DXMatrixMultiply(&pFriends->mtxWorld, &pFriends->mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, pFriends->pos.x, pFriends->pos.y, pFriends->pos.z);
		D3DXMatrixMultiply(&pFriends->mtxWorld, &pFriends->mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &pFriends->mtxWorld);

		// 現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		// 全モデル(パーツ)の描画
		for (int nCntOffSetModel = 0; nCntOffSetModel < pFriends->nNumOffSetModel; nCntOffSetModel++)
		{
			D3DXMATRIX mtxRotOffSetModel, mtxTransOffSetModel;	// 計算用マトリックス
			D3DXMATRIX mtxParent;								// 親のマトリックス

			// パーツのワールドマトリックスを初期化
			D3DXMatrixIdentity(&pFriends->aOffSetModel[nCntOffSetModel].mtxWorld);

			// パーツの向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRotOffSetModel, pFriends->aOffSetModel[nCntOffSetModel].rot.y, pFriends->aOffSetModel[nCntOffSetModel].rot.x, pFriends->aOffSetModel[nCntOffSetModel].rot.z);
			D3DXMatrixMultiply(&pFriends->aOffSetModel[nCntOffSetModel].mtxWorld, &pFriends->aOffSetModel[nCntOffSetModel].mtxWorld, &mtxRotOffSetModel);

			// パーツの位置を反映(オフセット)
			D3DXMatrixTranslation(&mtxTransOffSetModel, pFriends->aOffSetModel[nCntOffSetModel].pos.x, pFriends->aOffSetModel[nCntOffSetModel].pos.y, pFriends->aOffSetModel[nCntOffSetModel].pos.z);
			D3DXMatrixMultiply(&pFriends->aOffSetModel[nCntOffSetModel].mtxWorld, &pFriends->aOffSetModel[nCntOffSetModel].mtxWorld, &mtxTransOffSetModel);

			// パーツの「親のマトリックス」を設定
			if (pFriends->aOffSetModel[nCntOffSetModel].nIdxModelParent != -1)
			{// 親モデルがある場合
				mtxParent = pFriends->aOffSetModel[pFriends->aOffSetModel[nCntOffSetModel].nIdxModelParent].mtxWorld;
			}
			else
			{// 親モデルがない場合
				mtxParent = pFriends->mtxWorld;
			}

			// 算出した「パーツのワールドマトリックス」と「親のマトリックス」を掛け合わせる
			D3DXMatrixMultiply(&pFriends->aOffSetModel[nCntOffSetModel].mtxWorld,
				&pFriends->aOffSetModel[nCntOffSetModel].mtxWorld,
				&mtxParent);

			// パーツのワールドマトリックスを設定
			pDevice->SetTransform(D3DTS_WORLD, &pFriends->aOffSetModel[nCntOffSetModel].mtxWorld);

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)pFriends->aOffSetModel[nCntOffSetModel].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)pFriends->aOffSetModel[nCntOffSetModel].dwNumMat; nCntMat++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// テクスチャの設定
				pDevice->SetTexture(0, pFriends->aOffSetModel[nCntOffSetModel].apTexture[nCntMat]);

				// 仲間(パーツ)の描画
				pFriends->aOffSetModel[nCntOffSetModel].pMesh->DrawSubset(nCntMat);
			}
		}

		// 保存していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}
}

//=============================================================================
//	仲間の更新処理
//=============================================================================
void UpdateFriends(void)
{
	PFRIENDS pFriends = &g_afriends[0];		// ポインタ取得

	for (int nCntFrinds = 0; nCntFrinds < MAX_FRIENDS; nCntFrinds++, pFriends++)
	{
		if (pFriends->bUse == false)
		{// 使用していなければ弾く
			continue;
		}

		pFriends->posOld = pFriends->pos;		// 過去の位置更新

		// 状態確認
		switch (pFriends->state)
		{
			// フレンズ状態なら
		case FRIENDSSTATE_FRIENDS:
			pFriends->vecDest = *pFriends->posDest - pFriends->pos;	// 目的向きを計算
			pFriends->vecDest.y = 0.0f;								// y方向は無視
			pFriends->move = pFriends->vecDest * pFriends->fSpeed;	// 目的向きに速度をかける

			pFriends->rotDest.y = atan2f(pFriends->move.x, pFriends->move.z) - D3DX_PI;	// 目的向きを向かせる

			// 補正
			float fRotDiff = 0.0f;

			pFriends->rot.y = AngleNormalize(pFriends->rot.y);

			pFriends->rotDest.y = AngleNormalize(pFriends->rotDest.y);

			fRotDiff = pFriends->rotDest.y - pFriends->rot.y;		// 差分

			fRotDiff = AngleNormalize(fRotDiff);

			pFriends->rot.y += fRotDiff * 0.175f;

			fRotDiff = AngleNormalize(fRotDiff);

			pFriends->rot.y = AngleNormalize(pFriends->rot.y);

			pFriends->nPosLogCounter++;		// 記録までのカウンターを加算

			if (pFriends->nPosLogCounter == POSLOGCOUNT)
			{// 一定の値になったら記録する
				UpdatePosLog(pFriends->nIdxPosLog, pFriends->pos);
				pFriends->nPosLogCounter = 0;
			}

			break;
		}

		//pFriends->move.y += GRAVITY;			// 重力をかける
		pFriends->pos += pFriends->move;		// 位置を更新

		// 慣性を更新
		pFriends->move.x += (0.0f - pFriends->move.x) * pFriends->fInertia;
		pFriends->move.z += (0.0f - pFriends->move.z) * pFriends->fInertia;


		if (pFriends->pos.y < 0.0f)
		{
			pFriends->move.y = GRAVITY;
			pFriends->bJump = false;
		}

		// 壁との当たり判定
		CollisionMeshWall(&pFriends->pos, &pFriends->posOld, &pFriends->move);

		// フィールドとの当たり判定
		pFriends->pRideField = CollisionMeshField(&pFriends->pos, &pFriends->posOld, &pFriends->move);

		if (pFriends->pRideField != NULL)
		{// もしフィールド上にいたら
			//　慣性を設定
			switch (pFriends->pRideField->type)
			{
			case MESHFIELDTYPE_ICE:
				pFriends->fInertia = ICE_INERTIA;
				break;

			case MESHFIELDTYPE_SNOW:
				pFriends->fInertia = SNOW_INERTIA;
				break;

			case MESHFIELDTYPE_SEA:
				pFriends->pos = D3DXVECTOR3(0.0f, 0.0f, -1500.0f);
				break;
			}

			if (pFriends->bJump == true)
			{
				SetMotionFriends(MOTIONTYPE_LANDING, false, true, 10, pFriends);
				pFriends->state = FRIENDSSTATE_NUETRAL;
			}
		}

		// モデルとの当たり判定
		CollisionModel(&pFriends->pos, &pFriends->posOld, &pFriends->move, D3DXVECTOR3(10.0f, 10.0f, 10.0f), D3DXVECTOR3(10.0f, 10.0f, 10.0f));

		// 影の位置を更新
		SetPositionShadow(pFriends->nIdxShadow, D3DXVECTOR3(pFriends->pos.x, pFriends->pos.y - pFriends->pos.y, pFriends->pos.z));

		// 全モデル更新
		UpdateMotionFriends(pFriends);
	}
}

//=============================================================================
//	仲間の取得処理
//=============================================================================
PFRIENDS GetFriends(void)
{
	return &g_afriends[0];
}

//=============================================================================
//	モーションの設定処理
//=============================================================================
void SetMotionFriends(MOTIONTYPE motiontype, bool bLoopMotion, bool bBlendMotion, int nFrameBlend, Friends* pFriends)
{
	if (bBlendMotion == true)
	{
		pFriends->bBlendMotion = true;
		pFriends->nFrameBlend = nFrameBlend;
		pFriends->nCounterBlend = 0;

		pFriends->motiontypeBlend = motiontype;
		pFriends->bLoopMotionBlend = bLoopMotion;
		pFriends->nNumKeyBlend = pFriends->aMotionInfo[motiontype].nNumKey;
		pFriends->nKeyBlend = 0;
		pFriends->nCounterMotionBlend = 0;
	}
	else
	{
		pFriends->nNumKey = pFriends->aMotionInfo[motiontype].nNumKey;
		pFriends->motiontype = motiontype;
		pFriends->bLoopMotion = bLoopMotion;
		pFriends->nKey = 0;
		pFriends->nCounterMotion = 0;

		OffSetModel* pModel = &pFriends->aOffSetModel[0];
		KEY_INFO* pKeyInfo = &pFriends->aMotionInfo[pFriends->motiontype].aKeyInfo[0];

		for (int nCntModel = 0; nCntModel < pFriends->nNumOffSetModel; nCntModel++, pModel++, pKeyInfo++)
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
void UpdateMotionFriends(Friends *pFriends)
{
	float fPosX, fPosY, fPosZ, fRotX, fRotY, fRotZ;
	float fDiffKey;

	// 全モデル(パーツ)の更新
	for (int nCntModel = 0; nCntModel < pFriends->nNumOffSetModel; nCntModel++)
	{
		KEY DiffKey = {};
		KEY RateKey = {};

		// 現在のキー
		KEY* pKey = &pFriends->aMotionInfo[pFriends->motiontype].aKeyInfo[pFriends->nKey].aKey[nCntModel];

		// 次のキー
		KEY *pKeyNext = &pFriends->aMotionInfo[pFriends->motiontype].aKeyInfo[(pFriends->nKey + 1) % pFriends->nNumKey].aKey[nCntModel];

		// 対象モデル
		OffSetModel* pModel = &pFriends->aOffSetModel[nCntModel];

		// 最大フレームと現在のフレームの比率
		float fRateKey = (float)pFriends->nCounterMotion / (float)pFriends->aMotionInfo[pFriends->motiontype].aKeyInfo[pFriends->nKey].nFrame;

		if (pFriends->bBlendMotion == true)
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
			KEY* pKeyBlend = &pFriends->aMotionInfo[pFriends->motiontypeBlend].aKeyInfo[pFriends->nKeyBlend].aKey[nCntModel];

			// ブレンドモーションの次のキー
			KEY *pKeyNextBlend = &pFriends->aMotionInfo[pFriends->motiontypeBlend].aKeyInfo[(pFriends->nKeyBlend + 1) % pFriends->nNumKeyBlend].aKey[nCntModel];

			// ブレンドモーションの最大フレームと現在のフレームの比率
			float fRateKeyBlend = (float)pFriends->nCounterMotionBlend / (float)pFriends->aMotionInfo[pFriends->motiontypeBlend].aKeyInfo[pFriends->nKeyBlend].nFrame;

			// モーションブレンドの最大フレームと現在のフレームの比率
			float fRateBlend = (float)pFriends->nCounterBlend / (float)pFriends->nFrameBlend;

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

	if (pFriends->bBlendMotion == true)
	{// ブレンドあり
		// ブレンドモーションのカウンターを更新
		pFriends->nCounterMotionBlend++;

		if (pFriends->nCounterMotionBlend >= pFriends->aMotionInfo[pFriends->motiontypeBlend].aKeyInfo[pFriends->nKeyBlend].nFrame)
		{
			// キーを進める
			pFriends->nCounterMotionBlend = 0;	// カウンターリセット
			if ((pFriends->nKeyBlend = (pFriends->nKeyBlend + 1) % pFriends->nNumKeyBlend) == 0 && pFriends->bLoopMotionBlend == false)
			{// キーが終着点まで来たかつループ状態じゃなければ
				SetMotionFriends(MOTIONTYPE_NEUTRAL, true, true, 10, pFriends);
			}
		}

		// ブレンドカウンターを更新
		pFriends->nCounterBlend++;
		if (pFriends->nCounterBlend >= pFriends->nFrameBlend)
		{// カウンターが回り切ったら
			pFriends->nCounterBlend = 0;									// カウンターリセット
			pFriends->motiontype = pFriends->motiontypeBlend;				// 次のモーションに切り替える
			pFriends->nCounterMotion = pFriends->nCounterMotionBlend;		// モーションカウンターをブレンドと同期
			pFriends->nKey = pFriends->nKeyBlend;							// キー番号をブレンドと同期
			pFriends->bLoopMotion = pFriends->bLoopMotionBlend;				// ループ状態をブレンドと同期
			pFriends->bBlendMotion = false;									// ブレンド状態を終了
		}
	}
	else
	{// ブレンドなし
		pFriends->nCounterMotion++;

		if (pFriends->nCounterMotion >= pFriends->aMotionInfo[pFriends->motiontype].aKeyInfo[pFriends->nKey].nFrame)
		{
			// キーを進める
			pFriends->nCounterMotion = 0;	// カウンターリセット
			if ((pFriends->nKey = (pFriends->nKey + 1) % pFriends->nNumKey) == 0 && pFriends->bLoopMotion == false)
			{// キーが終着点まで来たかつループ状態じゃなければ
				SetMotionFriends(MOTIONTYPE_NEUTRAL, true, true, 10, pFriends);
			}
		}
	}
}

//=============================================================================
//	仲間の設定処理
//=============================================================================
void SetFriends(D3DXVECTOR3 pos, D3DXVECTOR3 rot, FRIENDSTYPE type, FRIENDEVENT event)
{
	PFRIENDS pFriends = &g_afriends[0];
	PFRIENDSINFO pFriendsInfo = &g_afriendsInfo[type];

	for (int nCntFriends = 0; nCntFriends < MAX_FRIENDS; nCntFriends++, pFriends++)
	{
		if (pFriends->bUse == true)
		{
			continue;
		}

		// 各変数を設定
		pFriends->pos = pos;
		pFriends->rot = rot;
		pFriends->type = type;
		pFriends->event = event;
		pFriends->nIdxShadow = SetShadow();
		pFriends->bUse = true;

		// 種別で半径を設定
		switch (type)
		{
		case FRIENDSTYPE_000:
			pFriends->fRadius = PENGUIN_RADIUS;
			break;

		case FRIENDSTYPE_001:
			pFriends->fRadius = SNOWMAN_RADIUS;
			break;
		}

		// モーション, パーツ分設定
		pFriends->nNumMotion = pFriendsInfo->nNumMotion;
		pFriends->nNumOffSetModel = pFriendsInfo->nNumOffSetModel;

		for (int nCntMotion = 0; nCntMotion < pFriendsInfo->nNumMotion; nCntMotion++)
		{
			pFriends->aMotionInfo[nCntMotion] = pFriendsInfo->aMotionInfo[nCntMotion];
		}

		for (int nCntOffSetModel = 0; nCntOffSetModel < pFriendsInfo->nNumOffSetModel; nCntOffSetModel++)
		{
			pFriends->aOffSetModel[nCntOffSetModel] = pFriendsInfo->aOffSetModel[nCntOffSetModel];
		}

		SetMotionFriends(MOTIONTYPE_NEUTRAL, true, false, 0, pFriends);
		break;
	}

}

//=============================================================================
//	仲間のパーツ設定処理
//=============================================================================
void LoadPartsFriends(const char* pPartsFile)
{
	PFRIENDSINFO pFriendsInfo = &g_afriendsInfo[g_nNumFriendsInfo];

	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	D3DXMATERIAL* pMat;

	OffSetModel *pOffSetModel = &pFriendsInfo->aOffSetModel[pFriendsInfo->nNumOffSetModel];

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

	pFriendsInfo->nNumOffSetModel++;
}

//=============================================================================
//	仲間のオフセット読み込み処理
//=============================================================================
void LoadCharacterFriends(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nIdxModel, int nIdxModelParent)
{
	PFRIENDSINFO pFriendsInfo = &g_afriendsInfo[g_nNumFriendsInfo];

	OffSetModel* pOffSetModel = &pFriendsInfo->aOffSetModel[nIdxModel];

	pOffSetModel->nIdxModel = nIdxModel;
	pOffSetModel->nIdxModelParent = nIdxModelParent;
	pOffSetModel->pos = pOffSetModel->OffPos = pos;
	pOffSetModel->rot = pOffSetModel->OffRot = rot;
}

//=============================================================================
//	仲間のモーション読み込み処理
//=============================================================================
void LoadMotionFriends(bool bLoop, int nNumKey, KEY_INFO* pKeyInfo, int nMotion)
{
	PFRIENDSINFO pFriendsInfo = &g_afriendsInfo[g_nNumFriendsInfo];

	MOTION_INFO* pMotionInfo = &pFriendsInfo->aMotionInfo[nMotion];

	pMotionInfo->bLoop = bLoop;
	pMotionInfo->nNumKey = nNumKey;

	for (int nCntKeyInfo = 0; nCntKeyInfo < nNumKey; nCntKeyInfo++, pKeyInfo++)
	{
		pMotionInfo->aKeyInfo[nCntKeyInfo] = *pKeyInfo;
	}

	pFriendsInfo->nNumMotion++;
}

//=============================================================================
//	仲間の状態管理処理
//=============================================================================
void FriendsStateController(PFRIENDS pFriends, FRIENDSSTATE state)
{

}

//=============================================================================
//	仲間の種類増加処理
//=============================================================================
void NumFriendsAdd(void)
{
	g_nNumFriendsInfo++;
}

//=============================================================================
//	仲間の当たり判定処理
//=============================================================================
bool CollisionFriends(D3DXVECTOR3* pPos, float fRadius)
{
	PFRIENDS pFriends = &g_afriends[0];

	float fDiff = 0.0f;
	D3DXVECTOR3 vecToFriends = {};

	for (int nCntFriends = 0; nCntFriends < MAX_FRIENDS; nCntFriends++, pFriends++)
	{
		if (pFriends->bUse == false || pFriends->state == FRIENDSSTATE_FRIENDS)
		{
			continue;
		}

		fDiff = powf(pFriends->pos.x - pPos->x, 2) + powf(pFriends->pos.y - pPos->y, 2) + powf(pFriends->pos.z - pPos->z, 2);

		if (fDiff <= powf(fRadius + pFriends->fRadius, 2.0f))
		{
			if (GetKeyboardTrigger(DIK_9) == true)
			{
				if (pFriends->state == FRIENDSSTATE_EVENT)
				{

				}
				else
				{
					pFriends->nIdxPosLog = FriendsAdd(&pFriends->posDest);

					pFriends->state = FRIENDSSTATE_FRIENDS;

					SetMotionFriends(MOTIONTYPE_ACTION, false, true, 10, pFriends);

					return true;
				}
			}
		}
	}

	return false;
}

//=============================================================================
//	仲間のイベント処理
//=============================================================================