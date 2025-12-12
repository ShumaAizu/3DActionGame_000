//=============================================================================
//
//	メッシュドーム処理 [meshdome.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "meshdome.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MESHDOME_SPLIT_VERTICAL		(64 + 1)											// 縦の分割数
#define MESHDOME_SPLIT_HORIZONTAL	(64 + 1)											// 横の分割数
#define MESHDOME_ANGLE_VERTICAL		(D3DX_PI / (MESHDOME_SPLIT_VERTICAL))				// 縦の分割数に応じた角度
#define MESHDOME_ANGLE_HORIZONTAL	(D3DX_PI / (MESHDOME_SPLIT_HORIZONTAL - 1))			// 横の分割数に応じた角度

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureMeshDome = NULL;		// テクスチャへのポインタ
MeshDome g_ameshdome[MAX_MESHDOME];					// メッシュドームの情報

//=============================================================================
//	メッシュドームの初期化処理
//=============================================================================
void InitMeshDome(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\sky004.jpg",
		&g_pTextureMeshDome);

	for (int nCntMeshDome = 0; nCntMeshDome < MAX_MESHDOME; nCntMeshDome++)
	{
		g_ameshdome[nCntMeshDome].pVtxBuff = NULL;
		g_ameshdome[nCntMeshDome].pIdxBuff = NULL;
		g_ameshdome[nCntMeshDome].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_ameshdome[nCntMeshDome].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_ameshdome[nCntMeshDome].tex = D3DXVECTOR2(0.0f, 0.0f);
		g_ameshdome[nCntMeshDome].fRadius = 15000.0f;
		g_ameshdome[nCntMeshDome].bUse = false;
	}

	SetMeshDome(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 5000.0f, MESHDOME_SPLIT_VERTICAL - 1, MESHDOME_SPLIT_HORIZONTAL - 1);
	//SetMeshDome(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 2500.0f, MESHDOME_SPLIT_VERTICAL - 1, MESHDOME_SPLIT_HORIZONTAL - 1);
}

//=============================================================================
//	メッシュドームの終了処理
//=============================================================================
void UninitMeshDome(void)
{
	// テクスチャの破棄
	if (g_pTextureMeshDome != NULL)
	{
		g_pTextureMeshDome->Release();
		g_pTextureMeshDome = NULL;
	}
	
	for (int nCntMeshDome = 0; nCntMeshDome < MAX_MESHDOME; nCntMeshDome++)
	{
		// 頂点バッファの破棄
		if (g_ameshdome[nCntMeshDome].pVtxBuff != NULL)
		{
			g_ameshdome[nCntMeshDome].pVtxBuff->Release();
			g_ameshdome[nCntMeshDome].pVtxBuff = NULL;
		}

		// インデックスバッファの破棄
		if (g_ameshdome[nCntMeshDome].pIdxBuff != NULL)
		{
			g_ameshdome[nCntMeshDome].pIdxBuff->Release();
			g_ameshdome[nCntMeshDome].pIdxBuff = NULL;
		}
	}
}

//=============================================================================
//	メッシュドームの描画処理
//=============================================================================
void DrawMeshDome(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);		// ライトを無効にする

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// 比較方法(基準値より大きければ描画)
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);					// アルファテストの参照値を設定(～以上で描画, intで設定)

	for (int nCntMeshDome = 0; nCntMeshDome < MAX_MESHDOME; nCntMeshDome++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_ameshdome[nCntMeshDome].mtxWorld);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_ameshdome[nCntMeshDome].rot.y, g_ameshdome[nCntMeshDome].rot.x, g_ameshdome[nCntMeshDome].rot.z);
		D3DXMatrixMultiply(&g_ameshdome[nCntMeshDome].mtxWorld, &g_ameshdome[nCntMeshDome].mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_ameshdome[nCntMeshDome].pos.x, g_ameshdome[nCntMeshDome].pos.y, g_ameshdome[nCntMeshDome].pos.z);
		D3DXMatrixMultiply(&g_ameshdome[nCntMeshDome].mtxWorld, &g_ameshdome[nCntMeshDome].mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_ameshdome[nCntMeshDome].mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_ameshdome[nCntMeshDome].pVtxBuff, 0, sizeof(VERTEX_3D));

		// インデックスバッファをデータストリームに設定
		pDevice->SetIndices(g_ameshdome[nCntMeshDome].pIdxBuff);

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureMeshDome);

		if (g_ameshdome[nCntMeshDome].bUse == true)
		{
			// メッシュドームの描画
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
				0,
				0,
				(g_ameshdome[nCntMeshDome].nVertical - 1) * g_ameshdome[nCntMeshDome].nHorizontal,
				0,
				((g_ameshdome[nCntMeshDome].nHorizontal - 1) * (g_ameshdome[nCntMeshDome].nVertical - 2) * 2) + ((g_ameshdome[nCntMeshDome].nVertical - 3) * 4));

		}
	}

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);		// ライトを有効にする

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// アルファテストを無効化
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);	// 比較方法(すべて描画)
	pDevice->SetRenderState(D3DRS_ALPHAREF, 255);				// 基準値を設定(すべてを描画している)
}

//=============================================================================
//	メッシュドームの更新処理
//=============================================================================
void UpdateMeshDome(void)
{
	PMESHDOME pMeshDome = &g_ameshdome[0];

	for (int nCntMeshDome = 0; nCntMeshDome < MAX_MESHDOME; nCntMeshDome++, pMeshDome++)
	{
		if (pMeshDome->bUse == false)
		{
			continue;
		}

		pMeshDome->tex.x += 0.00025f;

		VERTEX_3D* pVtx;			// 頂点情報へのポインタ

		// 頂点バッファをロックし,頂点情報へのポインタを取得
		pMeshDome->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		float fAngleVertical = (D3DX_PI * 0.5f) - (D3DX_PI / pMeshDome->nVertical);

		for (int nCntVtxVertical = 0; nCntVtxVertical < pMeshDome->nVertical - 1; nCntVtxVertical++)
		{
			for (int nCntVtxHorizontal = 0; nCntVtxHorizontal < pMeshDome->nHorizontal; nCntVtxHorizontal++)
			{
				// テクスチャ座標の設定
				pVtx[nCntVtxHorizontal].tex = D3DXVECTOR2((1.0f / (pMeshDome->nHorizontal / 2)) * nCntVtxHorizontal + pMeshDome->tex.x, (pMeshDome->tex.y / pMeshDome->nVertical) * nCntVtxVertical);
			}

			// ポインタを進める
			pVtx += pMeshDome->nHorizontal;
		}

		// 頂点バッファをアンロックする
		pMeshDome->pVtxBuff->Unlock();

	}
}

//=============================================================================
//	メッシュドームの設定処理
//=============================================================================
void SetMeshDome(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, int nVertical, int nHorizontal)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	PMESHDOME pMeshDome = &g_ameshdome[0];

	for (int nCntMeshDome = 0; nCntMeshDome < MAX_MESHDOME; nCntMeshDome++, pMeshDome++)
	{
		if (pMeshDome->bUse == true)
		{
			continue;
		}

		// 分割数を頂点数に合わせる
		nVertical++;
		nHorizontal++;

		// メッシュドームの設定
		pMeshDome->pos = pos;
		pMeshDome->rot = rot;
		pMeshDome->tex = D3DXVECTOR2(1.0f, 1.0f);
		pMeshDome->fRadius = fRadius;
		pMeshDome->nVertical = nVertical;
		pMeshDome->nHorizontal = nHorizontal;

		// 頂点バッファの生成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (nVertical - 1) * nHorizontal,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&pMeshDome->pVtxBuff,
			NULL);

		// 初期化
		VERTEX_3D* pVtx;			// 頂点情報へのポインタ

		// 頂点バッファをロックし,頂点情報へのポインタを取得
		pMeshDome->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		float fAngleVertical = (D3DX_PI * 0.5f) - (D3DX_PI / nVertical);

		for (int nCntVtxVertical = 0; nCntVtxVertical < nVertical - 1; nCntVtxVertical++)
		{
			D3DXVECTOR3 diff;
			D3DXVECTOR3 vecDir;
			float fRadius;
			float fAngleHorizontal = -D3DX_PI;

			diff = D3DXVECTOR3(sinf(0.0f) * pMeshDome->fRadius, cosf(fAngleVertical) * pMeshDome->fRadius, cosf(0.0f) * pMeshDome->fRadius);

			fRadius = SQRTF(diff.x, diff.y);

			for (int nCntVtxHorizontal = 0; nCntVtxHorizontal < nHorizontal; nCntVtxHorizontal++)
			{
				// 頂点座標の設定
				pVtx[nCntVtxHorizontal].pos = D3DXVECTOR3(sinf(fAngleHorizontal) * fRadius, sinf(fAngleVertical) * pMeshDome->fRadius, cosf(fAngleHorizontal) * fRadius);

				// 法線ベクトルの設定
				vecDir = D3DXVECTOR3(pVtx[nCntVtxHorizontal].pos.x, 0.0f, pVtx[nCntVtxHorizontal].pos.z);
				D3DXVec3Normalize(&vecDir, &vecDir);
				pVtx[nCntVtxHorizontal].nor = vecDir;

				// 頂点カラーの設定
				pVtx[nCntVtxHorizontal].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// テクスチャ座標の設定
				pVtx[nCntVtxHorizontal].tex = D3DXVECTOR2((1.0f / (nHorizontal / 2)) * nCntVtxHorizontal, (1.0f / nVertical) * nCntVtxVertical);

				// 角度を加算
				fAngleHorizontal += (D3DX_PI / (nHorizontal - 1)) * 2.0f;	// 合計6.28加算したいので倍にする
			}

			// 角度を減算
			fAngleVertical += -(D3DX_PI / nVertical) * 0.5f;			// 半円で留めたいので半分にして減算

			// ポインタを進める
			pVtx += nHorizontal;
		}

		// 頂点バッファをアンロックする
		pMeshDome->pVtxBuff->Unlock();

		// インデックスバッファの設定
		pDevice->CreateIndexBuffer(sizeof(WORD) * ((nHorizontal) * 2 * (nVertical - 2) + ((nVertical - 3) * 2)),
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_MANAGED,
			&pMeshDome->pIdxBuff,
			NULL);

		WORD* pIdx;		// インデックス情報へのポインタ

		// インデックスバッファをロックし、頂点情報へのポインタを取得
		pMeshDome->pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

		for (int nCntIdxVertical = 0; nCntIdxVertical < nVertical - 2; nCntIdxVertical++)
		{
			for (int nCntIdxHorizontal = 0; nCntIdxHorizontal < nHorizontal; nCntIdxHorizontal++)
			{
				if (nCntIdxVertical != 0 && nCntIdxHorizontal == 0)
				{// 高さが最初の行以外かつ幅が最初の行
					pIdx[0] = (nHorizontal * (nCntIdxVertical + 1)) + nCntIdxHorizontal;

					pIdx++;
				}

				pIdx[0] = (nHorizontal * (nCntIdxVertical + 1)) + nCntIdxHorizontal;
				pIdx[1] = nHorizontal * nCntIdxVertical + nCntIdxHorizontal;

				pIdx += 2;

				if (nCntIdxVertical != nHorizontal - 2 && nCntIdxHorizontal == nHorizontal - 1)
				{// 高さが最後の行以外かつ幅が最後の行
					pIdx[0] = nHorizontal * nCntIdxVertical + nCntIdxHorizontal;

					pIdx++;
				}
			}
		}

		// インデックスバッファをアンロックする
		pMeshDome->pIdxBuff->Unlock();

		pMeshDome->bUse = true;
		break;
	}
}