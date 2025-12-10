//=============================================================================
//
//	ビルボード処理 [billboard.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "billboard.h"
#include "input.h"
#include "shadow.h"
#include "camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureBillboard = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillboard = NULL;	// 頂点バッファへのポインタ
Billboard g_billboard;								// ビルボードの情報
D3DXMATRIX g_mtxWorldBillboard;						// ワールドマトリックス
int g_nIdxShadowBillboard = -1;						// 影のインデックス

//=============================================================================
//	ビルボードの初期化処理
//=============================================================================
void InitBillboard(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\ball000.png",
		&g_pTextureBillboard);

	g_billboard.pos = D3DXVECTOR3(0.0f, 100.0f, 300.0f);
	g_billboard.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_nIdxShadowBillboard = SetShadow();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBillboard,
		NULL);

	// 初期化
	VERTEX_3D *pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	g_pVtxBuffBillboard->Lock(0, 0, (void * *)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-BILLBOARD_WIDTH, BILLBOARD_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(BILLBOARD_WIDTH, BILLBOARD_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-BILLBOARD_WIDTH, -BILLBOARD_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(BILLBOARD_WIDTH, -BILLBOARD_HEIGHT, 0.0f);

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
	g_pVtxBuffBillboard->Unlock();
}

//=============================================================================
//	ビルボードの終了処理
//=============================================================================
void UninitBillboard(void)
{
	// テクスチャの破棄
	if (g_pTextureBillboard != NULL)
	{
		g_pTextureBillboard->Release();
		g_pTextureBillboard = NULL;
	}
	

	// 頂点バッファの破棄
	if (g_pVtxBuffBillboard != NULL)
	{
		g_pVtxBuffBillboard->Release();
		g_pVtxBuffBillboard = NULL;
	}
}

//=============================================================================
//	ビルボードの描画処理
//=============================================================================
void DrawBillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス
	D3DXMATRIX mtxView;							// ビューマトリックス

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// 比較方法(基準値より大きければ描画)
	pDevice->SetRenderState(D3DRS_ALPHAREF, 175);				// アルファテストの参照値を設定(～以上で描画, intで設定)

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldBillboard);

	// ビューマトリックスの取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// ポリゴンとカメラに対して正面を向ける
	D3DXMatrixInverse(&g_mtxWorldBillboard, NULL, &mtxView);	// 逆行列を求める
	g_mtxWorldBillboard._41 = 0.0f;
	g_mtxWorldBillboard._42 = 0.0f;
	g_mtxWorldBillboard._43 = 0.0f;

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_billboard.rot.y, g_billboard.rot.x, g_billboard.rot.z);
	D3DXMatrixMultiply(&g_mtxWorldBillboard, &g_mtxWorldBillboard, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_billboard.pos.x, g_billboard.pos.y, g_billboard.pos.z);
	D3DXMatrixMultiply(&g_mtxWorldBillboard, &g_mtxWorldBillboard, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldBillboard);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBillboard, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureBillboard);

	// ビルボードの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);		// アルファテストを無効化
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);	// 比較方法(すべて描画)
	pDevice->SetRenderState(D3DRS_ALPHAREF, 255);				// 基準値を設定(すべてを描画している)
}

//=============================================================================
//	ビルボードの更新処理
//=============================================================================
void UpdateBillboard(void)
{
	Camera* pCamera = GetCamera();

	if (GetKeyboardPress(DIK_UPARROW) == true)
	{
		g_billboard.pos.x -= sinf(pCamera->rot.y) * 1.0f;
		g_billboard.pos.z -= cosf(pCamera->rot.y) * 1.0f;
	}

	if (GetKeyboardPress(DIK_LEFTARROW) == true)
	{
		g_billboard.pos.x += sinf(pCamera->rot.y + 0.5f * D3DX_PI) * 1.0f;
		g_billboard.pos.z += cosf(pCamera->rot.y + 0.5f * D3DX_PI) * 1.0f;
	}

	if (GetKeyboardPress(DIK_DOWNARROW) == true)
	{
		g_billboard.pos.x += sinf(pCamera->rot.y) * 1.0f;
		g_billboard.pos.z += cosf(pCamera->rot.y) * 1.0f;
	}

	if (GetKeyboardPress(DIK_RIGHTARROW) == true)
	{
		g_billboard.pos.x -= sinf(pCamera->rot.y + 0.5f * D3DX_PI) * 1.0f;
		g_billboard.pos.z -= cosf(pCamera->rot.y + 0.5f * D3DX_PI) * 1.0f;
	}

	SetPositionShadow(g_nIdxShadowBillboard, D3DXVECTOR3(g_billboard.pos.x, g_billboard.pos.y - g_billboard.pos.y, g_billboard.pos.z));
}