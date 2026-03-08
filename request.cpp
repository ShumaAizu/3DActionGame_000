//=============================================================================
//
//	リクエスト処理 [request.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "request.h"
#include "input.h"
#include "shadow.h"
#include "camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_REQUEST				(128)			// リクエストの最大数

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_apTextureRequest[REQUESTTYPE_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRequest = NULL;				// 頂点バッファへのポインタ
Request g_arequest[MAX_REQUEST];								// リクエストの情報

const char* c_pRequestTexName[REQUESTTYPE_MAX] =
{
	"data\\TEXTURE\\request000.png",
	"data\\TEXTURE\\tutorial000.png",
	"data\\TEXTURE\\request001.png"
};

//=============================================================================
//	リクエストの初期化処理
//=============================================================================
void InitRequest(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	Request* pReqest = &g_arequest[0];

	for (int nCntTex = 0; nCntTex < REQUESTTYPE_MAX; nCntTex++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			c_pRequestTexName[nCntTex],
			&g_apTextureRequest[nCntTex]);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_REQUEST * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRequest,
		NULL);

	// 初期化
	memset(pReqest, NULL, sizeof(pReqest) * MAX_REQUEST);

	VERTEX_3D *pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffRequest->Lock(0, 0, (void * *)&pVtx, 0);

	for (int nCntRequest = 0; nCntRequest < MAX_REQUEST; nCntRequest++, pReqest++)
	{
		pReqest->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pReqest->col = COLOR_WHITE;
		pReqest->col.a = 0.0f;
		pReqest->fRadius = 30.0f;
		pReqest->bUse = false;

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-pReqest->fRadius, pReqest->fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pReqest->fRadius, pReqest->fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-pReqest->fRadius, -pReqest->fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pReqest->fRadius, -pReqest->fRadius, 0.0f);

		// 法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 頂点カラーの設定
		pVtx[0].col = pReqest->col;
		pVtx[1].col = pReqest->col;
		pVtx[2].col = pReqest->col;
		pVtx[3].col = pReqest->col;

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffRequest->Unlock();
}

//=============================================================================
//	リクエストの終了処理
//=============================================================================
void UninitRequest(void)
{
	// テクスチャの破棄
	for (int nCntTex = 0; nCntTex < REQUESTTYPE_MAX; nCntTex++)
	{
		if (g_apTextureRequest[nCntTex] != NULL)
		{
			g_apTextureRequest[nCntTex]->Release();
			g_apTextureRequest[nCntTex] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffRequest != NULL)
	{
		g_pVtxBuffRequest->Release();
		g_pVtxBuffRequest = NULL;
	}
}

//=============================================================================
//	リクエストの描画処理
//=============================================================================
void DrawRequest(void)
{
	Request* pRequest = &g_arequest[0];

	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス
	D3DXMATRIX mtxView;							// ビューマトリックス

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);		// ライトを無効にする

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// 比較方法(基準値より大きければ描画)
	pDevice->SetRenderState(D3DRS_ALPHAREF, 50);				// アルファテストの参照値を設定(～以上で描画, intで設定)

	for (int nCntRequest = 0; nCntRequest < MAX_REQUEST; nCntRequest++, pRequest++)
	{
		if (pRequest->bUse == false)
		{
			continue;
		}

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pRequest->mtxWorld);

		// ビューマトリックスの取得
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		// ポリゴンとカメラに対して正面を向ける
		D3DXMatrixInverse(&pRequest->mtxWorld, NULL, &mtxView);	// 逆行列を求める
		pRequest->mtxWorld._41 = 0.0f;
		pRequest->mtxWorld._42 = 0.0f;
		pRequest->mtxWorld._43 = 0.0f;

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, pRequest->pos.x, pRequest->pos.y, pRequest->pos.z);
		D3DXMatrixMultiply(&pRequest->mtxWorld, &pRequest->mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &pRequest->mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffRequest, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_apTextureRequest[pRequest->type]);

		// リクエストの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntRequest * 4, 2);
	}

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);		// ライトを有効にする

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// アルファテストを無効化
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);	// 比較方法(すべて描画)
	pDevice->SetRenderState(D3DRS_ALPHAREF, 255);				// 基準値を設定(すべてを描画している)
}

//=============================================================================
//	リクエストの更新処理
//=============================================================================
void UpdateRequest(void)
{
	Request* pRequest = &g_arequest[0];

	// 初期化
	VERTEX_3D* pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffRequest->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntRequest = 0; nCntRequest < MAX_REQUEST; nCntRequest++, pRequest++, pVtx += 4)
	{
		if (pRequest->bUse == false)
		{
			continue;
		}

		if (pRequest->bDisp == true)
		{
			if (pRequest->col.a < 1.0f)
			{
				pRequest->col.a += 0.05f;
			}
		}
		else
		{
			if (pRequest->col.a > 0.0f)
			{
				pRequest->col.a -= 0.05f;
			}
		}

		if (pRequest->type == REQUESTTYPE_TUTORIAL)
		{
			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-pRequest->fRadius * 0.5f, pRequest->fRadius * 0.25f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pRequest->fRadius * 0.5f, pRequest->fRadius * 0.25f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-pRequest->fRadius * 0.5f, -pRequest->fRadius * 0.25f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pRequest->fRadius * 0.5f, -pRequest->fRadius * 0.25f, 0.0f);
		}

		// 頂点カラーの設定
		pVtx[0].col = pRequest->col;
		pVtx[1].col = pRequest->col;
		pVtx[2].col = pRequest->col;
		pVtx[3].col = pRequest->col;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffRequest->Unlock();
}

//=============================================================================
//	リクエストの設定処理
//=============================================================================
int SetRequest(D3DXVECTOR3 pos, REQUESTTYPE type)
{
	Request* pRequest = &g_arequest[0];

	int nCntRequest = -1;

	for (nCntRequest = 0; nCntRequest < MAX_REQUEST; nCntRequest++, pRequest++)
	{
		if (pRequest->bUse == true)
		{
			continue;
		}

		pRequest->pos = pos;
		pRequest->type = type;

		pRequest->bDisp = true;
		pRequest->bUse = true;
		break;
	}

	return nCntRequest;
}

//=============================================================================
//	リクエストの位置更新処理
//=============================================================================
void SetPositionReqest(int nIdxReqest, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	if (nIdxReqest == -1)
	{
		return;
	}

	Camera* pCamera = GetCamera();
	switch (g_arequest[nIdxReqest].type)
	{
	case REQUESTTYPE_000:
		g_arequest[nIdxReqest].pos.x = pos.x + sinf(AngleNormalize(pCamera->rot.y + -0.5f * D3DX_PI)) * 32.5f;
		g_arequest[nIdxReqest].pos.y = pos.y + 50.0f;
		g_arequest[nIdxReqest].pos.z = pos.z + cosf(AngleNormalize(pCamera->rot.y + -0.5f * D3DX_PI)) * 32.5f;
		break;

	case REQUESTTYPE_TUTORIAL:
		g_arequest[nIdxReqest].pos.x = pos.x + sinf(AngleNormalize(pCamera->rot.y + 0.0f * D3DX_PI)) * 25.0f;
		g_arequest[nIdxReqest].pos.y = pos.y + 5.0f;
		g_arequest[nIdxReqest].pos.z = pos.z + cosf(AngleNormalize(pCamera->rot.y + 0.0f * D3DX_PI)) * 25.0f;
		break;

	case REQUESTTYPE_002:
		g_arequest[nIdxReqest].pos.x = pos.x + sinf(AngleNormalize(pCamera->rot.y + -0.5f * D3DX_PI)) * 32.5f;
		g_arequest[nIdxReqest].pos.y = pos.y + 50.0f;
		g_arequest[nIdxReqest].pos.z = pos.z + cosf(AngleNormalize(pCamera->rot.y + -0.5f * D3DX_PI)) * 32.5f;
		break;
	}
}

//=============================================================================
//	リクエストの解除処理
//=============================================================================
void ReleaseReqest(int nIdxReqest)
{
	if (nIdxReqest == -1)
	{
		return;
	}
	g_arequest[nIdxReqest].bUse = false;
}

//=============================================================================
//	リクエストの描画設定処理
//=============================================================================
void SetRequestDisp(int nIdxRequest, bool bDisp)
{
	if (nIdxRequest == -1)
	{
		return;
	}
	g_arequest[nIdxRequest].bDisp = bDisp;
}