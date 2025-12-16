//=============================================================================
//
//	スクリプト読み込み処理 [loadsclipt.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "loadscript.h"
#include "object.h"
#include "player.h"
#include "friends.h"
#include "model.h"
#include "offsetmodel.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_STRING			(256)				// 読み込める文字列の最大
#define LOAD_START			"SCRIPT"			// 読み込み
#define LOAD_END			"END_SCRIPT"		// 終了
#define LOAD_PLAYER			"CHARACTERSET"		// プレイヤー情報読み込み
#define LOAD_ENDPLAYER		"END_CHARACTERSET"	// プレイヤー情報読み込み終了
#define LOAD_PARTS			"PARTSSET"			// パーツ情報読み込み
#define LOAD_ENDPARTS		"END_PARTSSET"		// パーツ情報読み込み終了
#define LOAD_MOTION			"MOTIONSCRIPT"		// モーションスクリプト読み込み
#define LOAD_MOTIONINFO		"MOTIONSET"			// モーション情報読み込み
#define LOAD_ENDMOTIONINFO	"END_MOTIONSET"		// モーション情報読み込み終了
#define LOAD_NUMMODEL		"NUM_MODEL"			// モデル数読み込み
#define LOAD_MODEL			"MODEL_FILENAME"	// モデル情報読み込み
#define LOAD_MOTIONLOOP		"LOOP"				// ループモーションか読み込み
#define LOAD_NUMKEY			"NUM_KEY"			// キー数読み込み
#define LOAD_KEYSET			"KEYSET"			// キー情報読み込み
#define LOAD_ENDKEYSET		"END_KEYSET"		// キー情報読み込み終了
#define LOAD_FRAME			"FRAME"				// フレーム数読み込み
#define LOAD_KEY			"KEY"				// キー読み込み
#define LOAD_ENDKEY			"END_KEY"			// キー読み込み終了
#define LOAD_POS			"POS"				// 位置読み込み
#define LOAD_ROT			"ROT"				// 向き読み込み
#define LOAD_TYPE			"TYPE"				// 種類読み込み
#define LOAD_EVENT			"EVENT"				// イベント読み込み
#define LOAD_RADIUS			"RADIUS"			// 半径読み込み
#define LOAD_INDEX			"INDEX"				// インデックス読み込み
#define LOAD_PARENT			"PARENT"			// 親インデックス読み込み
#define LOAD_MODELINFO		"MODELSET"			// モデル情報読み込み
#define LOAD_ENDMODELINFO	"END_MODELSET"		// モデル情報読み込み終了
#define LOAD_ITEM			"ITEMSET"			// アイテム情報読み込み
#define LOAD_ENDITEM		"END_ITEMSET"		// アイテム情報読み込み終了
#define LOAD_FRIENDS		"FRIENDSET"			// 仲間の情報読み込み
#define LOAD_ENDFRIENDS		"END_FRIENDSET"		// 仲間の情報読み込み終了

//*****************************************************************************
// マクロ定義
//*****************************************************************************
int g_NumModel;								// 読み込んだモデル数
int g_NumMotion;							// 読み込んだモーション数

//=============================================================================
//	スクリプトの読み込み処理
//=============================================================================
HRESULT LoadScript(const char* pScriptFileName)
{
	// 受け取ったパスからファイルを開く
	FILE* pScriptFile = fopen(pScriptFileName, "r");

	if (pScriptFile == NULL)
	{// スクリプト読み込み失敗
		return E_FAIL;
	}

	char aStr[MAX_STRING] = {};				// 文字列格納
	char aScriptPath[FILENAME_MAX] = {};	// スクリプトファイルパス
	char aTexPath[FILENAME_MAX] = {};		// テクスチャファイルパス
	char* pStart = NULL;					// 文字列の開始位置
	int type = NULL;						// 種類
	g_NumModel = 0;							// モデル数初期化
	g_NumMotion = 0;						// モーション数初期化

	while (true)
	{
		memset(aStr, NULL, sizeof(aStr));			// 文字列初期化
		fgets(aStr, sizeof(aStr), pScriptFile);		// 一行読み取り

		if (strstr(aStr, LOAD_START) != NULL)
		{// 読み込み開始
			break;
		}

		if (feof(pScriptFile) != NULL)
		{// 読み込み失敗
			return E_FAIL;
		}
	}

	while (true)
	{
		memset(aStr, NULL, sizeof(aStr));					// 文字列初期化
		(void)fgets(aStr, sizeof(aStr), pScriptFile);		// 一行読み取り

		if (strstr(aStr, LOAD_MOTION) != NULL)
		{
			pStart = strchr(aStr, '=');

			(void)sscanf(pStart + 1, "%s %d", &aScriptPath, &type);

			LoadMotionInfo(aScriptPath, (OBJECTTYPE)type);
		}

		if (strstr(aStr, LOAD_END))
		{
			fclose(pScriptFile);

			break;
		}
	}

	return S_OK;
}

//=============================================================================
//	モーション情報読み込み処理
//=============================================================================
HRESULT LoadMotionInfo(const char* pMotionFileName, OBJECTTYPE type)
{
	FILE* pMotionFile = fopen(pMotionFileName, "r");

	if (pMotionFile == NULL)
	{// 読み込み失敗
		return E_FAIL;
	}

	char aStr[MAX_STRING] = {};			   // 文字列読み込み
	char aStrCpy[MAX_STRING] = {};		   // 文字列複製(整理)
	char* pStart = NULL;				   // 文字列開始位置
	char aModelPath[FILENAME_MAX] = {};	   // モデルのファイル名読み込み
	int nIdx = 0;						   // モデルのインデックス読み込み
	int nParent = 0;					   // モデルの親インデックス読み込み
	D3DXVECTOR3 pos = {};				   // 位置読み込み
	D3DXVECTOR3 rot = {};				   // 向き読み込み
	int nNumModel = 0;					   // モデル数読み込み
	int nCntMotion = 0;					   // モーション数カウント
	int bLoop = false;					   // ループするかどうか読み込み
	int nNumKey = 0;					   // キー数読み込み
	KEY_INFO KeyInfo[MAX_KEY] = {};		   // キー情報読み込み
	int nCntKey = 0;					   // キー数カウント
	int nCntParts = 0;					   // パーツ数カウント

	while (true)
	{
		memset(aStr, NULL, sizeof(aStr));				// 文字列クリア
		(void)fgets(aStr, sizeof(aStr), pMotionFile);	// 一列読み込み

		if (strstr(aStr, LOAD_START) != NULL)
		{// LOAD_STARTを読み込めば読み込み開始
			break;
		}

		if (feof(pMotionFile) != NULL)
		{// 読み込み失敗
			return E_FAIL;
		}
	}

	while (true)
	{
		memset(aStr, NULL, sizeof(aStr));				// 文字列クリア
		memset(aStrCpy, NULL, sizeof(aStrCpy));			// コピーもクリア
		(void)fgets(aStr, sizeof(aStr), pMotionFile);	// 一列読み込み
		LoadEnableString(&aStrCpy[0], &aStr[0]);		// 有効文字だけ抜き取って複製

		if (strstr(aStr, LOAD_NUMMODEL))
		{
			if ((pStart = strchr(aStr, '=')) == NULL)
			{
				continue;
			}

			(void)sscanf(pStart + 1, "%d", &nNumModel);
		}

		if (strstr(aStr, LOAD_MODEL))
		{
			if ((pStart = strchr(aStr, '=')) == NULL)
			{
				continue;
			}

			(void)sscanf(pStart + 1, "%s", &aModelPath);

			switch (type)
			{
			case OBJECTTYPE_PLAYER:
				LoadPartsPlayer(aModelPath);
				break;

			case OBJECTTYPE_FRIENDS:
				LoadPartsFriends(aModelPath);
				break;
			}
		}

		if (strstr(aStr, LOAD_PLAYER))
		{
			while (true)
			{
				memset(aStr, NULL, sizeof(aStr));
				memset(aStrCpy, NULL, sizeof(aStrCpy));
				(void)fgets(aStr, sizeof(aStr), pMotionFile);
				LoadEnableString(&aStrCpy[0], &aStr[0]);

				if (strcmp(aStrCpy, LOAD_PARTS) == 0)
				{
					while (true)
					{
						memset(aStr, NULL, sizeof(aStr));
						memset(aStrCpy, NULL, sizeof(aStrCpy));
						(void)fgets(aStr, sizeof(aStr), pMotionFile);
						LoadEnableString(&aStrCpy[0], &aStr[0]);

						if (strstr(aStr, LOAD_INDEX))
						{
							if ((pStart = strchr(aStr, '=')) == NULL)
							{
								continue;
							}

							(void)sscanf(pStart + 1, "%d", &nIdx);

							continue;
						}

						if (strstr(aStr, LOAD_PARENT))
						{
							if ((pStart = strchr(aStr, '=')) == NULL)
							{
								continue;
							}

							(void)sscanf(pStart + 1, "%d", &nParent);

							continue;
						}

						if (strstr(aStr, LOAD_POS))
						{
							if ((pStart = strchr(aStr, '=')) == NULL)
							{
								continue;
							}

							(void)sscanf(pStart + 1, "%f %f %f", &pos.x, &pos.y, &pos.z);

							continue;
						}

						if (strstr(aStr, LOAD_ROT))
						{
							if ((pStart = strchr(aStr, '=')) == NULL)
							{
								continue;
							}

							(void)sscanf(pStart + 1, "%f %f %f", &rot.x, &rot.y, &rot.z);

							continue;
						}

						if (strcmp(aStrCpy, LOAD_ENDPARTS) == 0)
						{
							switch (type)
							{
							case OBJECTTYPE_PLAYER:
								LoadCharacterPlayer(pos, rot, nIdx, nParent);
								break;

							case OBJECTTYPE_FRIENDS:
								LoadCharacterFriends(pos, rot, nIdx, nParent);
								break;
							}

							memset(&pos, NULL, sizeof(D3DXVECTOR3));
							memset(&rot, NULL, sizeof(D3DXVECTOR3));
							int nIdx = 0;
							int nParet = 0;
							break;
						}
					}
				}

				if (strcmp(aStrCpy, LOAD_ENDPLAYER) == 0)
				{
					break;
				}

			}
		}

		if (strcmp(aStrCpy, LOAD_MOTIONINFO) == 0)
		{
			while (true)
			{
				memset(aStr, NULL, sizeof(aStr));
				memset(aStrCpy, NULL, sizeof(aStrCpy));
				(void)fgets(aStr, sizeof(aStr), pMotionFile);
				LoadEnableString(&aStrCpy[0], &aStr[0]);

				if (strstr(aStr, LOAD_MOTIONLOOP))
				{
					if ((pStart = strchr(aStr, '=')) == NULL)
					{
						continue;
					}

					(void)sscanf(pStart + 1, "%d", &bLoop);

					continue;
				}

				if (strstr(aStr, LOAD_NUMKEY))
				{
					if ((pStart = strchr(aStr, '=')) == NULL)
					{
						continue;
					}

					(void)sscanf(pStart + 1, "%d", &nNumKey);

					continue;
				}

				if (strcmp(aStrCpy, LOAD_KEYSET) == 0)
				{
					while (true)
					{
						memset(aStr, NULL, sizeof(aStr));
						memset(aStrCpy, NULL, sizeof(aStrCpy));
						(void)fgets(aStr, sizeof(aStr), pMotionFile);
						LoadEnableString(&aStrCpy[0], &aStr[0]);

						if (strstr(aStr, LOAD_FRAME))
						{
							if ((pStart = strchr(aStr, '=')) == NULL)
							{
								continue;
							}

							(void)sscanf(pStart + 1, "%d", &KeyInfo[nCntKey].nFrame);

							continue;
						}

						if (strcmp(aStrCpy, LOAD_KEY) == 0)
						{
							while (true)
							{
								memset(aStr, NULL, sizeof(aStr));
								memset(aStrCpy, NULL, sizeof(aStrCpy));
								(void)fgets(aStr, sizeof(aStr), pMotionFile);
								LoadEnableString(&aStrCpy[0], &aStr[0]);

								if (strstr(aStr, LOAD_POS))
								{
									if ((pStart = strchr(aStr, '=')) == NULL)
									{
										continue;
									}

									(void)sscanf(pStart + 1, "%f %f %f", &KeyInfo[nCntKey].aKey[nCntParts].fPosX, &KeyInfo[nCntKey].aKey[nCntParts].fPosY, &KeyInfo[nCntKey].aKey[nCntParts].fPosZ);

									continue;
								}

								if (strstr(aStr, LOAD_ROT))
								{
									if ((pStart = strchr(aStr, '=')) == NULL)
									{
										continue;
									}

									(void)sscanf(pStart + 1, "%f %f %f", &KeyInfo[nCntKey].aKey[nCntParts].fRotX, &KeyInfo[nCntKey].aKey[nCntParts].fRotY, &KeyInfo[nCntKey].aKey[nCntParts].fRotZ);

									continue;
								}

								if (strcmp(aStrCpy, LOAD_ENDKEY) == 0)
								{
									nCntParts++;
									break;
								}
							}
						}

						if (strcmp(aStrCpy, LOAD_ENDKEYSET) == 0)
						{
							nCntKey++;
							nCntParts = 0;
							break;
						}
					}
				}

				if (strcmp(aStrCpy, LOAD_ENDMOTIONINFO) == 0)
				{
					switch (type)
					{
					case OBJECTTYPE_PLAYER:
						LoadMotion(bLoop, nNumKey, &KeyInfo[0], nCntMotion);
						break;

					case OBJECTTYPE_FRIENDS:
						LoadMotionFriends(bLoop, nNumKey, &KeyInfo[0], nCntMotion);
						break;
					}

					nCntMotion++;

					memset(&KeyInfo, NULL, sizeof(KEY_INFO));
					bLoop = false;
					nNumKey = 0;

					nCntKey = 0;

					break;
				}

			}
		}

		if (strcmp(aStrCpy, LOAD_END) == 0)
		{
			fclose(pMotionFile);

			break;
		}
	}

	return S_OK;
}

//=============================================================================
//	ステージ情報読み込み処理
//=============================================================================
HRESULT LoadStage(const char* pStageFileName)
{
	FILE* pStageFile = fopen(pStageFileName, "r");

	if (pStageFile == NULL)
	{// 読み込み失敗
		return E_FAIL;
	}

	char aStr[MAX_STRING] = {};			   // 文字列読み込み
	char aStrCpy[MAX_STRING] = {};		   // 文字列複製(整理)
	char* pStart = NULL;				   // 文字列開始位置
	char aModelPath[FILENAME_MAX] = {};	   // モデルのファイル名読み込み
	int nIdx = 0;						   // モデルのインデックス読み込み
	int nParent = 0;					   // モデルの親インデックス読み込み
	D3DXVECTOR3 pos = {};				   // 位置読み込み
	D3DXVECTOR3 rot = {};				   // 向き読み込み
	int nNumModel = 0;					   // モデル数読み込み

	while (true)
	{
		memset(aStr, NULL, sizeof(aStr));				// 文字列クリア
		(void)fgets(aStr, sizeof(aStr), pStageFile);	// 一列読み込み

		if (strstr(aStr, LOAD_START) != NULL)
		{// LOAD_STARTを読み込めば読み込み開始
			break;
		}

		if (feof(pStageFile) != NULL)
		{// 読み込み失敗
			return E_FAIL;
		}
	}

	while (true)
	{
		memset(aStr, NULL, sizeof(aStr));				// 文字列クリア
		memset(aStrCpy, NULL, sizeof(aStrCpy));			// コピーもクリア
		(void)fgets(aStr, sizeof(aStr), pStageFile);	// 一列読み込み
		LoadEnableString(&aStrCpy[0], &aStr[0]);		// 有効文字だけ抜き取って複製

		if (strstr(aStr, LOAD_NUMMODEL))
		{
			if ((pStart = strchr(aStr, '=')) == NULL)
			{
				continue;
			}

			(void)sscanf(pStart + 1, "%d", &nNumModel);
		}

		if (strstr(aStr, LOAD_MODEL))
		{
			if ((pStart = strchr(aStr, '=')) == NULL)
			{
				continue;
			}

			(void)sscanf(pStart + 1, "%s", &aModelPath);

			LoadModelData(aModelPath);
		}

		if (strcmp(aStrCpy, LOAD_END) == 0)
		{
			fclose(pStageFile);

			break;
		}
	}

	return S_OK;
}

//=============================================================================
//	オブジェクト情報読み込み処理
//=============================================================================
HRESULT LoadObject(const char* pObjectFileName)
{
	FILE* pObjectFile = fopen(pObjectFileName, "r");

	if (pObjectFile == NULL)
	{// 読み込み失敗
		return E_FAIL;
	}

	char aStr[MAX_STRING] = {};			   // 文字列読み込み
	char aStrCpy[MAX_STRING] = {};		   // 文字列複製(整理)
	char* pStart = NULL;				   // 文字列開始位置
	char aScriptPath[FILENAME_MAX] = {};   // スクリプトファイルパス
	char aModelPath[FILENAME_MAX] = {};	   // モデルのファイル名読み込み
	int nIdx = 0;						   // モデルのインデックス読み込み
	int nParent = 0;					   // モデルの親インデックス読み込み
	D3DXVECTOR3 pos = {};				   // 位置読み込み
	D3DXVECTOR3 rot = {};				   // 向き読み込み
	int type = NULL;					   // 種類
	int event = NULL;					   // イベント
	int Objecttype = NULL;				   // オブジェクトの種類
	int nNumModel = 0;					   // モデル数読み込み

	while (true)
	{
		memset(aStr, NULL, sizeof(aStr));				// 文字列クリア
		(void)fgets(aStr, sizeof(aStr), pObjectFile);	// 一列読み込み

		if (strstr(aStr, LOAD_START) != NULL)
		{// LOAD_STARTを読み込めば読み込み開始
			break;
		}

		if (feof(pObjectFile) != NULL)
		{// 読み込み失敗
			return E_FAIL;
		}
	}

	while (true)
	{
		memset(aStr, NULL, sizeof(aStr));				// 文字列クリア
		memset(aStrCpy, NULL, sizeof(aStrCpy));			// コピーもクリア
		(void)fgets(aStr, sizeof(aStr), pObjectFile);	// 一列読み込み
		LoadEnableString(&aStrCpy[0], &aStr[0]);		// 有効文字だけ抜き取って複製

		if (strstr(aStr, LOAD_MODEL))
		{
			if ((pStart = strchr(aStr, '=')) == NULL)
			{
				continue;
			}

			(void)sscanf(pStart + 1, "%s", &aModelPath);

			LoadModelData(aModelPath);
		}

		if (strstr(aStr, LOAD_MOTION) != NULL)
		{
			pStart = strchr(aStr, '=');

			(void)sscanf(pStart + 1, "%s %d", &aScriptPath, &Objecttype);

			LoadMotionInfo(aScriptPath, (OBJECTTYPE)Objecttype);

			memset(aScriptPath, NULL, sizeof(aScriptPath));

			NumFriendsAdd();
		}

		if (strcmp(aStrCpy, LOAD_FRIENDS) == 0)
		{
			while (true)
			{
				memset(aStr, NULL, sizeof(aStr));				// 文字列クリア
				memset(aStrCpy, NULL, sizeof(aStrCpy));			// コピーもクリア
				(void)fgets(aStr, sizeof(aStr), pObjectFile);	// 一列読み込み
				LoadEnableString(&aStrCpy[0], &aStr[0]);		// 有効文字だけ抜き取って複製

				if (strstr(aStr, LOAD_POS))
				{
					if ((pStart = strchr(aStr, '=')) == NULL)
					{
						continue;
					}

					(void)sscanf(pStart + 1, "%f %f %f", &pos.x, &pos.y, &pos.z);

					continue;
				}

				if (strstr(aStr, LOAD_ROT))
				{
					if ((pStart = strchr(aStr, '=')) == NULL)
					{
						continue;
					}

					(void)sscanf(pStart + 1, "%f %f %f", &rot.x, &rot.y, &rot.z);

					continue;
				}

				if (strstr(aStr, LOAD_TYPE))
				{
					if ((pStart = strchr(aStr, '=')) == NULL)
					{
						continue;
					}

					(void)sscanf(pStart + 1, "%d", &type);

					continue;
				}

				if (strstr(aStr, LOAD_EVENT))
				{
					if ((pStart = strchr(aStr, '=')) == NULL)
					{
						continue;
					}

					(void)sscanf(pStart + 1, "%d", &event);

					continue;
				}

				if (strcmp(aStrCpy, LOAD_ENDFRIENDS) == 0)
				{
					SetFriends(pos, rot, (FRIENDSTYPE)type, (FRIENDEVENT)event);
					break;
				}
			}
		}

		if (strcmp(aStrCpy, LOAD_END) == 0)
		{
			fclose(pObjectFile);

			break;
		}
	}

	return S_OK;
}

//=============================================================================
//	有効文字抜き出し処理
//=============================================================================
void LoadEnableString(char* aStrCpy, char* pStart)
{
	while (true)
	{
		if (*pStart == '\n')
		{
			pStart++;
			continue;
		}
		else if (*pStart == '\t')
		{
			pStart++;
			continue;
		}
		else if (*pStart == '#' || *pStart == '\0')
		{
			break;
		}
		else
		{
			strncat(aStrCpy, pStart, 1);
			pStart++;
		}
	}
}