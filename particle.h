//=============================================================================
//
//	パーティクル処理 [particle.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "main.h"

//*****************************************************************************
// パーティクルの種類
//*****************************************************************************
typedef enum
{
	PARTICLETYPE_NORMAL = 0,		// 通常
	PARTICLETYPE_RAINBOW,			// 虹色
	PARTICLETYPE_MAX
}PARTICLETYPE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);
void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, float fRadiusDecrease, float fAlphaDecrease, int nLife, float fStartAngle, float fEndAngle);
void SetRainbowParticle(D3DXVECTOR3 pos, float fRadius, int nLife, float fStartAngle, float fEndAngle);

#endif // _PARTICLE_H_