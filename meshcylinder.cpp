//=============================================================================
//
//	メッシュシリンダー処理 [meshcylinder.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "meshcylinder.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MESHCYLINDER_SPLIT_WIDHT		(32 + 1)	// 横の分割数
#define MESHCYLINDER_SPLIT_HEIGTH		(1 + 1)		// 縦の分割数
#define MESHCYLINDER_ROT				(D3DX_PI / (MESHCYLINDER_SPLIT_WIDHT - 1))

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureMeshCylinder = NULL;		// テクスチャへのポインタ
MeshCylinder g_ameshcylinder[MAX_MESHCYLINDER];			// メッシュシリンダーの情報

//=============================================================================
//	メッシュシリンダーの初期化処理
//=============================================================================
void InitMeshCylinder(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Iceberg000.png",
		&g_pTextureMeshCylinder);

	for (int nCntMeshCylinder = 0; nCntMeshCylinder < MAX_MESHCYLINDER; nCntMeshCylinder++)
	{
		g_ameshcylinder[nCntMeshCylinder].pVtxBuff = NULL;
		g_ameshcylinder[nCntMeshCylinder].pIdxBuff = NULL;
		g_ameshcylinder[nCntMeshCylinder].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_ameshcylinder[nCntMeshCylinder].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_ameshcylinder[nCntMeshCylinder].fRadius = MESHCYLINDER_RADIUS;
		g_ameshcylinder[nCntMeshCylinder].bUse = false;

		// 頂点バッファの生成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MESHCYLINDER_SPLIT_WIDHT * MESHCYLINDER_SPLIT_HEIGTH,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&g_ameshcylinder[nCntMeshCylinder].pVtxBuff,
			NULL);
	}

	// 初期化
	VERTEX_3D *pVtx;			// 頂点情報へのポインタ

	for (int nCntMeshCylinder = 0; nCntMeshCylinder < MAX_MESHCYLINDER; nCntMeshCylinder++)
	{
		// 頂点バッファをロックし,頂点情報へのポインタを取得
		g_ameshcylinder[nCntMeshCylinder].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntHeight = 0; nCntHeight < MESHCYLINDER_SPLIT_HEIGTH; nCntHeight++)
		{
			float fRot = -D3DX_PI;
			D3DXVECTOR3 vecDir;

			for (int nCntVtxWidht = 0; nCntVtxWidht < MESHCYLINDER_SPLIT_WIDHT; nCntVtxWidht++)
			{
				// 頂点座標の設定
				pVtx[nCntVtxWidht].pos = D3DXVECTOR3(sinf(fRot) * g_ameshcylinder[nCntMeshCylinder].fRadius, -MESHCYLINDER_HEIGHT * nCntHeight, cosf(fRot) * g_ameshcylinder[nCntMeshCylinder].fRadius);

				// 法線ベクトルの設定
				vecDir = D3DXVECTOR3(pVtx[nCntVtxWidht].pos.x, 0.0f, pVtx[nCntVtxWidht].pos.z);
				D3DXVec3Normalize(&vecDir, &vecDir);
				pVtx[nCntVtxWidht].nor = vecDir;

				// 頂点カラーの設定
				pVtx[nCntVtxWidht].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// テクスチャ座標の設定
				pVtx[nCntVtxWidht].tex = D3DXVECTOR2(0.25f * nCntVtxWidht, 1.0f * nCntHeight);

				// 角度を加算
				fRot += MESHCYLINDER_ROT * 2;
			}

			// ポインタを進める
			pVtx += MESHCYLINDER_SPLIT_WIDHT;
		}
		
		// 頂点バッファをアンロックする
		g_ameshcylinder[nCntMeshCylinder].pVtxBuff->Unlock();
	}

	for (int nCntMeshCylinder = 0; nCntMeshCylinder < MAX_MESHCYLINDER; nCntMeshCylinder++)
	{
		// インデックスバッファの設定
		pDevice->CreateIndexBuffer(sizeof(WORD) * (MESHCYLINDER_SPLIT_WIDHT * 2 * (MESHCYLINDER_SPLIT_HEIGTH - 1) + ((MESHCYLINDER_SPLIT_HEIGTH - 2) * 2)),
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_MANAGED,
			&g_ameshcylinder[nCntMeshCylinder].pIdxBuff,
			NULL);

		WORD* pIdx;		// インデックス情報へのポインタ

		// インデックスバッファをロックし、頂点情報へのポインタを取得
		g_ameshcylinder[nCntMeshCylinder].pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

		for (int nCntIdxHeigth = 0; nCntIdxHeigth < MESHCYLINDER_SPLIT_HEIGTH - 1; nCntIdxHeigth++)
		{
			for (int nCntIdxWidht = 0; nCntIdxWidht < MESHCYLINDER_SPLIT_WIDHT; nCntIdxWidht++)
			{
				if (nCntIdxHeigth != 0 && nCntIdxWidht == 0)
				{// 高さが最初の行以外かつ幅が最初の行
					pIdx[0] = (MESHCYLINDER_SPLIT_WIDHT * (nCntIdxHeigth + 1)) + nCntIdxWidht;

					pIdx++;
				}

				pIdx[0] = (MESHCYLINDER_SPLIT_WIDHT * (nCntIdxHeigth + 1)) + nCntIdxWidht;
				pIdx[1] = MESHCYLINDER_SPLIT_WIDHT * nCntIdxHeigth + nCntIdxWidht;

				pIdx += 2;

				if (nCntIdxHeigth != MESHCYLINDER_SPLIT_HEIGTH - 2 && nCntIdxWidht == MESHCYLINDER_SPLIT_WIDHT - 1)
				{// 高さが最後の行以外かつ幅が最後の行
					pIdx[0] = MESHCYLINDER_SPLIT_WIDHT * nCntIdxHeigth + nCntIdxWidht;

					pIdx++;
				}
			}
		}

		// インデックスバッファをアンロックする
		g_ameshcylinder[nCntMeshCylinder].pIdxBuff->Unlock();
	}

	SetMeshCylinder(D3DXVECTOR3(0.0f, 450.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

//=============================================================================
//	メッシュシリンダーの終了処理
//=============================================================================
void UninitMeshCylinder(void)
{
	// テクスチャの破棄
	if (g_pTextureMeshCylinder != NULL)
	{
		g_pTextureMeshCylinder->Release();
		g_pTextureMeshCylinder = NULL;
	}
	
	for (int nCntMeshCylinder = 0; nCntMeshCylinder < MAX_MESHCYLINDER; nCntMeshCylinder++)
	{
		// 頂点バッファの破棄
		if (g_ameshcylinder[nCntMeshCylinder].pVtxBuff != NULL)
		{
			g_ameshcylinder[nCntMeshCylinder].pVtxBuff->Release();
			g_ameshcylinder[nCntMeshCylinder].pVtxBuff = NULL;
		}

		// インデックスバッファの破棄
		if (g_ameshcylinder[nCntMeshCylinder].pIdxBuff != NULL)
		{
			g_ameshcylinder[nCntMeshCylinder].pIdxBuff->Release();
			g_ameshcylinder[nCntMeshCylinder].pIdxBuff = NULL;
		}
	}
}

//=============================================================================
//	メッシュシリンダーの描画処理
//=============================================================================
void DrawMeshCylinder(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// 比較方法(基準値より大きければ描画)
	pDevice->SetRenderState(D3DRS_ALPHAREF, 150);				// アルファテストの参照値を設定(～以上で描画, intで設定)

	for (int nCntMeshCylinder = 0; nCntMeshCylinder < MAX_MESHCYLINDER; nCntMeshCylinder++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_ameshcylinder[nCntMeshCylinder].mtxWorld);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_ameshcylinder[nCntMeshCylinder].rot.y, g_ameshcylinder[nCntMeshCylinder].rot.x, g_ameshcylinder[nCntMeshCylinder].rot.z);
		D3DXMatrixMultiply(&g_ameshcylinder[nCntMeshCylinder].mtxWorld, &g_ameshcylinder[nCntMeshCylinder].mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_ameshcylinder[nCntMeshCylinder].pos.x, g_ameshcylinder[nCntMeshCylinder].pos.y, g_ameshcylinder[nCntMeshCylinder].pos.z);
		D3DXMatrixMultiply(&g_ameshcylinder[nCntMeshCylinder].mtxWorld, &g_ameshcylinder[nCntMeshCylinder].mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_ameshcylinder[nCntMeshCylinder].mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_ameshcylinder[nCntMeshCylinder].pVtxBuff, 0, sizeof(VERTEX_3D));

		// インデックスバッファをデータストリームに設定
		pDevice->SetIndices(g_ameshcylinder[nCntMeshCylinder].pIdxBuff);

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureMeshCylinder);

		if (g_ameshcylinder[nCntMeshCylinder].bUse == true)
		{
			// メッシュシリンダーの描画
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
				0,
				0,
				MESHCYLINDER_SPLIT_WIDHT * MESHCYLINDER_SPLIT_HEIGTH,
				0,
				((MESHCYLINDER_SPLIT_WIDHT - 1) * (MESHCYLINDER_SPLIT_HEIGTH - 1) * 2) + ((MESHCYLINDER_SPLIT_HEIGTH - 2) * 4));
		}
	}

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// アルファテストを無効化
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);	// 比較方法(すべて描画)
	pDevice->SetRenderState(D3DRS_ALPHAREF, 255);				// 基準値を設定(すべてを描画している)
}

//=============================================================================
//	メッシュシリンダーの更新処理
//=============================================================================
void UpdateMeshCylinder(void)
{

}

//=============================================================================
//	メッシュシリンダーの設定処理
//=============================================================================
void SetMeshCylinder(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	for (int nCntCylinder = 0; nCntCylinder < MAX_MESHCYLINDER; nCntCylinder++)
	{
		if (g_ameshcylinder[nCntCylinder].bUse == false)
		{
			// メッシュシリンダーの設定
			g_ameshcylinder[nCntCylinder].pos = pos;
			g_ameshcylinder[nCntCylinder].rot = rot;
			g_ameshcylinder[nCntCylinder].bUse = true;
			break;
		}
	}
}