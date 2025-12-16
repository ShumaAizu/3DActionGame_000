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
#define MESHSPHERE_SPLIT_VERTICAL		(8 + 1)											// 縦の分割数
#define MESHSPHERE_SPLIT_HORIZONTAL		(8 + 1)											// 横の分割数
#define MESHSPHERE_ANGLE_VERTICAL		(D3DX_PI / (MESHSPHERE_SPLIT_VERTICAL - 1))			// 縦の分割数に応じた角度
#define MESHSPHERE_ANGLE_HORIZONTAL		(D3DX_PI / (MESHSPHERE_SPLIT_HORIZONTAL - 1))		// 横の分割数に応じた角度

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureMeshSphere = NULL;				// テクスチャへのポインタ
MeshSphere g_ameshsphere[MAX_MESHSPHERE];					// メッシュスフィアの情報

//=============================================================================
//	メッシュスフィアの初期化処理
//=============================================================================
void InitMeshSphere(void)
{
	MeshSphere* pMeshSphere = &g_ameshsphere[0];
	int nCntMeshSphere;

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\ski002.jpg",
		&g_pTextureMeshSphere);

	for (nCntMeshSphere = 0; nCntMeshSphere < MAX_MESHSPHERE; nCntMeshSphere++, pMeshSphere++)
	{
		pMeshSphere->pVtxBuff = NULL;
		pMeshSphere->pVtxFanBuff = NULL;
		pMeshSphere->pIdxBuff = NULL;
		pMeshSphere->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pMeshSphere->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pMeshSphere->fRadius = 300.0f;
		pMeshSphere->bUse = false;

		// 頂点バッファの生成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (MESHSPHERE_SPLIT_VERTICAL - 1) * MESHSPHERE_SPLIT_HORIZONTAL,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&pMeshSphere->pVtxBuff,
			NULL);

		// 頂点バッファの生成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MESHSPHERE_SPLIT_HORIZONTAL,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&pMeshSphere->pVtxFanBuff,
			NULL);

		// 初期化
		VERTEX_3D *pVtx;			// 頂点情報へのポインタ
		VERTEX_3D* pVtxFan;			// 頂点情報へのポインタ

		float fAngleVertical = (D3DX_PI * 0.5f) - MESHSPHERE_ANGLE_VERTICAL;

		// 頂点バッファをロックし,頂点情報へのポインタを取得
		pMeshSphere->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pMeshSphere->pVtxFanBuff->Lock(0, 0, (void**)&pVtxFan, 0);

		for (int nCntVtxVertical = 0; nCntVtxVertical < MESHSPHERE_SPLIT_VERTICAL; nCntVtxVertical++)
		{
			D3DXVECTOR3 diff;
			D3DXVECTOR3 vecDir;
			float fRadius;
			float fAngleHorizontal = D3DX_PI;

			diff = D3DXVECTOR3(sinf(0.0f) * pMeshSphere->fRadius, cosf(fAngleVertical) * pMeshSphere->fRadius, 0.0f);

			fRadius = SQRTF(diff.x, diff.y);

			for (int nCntVtxHorizontal = 0; nCntVtxHorizontal < MESHSPHERE_SPLIT_HORIZONTAL; nCntVtxHorizontal++)
			{
				if (nCntVtxVertical == 0 || nCntVtxVertical == MESHSPHERE_SPLIT_VERTICAL - 1)
				{
					if (nCntVtxHorizontal == 0)
					{
						// 頂点座標の設定
						pVtxFan[nCntVtxHorizontal].pos = D3DXVECTOR3(0.0f, pMeshSphere->fRadius, 0.0f);
						fAngleHorizontal += MESHSPHERE_ANGLE_HORIZONTAL * 2.0f;
					}
					else
					{
						// 頂点座標の設定
						pVtxFan[nCntVtxHorizontal].pos = D3DXVECTOR3(sinf(fAngleHorizontal) * fRadius, sinf(fAngleVertical) * pMeshSphere->fRadius, cosf(fAngleHorizontal) * fRadius);
					}

					// 法線ベクトルの設定
					vecDir = D3DXVECTOR3(pVtxFan[nCntVtxHorizontal].pos.x, 0.0f, pVtxFan[nCntVtxHorizontal].pos.z);
					D3DXVec3Normalize(&vecDir, &vecDir);
					pVtxFan[nCntVtxHorizontal].nor = vecDir;

					// 頂点カラーの設定
					pVtxFan[nCntVtxHorizontal].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					// テクスチャ座標の設定
					pVtxFan[nCntVtxHorizontal].tex = D3DXVECTOR2(1.0f * nCntVtxHorizontal, 1.0f * nCntVtxVertical);
				}
				else
				{
					// 頂点座標の設定
					pVtx[nCntVtxHorizontal].pos = D3DXVECTOR3(sinf(fAngleHorizontal) * fRadius, sinf(fAngleVertical) * pMeshSphere->fRadius, cosf(fAngleHorizontal) * fRadius);

					// 法線ベクトルの設定
					vecDir = D3DXVECTOR3(pVtx[nCntVtxHorizontal].pos.x, 0.0f, pVtx[nCntVtxHorizontal].pos.z);
					D3DXVec3Normalize(&vecDir, &vecDir);
					pVtx[nCntVtxHorizontal].nor = vecDir;

					// 頂点カラーの設定
					pVtx[nCntVtxHorizontal].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					// テクスチャ座標の設定
					pVtx[nCntVtxHorizontal].tex = D3DXVECTOR2(1.0f * nCntVtxHorizontal, 1.0f * nCntVtxVertical);
				}

				// 角度を加算
				fAngleHorizontal += -MESHSPHERE_ANGLE_HORIZONTAL * 2.0f;
			}

			// 角度を減算
			fAngleVertical += -MESHSPHERE_ANGLE_VERTICAL;

			if (nCntVtxVertical == 0 || nCntVtxVertical == MESHSPHERE_SPLIT_VERTICAL - 1)
			{
				// 角度を減算
				fAngleVertical += MESHSPHERE_ANGLE_VERTICAL;
			}

			if (nCntVtxVertical != 0 && nCntVtxVertical != MESHSPHERE_SPLIT_VERTICAL - 1)
			{
				// ポインタを進める
				pVtx += MESHSPHERE_SPLIT_HORIZONTAL;
			}
		}

		// 頂点バッファをアンロックする
		pMeshSphere->pVtxBuff->Unlock();
		pMeshSphere->pVtxFanBuff->Unlock();

		// インデックスバッファの設定
		pDevice->CreateIndexBuffer(sizeof(WORD) * ((MESHSPHERE_SPLIT_HORIZONTAL) * 2 * (MESHSPHERE_SPLIT_VERTICAL - 1) + ((MESHSPHERE_SPLIT_VERTICAL - 2) * 2)),
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_MANAGED,
			&pMeshSphere->pIdxBuff,
			NULL);

		WORD* pIdx;		// インデックス情報へのポインタ

		int nCntIdx = 0;

		// インデックスバッファをロックし、頂点情報へのポインタを取得
		pMeshSphere->pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

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
		pMeshSphere->pIdxBuff->Unlock();
	}

	SetMeshSphere(D3DXVECTOR3(0.0f, 100.0f, -1000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
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

		if (g_ameshsphere[nCntMeshSphere].pVtxFanBuff != NULL)
		{
			g_ameshsphere[nCntMeshSphere].pVtxFanBuff->Release();
			g_ameshsphere[nCntMeshSphere].pVtxFanBuff = NULL;
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
	MeshSphere* pMeshSphere = &g_ameshsphere[0];

	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// 比較方法(基準値より大きければ描画)
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);					// アルファテストの参照値を設定(～以上で描画, intで設定)

	for (int nCntMeshSphere = 0; nCntMeshSphere < MAX_MESHSPHERE; nCntMeshSphere++, pMeshSphere++)
	{
		if (pMeshSphere->bUse == false)
		{
			continue;
		}

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pMeshSphere->mtxWorld);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pMeshSphere->rot.y, pMeshSphere->rot.x, pMeshSphere->rot.z);
		D3DXMatrixMultiply(&pMeshSphere->mtxWorld, &pMeshSphere->mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, pMeshSphere->pos.x, pMeshSphere->pos.y, pMeshSphere->pos.z);
		D3DXMatrixMultiply(&pMeshSphere->mtxWorld, &pMeshSphere->mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &pMeshSphere->mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, pMeshSphere->pVtxFanBuff, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// ファンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, MESHSPHERE_SPLIT_HORIZONTAL - 1);

		break;

		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureMeshSphere);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, pMeshSphere->pVtxBuff, 0, sizeof(VERTEX_3D));

		// インデックスバッファをデータストリームに設定
		pDevice->SetIndices(pMeshSphere->pIdxBuff);

		// メッシュスフィアの描画
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
			0,
			0,
			(MESHSPHERE_SPLIT_VERTICAL - 1) * MESHSPHERE_SPLIT_HORIZONTAL,
			0,
			((MESHSPHERE_SPLIT_HORIZONTAL - 1) * (MESHSPHERE_SPLIT_VERTICAL - 2) * 2) + ((MESHSPHERE_SPLIT_VERTICAL - 3) * 4));
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