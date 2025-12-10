//=============================================================================
//
//	弾の処理 [bullet.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "bullet.h"
#include "input.h"
#include "shadow.h"
#include "camera.h"

#include "meshwall.h"
#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_BULLET			(256)		// 弾の最大数
#define BULLET_SPEED		(7.5f)		// 弾の速さ

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;			// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;	// 頂点バッファへのポインタ
Bullet g_abullet[MAX_BULLET] = {};					// 弾の情報
D3DXMATRIX g_mtxWarldBullet;						// ワールドマトリックス
int g_nIdxShadowBullet = -1;						// 影のインデックス

//=============================================================================
//	弾の初期化処理
//=============================================================================
void InitBullet(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\ball000.png",
		&g_pTextureBullet);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_abullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_abullet[nCntBullet].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_abullet[nCntBullet].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_abullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_abullet[nCntBullet].bUse = false;
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);

	// 初期化
	VERTEX_3D *pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void * *)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-BULLET_WIDTH, BULLET_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(BULLET_WIDTH, BULLET_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-BULLET_WIDTH, -BULLET_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(BULLET_WIDTH, -BULLET_HEIGHT, 0.0f);

	// 法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}

//=============================================================================
//	弾の終了処理
//=============================================================================
void UninitBullet(void)
{
	// テクスチャの破棄
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}
	

	// 頂点バッファの破棄
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//=============================================================================
//	弾の描画処理
//=============================================================================
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス
	D3DXMATRIX mtxView;							// ビューマトリックス

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// 比較方法(基準値より大きければ描画)
	pDevice->SetRenderState(D3DRS_ALPHAREF, 175);					// アルファテストの参照値を設定(～以上で描画, intで設定)

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_mtxWarldBullet);

		// ビューマトリックスの取得
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		// ポリゴンとカメラに対して正面を向ける
		D3DXMatrixInverse(&g_mtxWarldBullet, NULL, &mtxView);	// 逆行列を求める
		g_mtxWarldBullet._41 = 0.0f;
		g_mtxWarldBullet._42 = 0.0f;
		g_mtxWarldBullet._43 = 0.0f;

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_abullet[nCntBullet].pos.x, g_abullet[nCntBullet].pos.y, g_abullet[nCntBullet].pos.z);
		D3DXMatrixMultiply(&g_mtxWarldBullet, &g_mtxWarldBullet, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWarldBullet);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureBullet);

		if (g_abullet[nCntBullet].bUse == true)
		{
			// 弾の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		}

	}

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// アルファテストを無効化
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);	// 比較方法(すべて描画)
	pDevice->SetRenderState(D3DRS_ALPHAREF, 255);				// 基準値を設定(すべてを描画している)
}

//=============================================================================
//	弾の更新処理
//=============================================================================
void UpdateBullet(void)
{
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_abullet[nCntBullet].bUse == false)
		{
			continue;
		}

		g_abullet[nCntBullet].posOld = g_abullet[nCntBullet].pos;

		g_abullet[nCntBullet].pos += g_abullet[nCntBullet].move;

		if (CollisionDot(&g_abullet[nCntBullet].pos, &g_abullet[nCntBullet].posOld, &g_abullet[nCntBullet].move, &g_abullet[nCntBullet].rot) == true)
		{
			g_abullet[nCntBullet].move.x = sinf(g_abullet[nCntBullet].rot.y) * BULLET_SPEED;
		}	g_abullet[nCntBullet].move.z = cosf(g_abullet[nCntBullet].rot.y) * BULLET_SPEED;

		PrintDebugProc("弾の向き = %.2f\n", g_abullet[nCntBullet].rot.y);

		g_abullet[nCntBullet].nLife--;

		if (g_abullet[nCntBullet].nLife < 0)
		{
			g_abullet[nCntBullet].bUse = false;
		}

	}
}

//=============================================================================
//	弾の設定処理
//=============================================================================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_abullet[nCntBullet].bUse == true)
		{
			continue;
		}

		rot.y = rot.y - D3DX_PI;

		rot.y = AngleNormalize(rot.y);

		g_abullet[nCntBullet].pos = pos;
		g_abullet[nCntBullet].rot = rot;

		g_abullet[nCntBullet].nLife = 50;

		g_abullet[nCntBullet].move.x = sinf(rot.y) * BULLET_SPEED;
		g_abullet[nCntBullet].move.z = cosf(rot.y) * BULLET_SPEED;

		g_abullet[nCntBullet].bUse = true;
		break;
	}
}