//=============================================================================
//
//	メッシュスフィア処理 [meshsphere.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "meshsphere.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MESHSPHERE_SPLIT_VERTICAL		(16 + 1)											// 縦の分割数
#define MESHSPHERE_SPLIT_HORIZONTAL		(16 + 1)											// 横の分割数
#define MESHSPHERE_ANGLE_VERTICAL		(D3DX_PI / (MESHSPHERE_SPLIT_VERTICAL - 1))			// 縦の分割数に応じた角度
#define MESHSPHERE_ANGLE_HORIZONTAL		(D3DX_PI / (MESHSPHERE_SPLIT_HORIZONTAL - 1))		// 横の分割数に応じた角度

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureMeshSphere = NULL;		// テクスチャへのポインタ
MeshSphere g_ameshsphere[MAX_MESHSPHERE];					// メッシュスフィアの情報

//=============================================================================
//	メッシュスフィアの初期化処理
//=============================================================================
void InitMeshSphere(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\ski002.jpg",
		&g_pTextureMeshSphere);

	for (int nCntMeshSphere = 0; nCntMeshSphere < MAX_MESHSPHERE; nCntMeshSphere++)
	{
		g_ameshsphere[nCntMeshSphere].pVtxBuff = NULL;
		g_ameshsphere[nCntMeshSphere].pIdxBuff = NULL;
		g_ameshsphere[nCntMeshSphere].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_ameshsphere[nCntMeshSphere].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_ameshsphere[nCntMeshSphere].fRadius = 300.0f;
		g_ameshsphere[nCntMeshSphere].bUse = false;

		// 頂点バッファの生成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (MESHSPHERE_SPLIT_VERTICAL - 1) * MESHSPHERE_SPLIT_HORIZONTAL,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&g_ameshsphere[nCntMeshSphere].pVtxBuff,
			NULL);
	}

	// 初期化
	VERTEX_3D *pVtx;			// 頂点情報へのポインタ

	for (int nCntMeshSphere = 0; nCntMeshSphere < MAX_MESHSPHERE; nCntMeshSphere++)
	{
		//float fAngleVertical = D3DX_PI - MESHSPHERE_ANGLE_VERTICAL;
		float fAngleVertical = (D3DX_PI * 0.5f) - MESHSPHERE_ANGLE_VERTICAL;

		// 頂点バッファをロックし,頂点情報へのポインタを取得
		g_ameshsphere[nCntMeshSphere].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntVtxVertical = 0; nCntVtxVertical < MESHSPHERE_SPLIT_VERTICAL - 1; nCntVtxVertical++)
		{
			D3DXVECTOR3 diff;
			D3DXVECTOR3 vecDir;
			float fRadius;
			float fAngleHorizontal = D3DX_PI;

			diff = D3DXVECTOR3(sinf(0.0f) * g_ameshsphere[nCntMeshSphere].fRadius, cosf(fAngleVertical) * g_ameshsphere[nCntMeshSphere].fRadius, 0.0f);

			fRadius = SQRTF(diff.x, diff.y);

			for (int nCntVtxHorizontal = 0; nCntVtxHorizontal < MESHSPHERE_SPLIT_HORIZONTAL; nCntVtxHorizontal++)
			{
				// 頂点座標の設定
				pVtx[nCntVtxHorizontal].pos = D3DXVECTOR3(sinf(fAngleHorizontal) * fRadius, sinf(fAngleVertical) * g_ameshsphere[nCntMeshSphere].fRadius, cosf(fAngleHorizontal) * fRadius);

				// 法線ベクトルの設定
				vecDir = D3DXVECTOR3(pVtx[nCntVtxHorizontal].pos.x, 0.0f, pVtx[nCntVtxHorizontal].pos.z);
				D3DXVec3Normalize(&vecDir, &vecDir);
				pVtx[nCntVtxHorizontal].nor = vecDir;

				// 頂点カラーの設定
				pVtx[nCntVtxHorizontal].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// テクスチャ座標の設定
				pVtx[nCntVtxHorizontal].tex = D3DXVECTOR2(1.0f * nCntVtxHorizontal, 1.0f * nCntVtxVertical);

				// 角度を加算
				fAngleHorizontal += -MESHSPHERE_ANGLE_HORIZONTAL * 2.0f;
			}

			// 角度を減算
			fAngleVertical += -MESHSPHERE_ANGLE_VERTICAL;

			// ポインタを進める
			pVtx += MESHSPHERE_SPLIT_HORIZONTAL;
		}

		// 頂点バッファをアンロックする
		g_ameshsphere[nCntMeshSphere].pVtxBuff->Unlock();
	}

	for (int nCntMeshSphere = 0; nCntMeshSphere < MAX_MESHSPHERE; nCntMeshSphere++)
	{
		// インデックスバッファの設定
		pDevice->CreateIndexBuffer(sizeof(WORD) * ((MESHSPHERE_SPLIT_HORIZONTAL) * 2 * (MESHSPHERE_SPLIT_VERTICAL - 1) + ((MESHSPHERE_SPLIT_VERTICAL - 2) * 2)),
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_MANAGED,
			&g_ameshsphere[nCntMeshSphere].pIdxBuff,
			NULL);

		WORD* pIdx;		// インデックス情報へのポインタ

		int nCntIdx = 0;

		// インデックスバッファをロックし、頂点情報へのポインタを取得
		g_ameshsphere[nCntMeshSphere].pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

		for (int nCntIdxVertical = 0; nCntIdxVertical < MESHSPHERE_SPLIT_VERTICAL - 3; nCntIdxVertical++)
		{
			for (int nCntIdxHorizontal = 0; nCntIdxHorizontal < MESHSPHERE_SPLIT_HORIZONTAL; nCntIdxHorizontal++)
			{
				if (nCntIdxVertical != 0 && nCntIdxHorizontal == 0)
				{// 高さが最初の行以外かつ幅が最初の行
					pIdx[0] = (MESHSPHERE_SPLIT_HORIZONTAL * (nCntIdxVertical + 1)) + nCntIdxHorizontal;

					pIdx++;
				}

				pIdx[0] = (MESHSPHERE_SPLIT_HORIZONTAL * (nCntIdxVertical + 1)) + nCntIdxHorizontal;
				pIdx[1] = MESHSPHERE_SPLIT_HORIZONTAL * nCntIdxVertical + nCntIdxHorizontal;

				pIdx += 2;

				if (nCntIdxVertical != MESHSPHERE_SPLIT_HORIZONTAL - 2 && nCntIdxHorizontal == MESHSPHERE_SPLIT_HORIZONTAL - 1)
				{// 高さが最後の行以外かつ幅が最後の行
					pIdx[0] = MESHSPHERE_SPLIT_HORIZONTAL * nCntIdxVertical + nCntIdxHorizontal;

					pIdx++;
				}
			}
		}

		// インデックスバッファをアンロックする
		g_ameshsphere[nCntMeshSphere].pIdxBuff->Unlock();
	}

	//SetMeshSphere(D3DXVECTOR3(0.0f, 300.0f, -1000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

//=============================================================================
//	メッシュスフィアの終了処理
//=============================================================================
void UninitMeshSphere(void)
{
	// テクスチャの破棄
	if (g_pTextureMeshSphere != NULL)
	{
		g_pTextureMeshSphere->Release();
		g_pTextureMeshSphere = NULL;
	}
	
	for (int nCntMeshSphere = 0; nCntMeshSphere < MAX_MESHSPHERE; nCntMeshSphere++)
	{
		// 頂点バッファの破棄
		if (g_ameshsphere[nCntMeshSphere].pVtxBuff != NULL)
		{
			g_ameshsphere[nCntMeshSphere].pVtxBuff->Release();
			g_ameshsphere[nCntMeshSphere].pVtxBuff = NULL;
		}

		// インデックスバッファの破棄
		if (g_ameshsphere[nCntMeshSphere].pIdxBuff != NULL)
		{
			g_ameshsphere[nCntMeshSphere].pIdxBuff->Release();
			g_ameshsphere[nCntMeshSphere].pIdxBuff = NULL;
		}
	}
}

//=============================================================================
//	メッシュスフィアの描画処理
//=============================================================================
void DrawMeshSphere(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// 比較方法(基準値より大きければ描画)
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);					// アルファテストの参照値を設定(～以上で描画, intで設定)

	for (int nCntMeshSphere = 0; nCntMeshSphere < MAX_MESHSPHERE; nCntMeshSphere++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_ameshsphere[nCntMeshSphere].mtxWorld);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_ameshsphere[nCntMeshSphere].rot.y, g_ameshsphere[nCntMeshSphere].rot.x, g_ameshsphere[nCntMeshSphere].rot.z);
		D3DXMatrixMultiply(&g_ameshsphere[nCntMeshSphere].mtxWorld, &g_ameshsphere[nCntMeshSphere].mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_ameshsphere[nCntMeshSphere].pos.x, g_ameshsphere[nCntMeshSphere].pos.y, g_ameshsphere[nCntMeshSphere].pos.z);
		D3DXMatrixMultiply(&g_ameshsphere[nCntMeshSphere].mtxWorld, &g_ameshsphere[nCntMeshSphere].mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_ameshsphere[nCntMeshSphere].mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_ameshsphere[nCntMeshSphere].pVtxBuff, 0, sizeof(VERTEX_3D));

		// インデックスバッファをデータストリームに設定
		pDevice->SetIndices(g_ameshsphere[nCntMeshSphere].pIdxBuff);

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureMeshSphere);

		if (g_ameshsphere[nCntMeshSphere].bUse == true)
		{
			// メッシュスフィアの描画
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
				0,
				0,
				(MESHSPHERE_SPLIT_VERTICAL - 1) * MESHSPHERE_SPLIT_HORIZONTAL,
				0,
				((MESHSPHERE_SPLIT_HORIZONTAL - 1) * (MESHSPHERE_SPLIT_VERTICAL - 2) * 2) + ((MESHSPHERE_SPLIT_VERTICAL - 3) * 4));

		}
	}

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// アルファテストを無効化
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);	// 比較方法(すべて描画)
	pDevice->SetRenderState(D3DRS_ALPHAREF, 255);				// 基準値を設定(すべてを描画している)
}

//=============================================================================
//	メッシュスフィアの更新処理
//=============================================================================
void UpdateMeshSphere(void)
{

}

//=============================================================================
//	メッシュスフィアの設定処理
//=============================================================================
void SetMeshSphere(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	for (int nCntSphere = 0; nCntSphere < MAX_MESHSPHERE; nCntSphere++)
	{
		if (g_ameshsphere[nCntSphere].bUse == false)
		{
			// メッシュスフィアの設定
			g_ameshsphere[nCntSphere].pos = pos;
			g_ameshsphere[nCntSphere].rot = rot;
			g_ameshsphere[nCntSphere].bUse = true;
			break;
		}
	}
}