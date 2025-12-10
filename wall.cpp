//=============================================================================
//
//	壁の処理 [wall.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "wall.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureWall = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL;	// 頂点バッファへのポインタ
Wall g_awall[MAX_WALL] = {};					// 壁の情報

//=============================================================================
//	壁の初期化処理
//=============================================================================
void InitWall(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\wall000.jpg",
		&g_pTextureWall);

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		g_awall[nCntWall].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_awall[nCntWall].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_awall[nCntWall].bUse = false;
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_WALL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffWall,
		NULL);

	// 初期化
	VERTEX_3D *pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffWall->Lock(0, 0, (void * *)&pVtx, 0);

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(WALL_WIDTH, WALL_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(-WALL_WIDTH, WALL_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(WALL_WIDTH, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(-WALL_WIDTH, 0.0f, 0.0f);

		// 法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

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

		pVtx += 4;
	}

	//SetWall(D3DXVECTOR3(0.0f, 0.0f, 100.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
	//SetWall(D3DXVECTOR3(-100.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
	//SetWall(D3DXVECTOR3(100.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
	//SetWall(D3DXVECTOR3(0.0f, 0.0f, -100.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// 頂点バッファをアンロックする
	g_pVtxBuffWall->Unlock();
}

//=============================================================================
//	壁の終了処理
//=============================================================================
void UninitWall(void)
{
	// テクスチャの破棄
	if (g_pTextureWall != NULL)
	{
		g_pTextureWall->Release();
		g_pTextureWall = NULL;
	}
	

	// 頂点バッファの破棄
	if (g_pVtxBuffWall != NULL)
	{
		g_pVtxBuffWall->Release();
		g_pVtxBuffWall = NULL;
	}
}

//=============================================================================
//	壁の描画処理
//=============================================================================
void DrawWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_awall[nCntWall].mtxWorld);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_awall[nCntWall].rot.y, g_awall[nCntWall].rot.x, g_awall[nCntWall].rot.z);
		D3DXMatrixMultiply(&g_awall[nCntWall].mtxWorld, &g_awall[nCntWall].mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_awall[nCntWall].pos.x, g_awall[nCntWall].pos.y, g_awall[nCntWall].pos.z);
		D3DXMatrixMultiply(&g_awall[nCntWall].mtxWorld, &g_awall[nCntWall].mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_awall[nCntWall].mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffWall, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureWall);

		if (g_awall[nCntWall].bUse == true)
		{
			// 壁の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWall * 4, 2);
		}
	}
}

//=============================================================================
//	壁の更新処理
//=============================================================================
void UpdateWall(void)
{

}

//=============================================================================
//	壁の設定処理
//=============================================================================
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if (g_awall[nCntWall].bUse == false)
		{
			// 壁の設定
			g_awall[nCntWall].pos = pos;
			g_awall[nCntWall].rot = rot;
			g_awall[nCntWall].bUse = true;
			break;
		}
	}
}

//=============================================================================
//	壁の処理
//=============================================================================
void SetPositionWall(int IdxWall, D3DXVECTOR3 pos)
{
	g_awall[IdxWall].pos = pos;
}