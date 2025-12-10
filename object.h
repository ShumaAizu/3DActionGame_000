////=============================================================================
////
////	オブジェクト処理 [object.h]
////	Author : SHUMA AIZU
//// 
////=============================================================================
//
//#ifndef _OBJECT_H_
//#define _OBJECT_H_
//
//#include "main.h"
//#include "player.h"
//
////*****************************************************************************
//// マクロ定義
////*****************************************************************************
//#define MAX_OBJECTTEX				(16)			// 読み込むテクスチャの最大数
//#define MAX_OBJECT					(64)			// オブジェクトの最大数
//#define MAX_OBJECTPARENT			(16)			// パーツの最大数
//#define MAX_KEY						(16)			// キーの最大数
//#define MAX_MOTION					(16)			// モーションの最大数
//
////*****************************************************************************
//// モーションの種類
////*****************************************************************************
//typedef enum
//{
//	MOTIONTYPE_NEUTRAL = 0,		// 待機
//	MOTIONTYPE_MOVE,			// 移動
//	MOTIONTYPE_ACTION,			// アクション
//	MOTIONTYPE_JUMP,			// ジャンプ
//	MOTIONTYPE_LANDING,			// 着地
//	MOTIONTYPE_MAX
//}MOTIONTYPE;
//
////*****************************************************************************
//// キー要素の構造体
////*****************************************************************************
//typedef struct
//{
//	float fPosX;				// 位置X
//	float fPosY;				// 位置Y
//	float fPosZ;				// 位置Z
//	float fRotX;				// 向きX
//	float fRotY;				// 向きY
//	float fRotZ;				// 向きZ
//}KEY;
//
////*****************************************************************************
//// キー情報の構造体
////*****************************************************************************
//typedef struct
//{
//	int nFrame;					// 再生フレーム
//	KEY aKey[MAX_OBJECTPARENT];	// 各パーツのキー要素
//}KEY_INFO;
//
////*****************************************************************************
//// モーション情報の構造体
////*****************************************************************************
//typedef struct MOTION_INFO
//{
//	bool bLoop;
//	int nNumKey;
//	KEY_INFO aKeyInfo[MAX_KEY];
//}MOTION_INFO;
//
//typedef struct MOTION_INFO* PMOTIONINFO;
//
////*****************************************************************************
//// オブジェクト情報構造体定義
////*****************************************************************************
//typedef struct ObjectInfo
//{
//	LPDIRECT3DTEXTURE9 apTexture[MAX_OBJECTTEX];	// テクスチャへのポインタ
//	LPD3DXMESH pMesh;								// メッシュへのポインタ
//	LPD3DXBUFFER pBuffMat;							// マテリアルへのポインタ
//	DWORD dwNumMat;									// マテリアルの数
//	D3DXVECTOR3 vtxMin;								// 頂点位置の最小値
//	D3DXVECTOR3 vtxMax;								// 頂点位置の最大値
//	bool bUse;										// 使用状態
//}ObjectInfo;
//
//typedef struct ObjectInfo* POBJECTUNFO;
//
////*****************************************************************************
//// 階層構造オブジェクト構造体定義
////*****************************************************************************
//typedef struct ParentObjectInfo
//{
//	D3DXVECTOR3 posWorld;									// ワールド座標
//	D3DXVECTOR3 posLocal;									// ローカル座標
//	D3DXVECTOR3 rotWorld;									// ワールドベクトル
//	D3DXVECTOR3 rotLocal;									// ローカルベクトル
//	D3DXMATRIX mtxWorld;									// ワールドマトリックス
//	int nIdxObject;											// オブジェクト識別インデックス
//	int nIdxModel;											// モデル階層インデックス
//	int nIdxModelParent;									// 親モデルのインデックス
//	bool bUse;												// 使用状態
//}ParentObjectInfo;
//
//typedef struct ParentObjectInfo* PPARENTOBJINFO;
//
////*****************************************************************************
//// オブジェクト情報構造体定義
////*****************************************************************************
//typedef struct
//{
//	D3DXVECTOR3 pos;								// ワールド座標
//	D3DXVECTOR3 rot;								// ワールドベクトル
//	D3DXMATRIX mtxWorld;							// ワールドマトリックス
//	int nType;										// 種類
//	bool bUse;										// 使用状態
//}Object;
//
////*****************************************************************************
//// プロトタイプ宣言
////*****************************************************************************
//void InitObject(void);
//void UninitObject(void);
//void UpdateObject(void);
//void DrawObject(void);
//
//#endif