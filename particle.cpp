//=============================================================================
//
//	パーティクル処理 [particle.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================
#include "particle.h"
#include "effect.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_PARTICLE		(128)		// パーティクルの最大数

//*****************************************************************************
// パーティクル構造体
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXCOLOR col;			// 色
	PARTICLETYPE type;		// 種類
	float fStartAngle;		// 開始地点角度
	float fEndAngle;		// 終了地点角度
	float fRadius;			// 半径
	float fRadiusDecrease;	// 半径を減らす速度
	float fAlphaDecrease;	// アルファ値を減らす速度
	int nLife;				// 寿命
	bool bUse;				// 使用状況
}Particle;

//*****************************************************************************
// グローバル変数
//*****************************************************************************
Particle g_aParticle[MAX_PARTICLE];			// パーティクルの情報

//====================================
//	パーティクルの初期化処理
//====================================
void InitParticle(void)
{
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		g_aParticle[nCntParticle].pos = {};
		g_aParticle[nCntParticle].col = {0.0f, 0.0f, 0.0f, 1.0f};
		g_aParticle[nCntParticle].type = PARTICLETYPE_NORMAL;
		g_aParticle[nCntParticle].fRadius = {};
		g_aParticle[nCntParticle].fRadiusDecrease = 1.25f;
		g_aParticle[nCntParticle].fAlphaDecrease = 0.025f;
		g_aParticle[nCntParticle].fStartAngle = {};
		g_aParticle[nCntParticle].fEndAngle = {};
		g_aParticle[nCntParticle].nLife = {};
		g_aParticle[nCntParticle].bUse = false;
	}
}

//====================================
//	パーティクルの終了処理
//====================================
void UninitParticle(void)
{

}

//====================================
//	パーティクルの更新処理
//====================================
void UpdateParticle(void)
{
	D3DXVECTOR3 pos = {};		// 位置
	D3DXVECTOR3 move = {};		// パーティクルの移動量
	D3DXCOLOR col = {};			// 色
	float fMove = {};			// 移動量
	float fRadius = {};			// 半径
	float fRadiusDecrease = 1.25f;	// 半径を減らす速度
	float fAlphaDecrease = 0.25f;	// アルファ値を減らす速度
	int nLife = {};				// 寿命

	float fAngle = {};
	int nStartAngle = {};
	int nEndAngle = {};

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == true)
		{
			for (int nCntAppear = 0; nCntAppear < 1; nCntAppear++)
			{
				pos = g_aParticle[nCntParticle].pos;

				//pos.x += (float)(rand() % 3000) / 100.0f + 1.0f;
				//pos.y += (float)(rand() % 3000) / 100.0f + 1.0f;

				nStartAngle = (int)(g_aParticle[nCntParticle].fStartAngle * 100) + 1;
				nEndAngle = (int)(g_aParticle[nCntParticle].fEndAngle * 100);

				nStartAngle = nStartAngle - nEndAngle;

				fAngle = (float)(rand() % nStartAngle + nEndAngle) / 100.0f;

				if (fAngle < -D3DX_PI)
				{
					fAngle += D3DX_PI * 2;
				}
				else if (fAngle > D3DX_PI)
				{
					fAngle -= D3DX_PI * 2;
				}

				fMove = (float)(rand() % 500) / 100.0f + 0.5f;

				move.x = sinf(fAngle) * fMove;
				move.y = cosf(fAngle) * fMove;

				switch (g_aParticle[nCntParticle].type)
				{
				case PARTICLETYPE_NORMAL:
					col = g_aParticle[nCntParticle].col;
					break;

				case PARTICLETYPE_RAINBOW:
					col.r = (float)(rand() % 101) / 100.0f;
					col.g = (float)(rand() % 101) / 100.0f;
					col.b = (float)(rand() % 101) / 100.0f;
					col.a = 1.0f;
					break;

				}

				fRadiusDecrease = g_aParticle[nCntParticle].fRadiusDecrease;
				fAlphaDecrease = g_aParticle[nCntParticle].fAlphaDecrease;

				fRadius = (float)(rand() % (int)g_aParticle[nCntParticle].fRadius) / 100.0f + 5.0f;
				nLife = 200;

				SetEffect(pos, move, col, fRadius, fRadiusDecrease, fAlphaDecrease, nLife);
			}

			g_aParticle[nCntParticle].nLife--;

			if (g_aParticle[nCntParticle].nLife <= 0)
			{
				g_aParticle[nCntParticle].bUse = false;
			}
		}
	}
}

//====================================
//	パーティクルの描画処理
//====================================
void DrawParticle(void)
{

}

//====================================
//	パーティクルの設定処理
//====================================
void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, float fRadiusDecrease, float fAlphaDecrease, int nLife, float fStartAngle, float fEndAngle)
{

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == false)
		{// パーティクルを使用していない
			g_aParticle[nCntParticle].pos = pos;							// 受け取った位置を代入
			g_aParticle[nCntParticle].col = col;							// 受け取った色を代入
			g_aParticle[nCntParticle].nLife = nLife;						// 受け取った寿命を代入
			g_aParticle[nCntParticle].fRadius = fRadius;					// 受け取った半径を代入
			g_aParticle[nCntParticle].fStartAngle = fStartAngle;			// 受け取った開始角度を代入
			g_aParticle[nCntParticle].fEndAngle = fEndAngle;				// 受け取った終了角度を代入
			g_aParticle[nCntParticle].fRadiusDecrease = fRadiusDecrease;	// 受け取った半径減数を代入
			g_aParticle[nCntParticle].fAlphaDecrease = fAlphaDecrease;		// 受け取ったアルファ値減数を代入
			g_aParticle[nCntParticle].type = PARTICLETYPE_NORMAL;
			g_aParticle[nCntParticle].bUse = true;			// エフェクトが使用されている状態にする
			break;		// ここでfor文を抜ける
		}
	}
}

//====================================
//	パーティクルの設定処理
//====================================
void SetRainbowParticle(D3DXVECTOR3 pos, float fRadius, int nLife, float fStartAngle, float fEndAngle)
{

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == false)
		{// パーティクルを使用していない
			g_aParticle[nCntParticle].pos = pos;			// 受け取った位置を代入
			g_aParticle[nCntParticle].nLife = nLife;		// 受け取った寿命を代入
			g_aParticle[nCntParticle].fRadius = fRadius;	// 受け取った半径を代入
			g_aParticle[nCntParticle].fStartAngle = fStartAngle;
			g_aParticle[nCntParticle].fEndAngle = fEndAngle;

			g_aParticle[nCntParticle].col.r = (float)(rand() % 101) / 100.0f;
			g_aParticle[nCntParticle].col.g = (float)(rand() % 101) / 100.0f;
			g_aParticle[nCntParticle].col.b = (float)(rand() % 101) / 100.0f;

			g_aParticle[nCntParticle].type = PARTICLETYPE_RAINBOW;

			g_aParticle[nCntParticle].bUse = true;			// エフェクトが使用されている状態にする
			break;		// ここでfor文を抜ける
		}
	}
}