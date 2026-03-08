//=============================================================================
//
//	リアクション処理 [reaction.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "reaction.h"
#include "input.h"
#include "shadow.h"
#include "camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_REACTION				(4096)			// リアクションの最大数

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_apTextureReaction[REACTIONTYPE_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffReaction = NULL;			// 頂点バッファへのポインタ
Reaction g_areaction[MAX_REACTION];									// リアクションの情報

const char* c_pReactionTexName[REACTIONTYPE_MAX] =
{
	"data\\TEXTURE\\reaction000.png",
	"data\\TEXTURE\\reaction001.png",
	"data\\TEXTURE\\reaction002.png"
};

//=============================================================================
//	リアクションの初期化処理
//=============================================================================
void InitReaction(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntTex = 0; nCntTex < REACTIONTYPE_MAX; nCntTex++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			c_pReactionTexName[nCntTex],
			&g_apTextureReaction[nCntTex]);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_REACTION * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffReaction,
		NULL);

	// 初期化
	VERTEX_3D *pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffReaction->Lock(0, 0, (void * *)&pVtx, 0);

	for (int nCntReaction = 0; nCntReaction < MAX_REACTION; nCntReaction++)
	{
		g_areaction[nCntReaction].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_areaction[nCntReaction].fRadius = 0.0f;
		g_areaction[nCntReaction].bUse = false;

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-g_areaction[nCntReaction].fRadius, g_areaction[nCntReaction].fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_areaction[nCntReaction].fRadius, g_areaction[nCntReaction].fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_areaction[nCntReaction].fRadius, -g_areaction[nCntReaction].fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_areaction[nCntReaction].fRadius, -g_areaction[nCntReaction].fRadius, 0.0f);

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

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffReaction->Unlock();
}

//=============================================================================
//	リアクションの終了処理
//=============================================================================
void UninitReaction(void)
{
	// テクスチャの破棄
	for (int nCntTex = 0; nCntTex < REACTIONTYPE_MAX; nCntTex++)
	{
		if (g_apTextureReaction[nCntTex] != NULL)
		{
			g_apTextureReaction[nCntTex]->Release();
			g_apTextureReaction[nCntTex] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffReaction != NULL)
	{
		g_pVtxBuffReaction->Release();
		g_pVtxBuffReaction = NULL;
	}
}

//=============================================================================
//	リアクションの描画処理
//=============================================================================
void DrawReaction(void)
{
	Reaction* pReaction = &g_areaction[0];

	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス
	D3DXMATRIX mtxView;							// ビューマトリックス

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);		// ライトを無効にする

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// 比較方法(基準値より大きければ描画)
	pDevice->SetRenderState(D3DRS_ALPHAREF, 175);				// アルファテストの参照値を設定(～以上で描画, intで設定)

	for (int nCntReaction = 0; nCntReaction < MAX_REACTION; nCntReaction++, pReaction++)
	{
		if (pReaction->bUse == false)
		{
			continue;
		}

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pReaction->mtxWorld);

		// ビューマトリックスの取得
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		// ポリゴンとカメラに対して正面を向ける
		D3DXMatrixInverse(&pReaction->mtxWorld, NULL, &mtxView);	// 逆行列を求める
		pReaction->mtxWorld._41 = 0.0f;
		pReaction->mtxWorld._42 = 0.0f;
		pReaction->mtxWorld._43 = 0.0f;

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, pReaction->pos.x, pReaction->pos.y, pReaction->pos.z);
		D3DXMatrixMultiply(&pReaction->mtxWorld, &pReaction->mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &pReaction->mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffReaction, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_apTextureReaction[pReaction->type]);

		// リアクションの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntReaction * 4, 2);
	}

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);		// ライトを有効にする

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// アルファテストを無効化
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);	// 比較方法(すべて描画)
	pDevice->SetRenderState(D3DRS_ALPHAREF, 255);				// 基準値を設定(すべてを描画している)
}

//=============================================================================
//	リアクションの更新処理
//=============================================================================
void UpdateReaction(void)
{
	Reaction* pReaction = &g_areaction[0];

	for (int nCntReaction = 0; nCntReaction < MAX_REACTION; nCntReaction++, pReaction++)
	{
		if (pReaction->bUse == false)
		{
			continue;
		}

		pReaction->pos += pReaction->move;

		pReaction->nLife--;

		if (pReaction->nLife < 0)
		{
			pReaction->bUse = false;
		}

	}

	for (int nCntReaction = 0; nCntReaction < 15; nCntReaction++)
	{
		SetReaction(D3DXVECTOR3((float)(rand() % 6000 - 3000), (float)(rand() % 500 + 300), (float)(rand() % 6000 - 3000)), D3DXVECTOR3(-(float)(rand() % 5), -(float)(rand() % 5 + 1), 0.0f), (float)(rand() % 5 + 1), (REACTIONTYPE)(rand() % REACTIONTYPE_MAX));
	}
}

//=============================================================================
//	リアクションの設定処理
//=============================================================================
void SetReaction(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fRadius, REACTIONTYPE type)
{
	Reaction* pReaction = &g_areaction[0];

	VERTEX_3D* pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffReaction->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntReaction = 0; nCntReaction < MAX_REACTION; nCntReaction++, pVtx += 4, pReaction++)
	{
		if (pReaction->bUse == true)
		{
			continue;
		}

		pReaction->pos = pos;
		pReaction->move = move;
		pReaction->fRadius = fRadius;
		pReaction->type = type;
		pReaction->nLife = 60;

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-pReaction->fRadius, pReaction->fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pReaction->fRadius, pReaction->fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-pReaction->fRadius, -pReaction->fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pReaction->fRadius, -pReaction->fRadius, 0.0f);

		g_areaction[nCntReaction].bUse = true;
		break;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffReaction->Unlock();
}