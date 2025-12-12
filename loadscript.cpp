//=============================================================================
//
//	スクリプト読み込み処理 [loadsclipt.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "loadscript.h"
#include "object.h"
#include "player.h"
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
#define LOAD_INDEX			"INDEX"				// インデックス読み込み
#define LOAD_PARENT			"PARENT"			// 親インデックス読み込み

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
	FILE* pScriptFile = fopen(pScriptFileName, "r");

	if (pScriptFile == NULL)
	{// スクリプト読み込み失敗
		return E_FAIL;
	}

	char aStr[MAX_STRING] = {};
	char aScriptPath[FILENAME_MAX] = {};
	char aTexPath[FILENAME_MAX] = {};
	char* pStart = NULL;
	g_NumModel = 0;
	g_NumMotion = 0;

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

			(void)sscanf(pStart + 1, "%s", &aScriptPath);

			LoadMotionInfo(aScriptPath);
		}

		if (strstr(aStr, LOAD_END))
		{
			fclose(pScriptFile);

			break;
		}
	}
}

//=============================================================================
//	モーション情報読み込み処理
//=============================================================================
HRESULT LoadMotionInfo(const char* pMotionFileName)
{
	FILE* pMotionFile = fopen(pMotionFileName, "r");

	if (pMotionFile == NULL)
	{// 読み込み失敗
		return E_FAIL;
	}

	char aStr[MAX_STRING] = {};
	char aStrCpy[MAX_STRING] = {};
	char* pStart = NULL;
	char aModelPath[FILENAME_MAX] = {};
	int nIdx = 0;
	int nParent = 0;
	D3DXVECTOR3 pos = {};
	D3DXVECTOR3 rot = {};
	int nNumModel = 0;
	int nCntMotion = 0;
	int bLoop = false;
	int nNumKey = 0;
	KEY_INFO KeyInfo[MAX_KEY] = {};
	int nCntKey = 0;
	int nCntParts = 0;

	while (true)
	{
		memset(aStr, NULL, sizeof(aStr));
		(void)fgets(aStr, sizeof(aStr), pMotionFile);

		if (strstr(aStr, LOAD_START) != NULL)
		{// 読み込み開始
			break;
		}

		if (feof(pMotionFile) != NULL)
		{// 読み込み失敗
			return E_FAIL;
		}
	}

	while (true)
	{
		memset(aStr, NULL, sizeof(aStr));
		memset(aStrCpy, NULL, sizeof(aStrCpy));
		(void)fgets(aStr, sizeof(aStr), pMotionFile);
		LoadEnableString(&aStrCpy[0], &aStr[0]);

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

			LoadPartsPlayer(aModelPath);
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
							LoadCharacterPlayer(pos, rot, nIdx, nParent);

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
					LoadMotion(bLoop, nNumKey, &KeyInfo[0], nCntMotion);

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