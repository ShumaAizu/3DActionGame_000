////=============================================================================
////
////	スクリプト読み込み処理 [loadsclipt.cpp]
////	Author : SHUMA AIZU
//// 
////=============================================================================
//
//#include "loadscript.h"
//
////*****************************************************************************
//// マクロ定義
////*****************************************************************************
//#define MAX_STRING			(256)			// 読み込める文字列の最大
//#define LOAD_START			"SCRIPT"		// 読み込み開始
//#define LOAD_PLAYER			"CHARACTERSET"	// プレイヤー情報読み込み開始
//#define LOAD_MOTION			"MOTIONSCRIPT"	// モーションスクリプト読み込み開始
//#define LOAD_MOTIONINFO		"MOTIONSET"		// モーション情報読み込み開始
//
////*****************************************************************************
//// マクロ定義
////*****************************************************************************
//int g_NumModel;								// 読み込んだモデル数
//int g_NumMotion;							// 読み込んだモーション数
//
////=============================================================================
////	スクリプトの読み込み処理
////=============================================================================
//HRESULT LoadScript(const char* pScriptFileName)
//{
//	FILE* pScriptFile = fopen(pScriptFileName, "r");
//
//	if (pScriptFile == NULL)
//	{// スクリプト読み込み失敗
//		return E_FAIL;
//	}
//
//	char aStr[MAX_STRING] = {};
//	char aScriptPath[FILENAME_MAX] = {};
//	char aTexPath[FILENAME_MAX] = {};
//	char* pStart = NULL;
//	g_NumModel = 0;
//	g_NumMotion = 0;
//
//	while (true)
//	{
//		memset(aStr, NULL, sizeof(aStr));			// 文字列初期化
//		fgets(aStr, sizeof(aStr), pScriptFile);		// 一行読み取り
//
//		if (strstr(aStr, LOAD_START) != NULL)
//		{// 読み込み開始
//			break;
//		}
//
//		if (feof(pScriptFile) != NULL)
//		{// 読み込み失敗
//			return E_FAIL;
//		}
//	}
//
//	while (true)
//	{
//		memset(aStr, NULL, sizeof(aStr));					// 文字列初期化
//		(void)fgets(aStr, sizeof(aStr), pScriptFile);		// 一行読み取り
//
//		if (strstr(aStr, LOAD_MOTION) != NULL)
//		{
//			pStart = strchr(aStr, '=');
//
//			(void)sscanf(pStart + 1, "%s", &aScriptPath);
//
//			LoadMotionInfo(aScriptPath);
//		}
//	}
//}
//
////=============================================================================
////	モーション情報読み込み処理
////=============================================================================
//HRESULT LoadMotionInfo(const char* pMotionFileName)
//{
//	FILE* pMotionFile = fopen(pMotionFileName, "r");
//
//	if (pMotionFile == NULL)
//	{// 読み込み失敗
//		return E_FAIL;
//	}
//
//	char aStr[MAX_STRING] = {};
//	char aModelPath[FILENAME_MAX] = {};
//
//	while (true)
//	{
//		memset(aStr, NULL, sizeof(aStr));
//		(void)fgets(aStr, sizeof(aStr), pMotionFile);
//
//		if (strstr(aStr, LOAD_START) != NULL)
//		{// 読み込み開始
//			break;
//		}
//
//		if (feof(pMotionFile) != NULL)
//		{// 読み込み失敗
//			return E_FAIL;
//		}
//	}
//
//	while (true)
//	{
//		memset(aStr, NULL, sizeof(aStr));
//		(void)fgets(aStr, sizeof(aStr), pMotionFile);
//
//
//	}
//}