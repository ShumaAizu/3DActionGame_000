//=============================================================================
//
//	メッシュリング処理 [meshring.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "meshring.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MESHRING_SPLIT		(32 + 1)		// 分割数
#define MESHRING_ANGLE		(D3DX_PI / (MESHRING_SPLIT - 1))

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureMeshRing = NULL;		// テクスチャへのポインタ
MeshRing g_ameshring[MAX_MESHRING];					// メッシュリングの情報

//=============================================================================
//	メッシュリングの初期化処理
//=============================================================================
void InitMeshRing(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\ring000.jpg",
		&g_pTextureMeshRing);

	for (int nCntMeshRing = 0; nCntMeshRing < MAX_MESHRING; nCntMeshRing++)
	{
		g_ameshring[nCntMeshRing].pVtxBuff = NULL;
		g_ameshring[nCntMeshRing].pIdxBuff = NULL;
		g_ameshring[nCntMeshRing].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_ameshring[nCntMeshRing].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_ameshring[nCntMeshRing].fInRadius = 50.0f;
		g_ameshring[nCntMeshRing].fOutRadius = 100.0f;
		g_ameshring[nCntMeshRing].bUse = false;

		// 頂点バッファの生成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MESHRING_SPLIT * MESHRING_SPLIT,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&g_ameshring[nCntMeshRing].pVtxBuff,
			NULL);
	}

	// 初期化
	VERTEX_3D *pVtx;			// 頂点情報へのポインタ

	for (int nCntMeshRing = 0; nCntMeshRing < MAX_MESHRING; nCntMeshRing++)
	{
		// 頂点バッファをロックし,頂点情報へのポインタを取得
		g_ameshring[nCntMeshRing].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		float fAngle = D3DX_PI;

		for (int nCntVtx = 0; nCntVtx < MESHRING_SPLIT; nCntVtx++)
		{
			pVtx[0].pos = D3DXVECTOR3(sinf(fAngle) * g_ameshring[nCntMeshRing].fOutRadius, 0.0f, cosf(fAngle) * g_ameshring[nCntMeshRing].fOutRadius);
			pVtx[1].pos = D3DXVECTOR3(sinf(fAngle) * g_ameshring[nCntMeshRing].fInRadius, 0.0f, cosf(fAngle) * g_ameshring[nCntMeshRing].fInRadius);

			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			pVtx[0].tex = D3DXVECTOR2(1.0f * nCntVtx, 1.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f * nCntVtx, 0.0f);

			fAngle += -MESHRING_ANGLE * 2;
			pVtx += 2;
		}
		
		// 頂点バッファをアンロックする
		g_ameshring[nCntMeshRing].pVtxBuff->Unlock();
	}

	for (int nCntMeshRing = 0; nCntMeshRing < MAX_MESHRING; nCntMeshRing++)
	{
		// インデックスバッファの設定
		pDevice->CreateIndexBuffer(sizeof(WORD) * MESHRING_SPLIT * MESHRING_SPLIT,
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_MANAGED,
			&g_ameshring[nCntMeshRing].pIdxBuff,
			NULL);

		WORD* pIdx;		// インデックス情報へのポインタ

		// インデックスバッファをロックし、頂点情報へのポインタを取得
		g_ameshring[nCntMeshRing].pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

		for (int nCntVtx = 0; nCntVtx < MESHRING_SPLIT * MESHRING_SPLIT; nCntVtx++)
		{
			pIdx[nCntVtx] = nCntVtx;
		}

		// インデックスバッファをアンロックする
		g_ameshring[nCntMeshRing].pIdxBuff->Unlock();
	}

	//SetMeshRing(D3DXVECTOR3(0.0f, 50.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

//=============================================================================
//	メッシュリングの終了処理
//=============================================================================
void UninitMeshRing(void)
{
	// テクスチャの破棄
	if (g_pTextureMeshRing != NULL)
	{
		g_pTextureMeshRing->Release();
		g_pTextureMeshRing = NULL;
	}
	
	for (int nCntMeshRing = 0; nCntMeshRing < MAX_MESHRING; nCntMeshRing++)
	{
		// 頂点バッファの破棄
		if (g_ameshring[nCntMeshRing].pVtxBuff != NULL)
		{
			g_ameshring[nCntMeshRing].pVtxBuff->Release();
			g_ameshring[nCntMeshRing].pVtxBuff = NULL;
		}

		// インデックスバッファの破棄
		if (g_ameshring[nCntMeshRing].pIdxBuff != NULL)
		{
			g_ameshring[nCntMeshRing].pIdxBuff->Release();
			g_ameshring[nCntMeshRing].pIdxBuff = NULL;
		}
	}
}

//=============================================================================
//	メッシュリングの描画処理
//=============================================================================
void DrawMeshRing(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス

	for (int nCntMeshRing = 0; nCntMeshRing < MAX_MESHRING; nCntMeshRing++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_ameshring[nCntMeshRing].mtxWorld);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_ameshring[nCntMeshRing].rot.y, g_ameshring[nCntMeshRing].rot.x, g_ameshring[nCntMeshRing].rot.z);
		D3DXMatrixMultiply(&g_ameshring[nCntMeshRing].mtxWorld, &g_ameshring[nCntMeshRing].mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_ameshring[nCntMeshRing].pos.x, g_ameshring[nCntMeshRing].pos.y, g_ameshring[nCntMeshRing].pos.z);
		D3DXMatrixMultiply(&g_ameshring[nCntMeshRing].mtxWorld, &g_ameshring[nCntMeshRing].mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_ameshring[nCntMeshRing].mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_ameshring[nCntMeshRing].pVtxBuff, 0, sizeof(VERTEX_3D));

		// インデックスバッファをデータストリームに設定
		pDevice->SetIndices(g_ameshring[nCntMeshRing].pIdxBuff);

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureMeshRing);

		if (g_ameshring[nCntMeshRing].bUse == true)
		{
			// メッシュリングの描画
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
				0,
				0,
				MESHRING_SPLIT * MESHRING_SPLIT,
				0,
				(MESHRING_SPLIT - 1) + (MESHRING_SPLIT - 1));
		}
	}
}

//=============================================================================
//	メッシュリングの更新処理
//=============================================================================
void UpdateMeshRing(void)
{
	// 初期化
	VERTEX_3D* pVtx;			// 頂点情報へのポインタ

	if (GetKeyboardPress(DIK_9) == true)
	{
		g_ameshring[0].fInRadius += 5.0f;
		g_ameshring[0].fOutRadius += 5.0f;
	}

	if (GetKeyboardPress(DIK_8) == true)
	{
		g_ameshring[0].fInRadius += -5.0f;
		g_ameshring[0].fOutRadius += -5.0f;
	}

	for (int nCntMeshRing = 0; nCntMeshRing < MAX_MESHRING; nCntMeshRing++)
	{
		if (g_ameshring[nCntMeshRing].bUse == false)
		{
			continue;
		}

		// 頂点バッファをロックし,頂点情報へのポインタを取得
		g_ameshring[nCntMeshRing].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		float fAngle = D3DX_PI;

		for (int nCntVtx = 0; nCntVtx < MESHRING_SPLIT; nCntVtx++)
		{
			pVtx[0].pos = D3DXVECTOR3(sinf(fAngle) * g_ameshring[nCntMeshRing].fOutRadius, 0.0f, cosf(fAngle) * g_ameshring[nCntMeshRing].fOutRadius);
			pVtx[1].pos = D3DXVECTOR3(sinf(fAngle) * g_ameshring[nCntMeshRing].fInRadius, 0.0f, cosf(fAngle) * g_ameshring[nCntMeshRing].fInRadius);

			fAngle += -MESHRING_ANGLE * 2;
			pVtx += 2;
		}

		// 頂点バッファをアンロックする
		g_ameshring[nCntMeshRing].pVtxBuff->Unlock();
	}
}

//=============================================================================
//	メッシュリングの設定処理
//=============================================================================
void SetMeshRing(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	for (int nCntRing = 0; nCntRing < MAX_MESHRING; nCntRing++)
	{
		if (g_ameshring[nCntRing].bUse == false)
		{
			// メッシュリングの設定
			g_ameshring[nCntRing].pos = pos;
			g_ameshring[nCntRing].rot = rot;
			g_ameshring[nCntRing].bUse = true;
			break;
		}
	}
}