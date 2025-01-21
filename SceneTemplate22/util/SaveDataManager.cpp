#include "SaveDataManager.h"
#include <DxLib.h>
#include <cassert>
#include "object/player/Player.h"

namespace
{
	const char* const kSaveDataFilename = "Data/SaveData.csv";
}

SaveDataManager::SaveDataManager()
{
}

SaveDataManager::~SaveDataManager()
{
}

SaveDataManager& SaveDataManager::GetInstance()
{
	// TODO: return ステートメントをここに挿入します
	static SaveDataManager instance;
	return instance;
}

//セーブデータの読み込み
void SaveDataManager::Load()
{
	// ファイルを開く
	int fileHandle = FileRead_open(kSaveDataFilename);
	if (fileHandle == 0)
	{
		assert(false && "読み込み失敗");
		Init();
		return;
	}

	// クリアタイムを読み込む
	FileRead_read(&m_info.clearTime, sizeof(int), fileHandle);
	// 解放情報を読み込む
	for (int i = 0; i < static_cast<int>(e_PlayerKind::kMax); i++)
	{
		e_PlayerKind kind = static_cast<e_PlayerKind>(i);
		bool isRelease;
		FileRead_read(&isRelease, sizeof(bool), fileHandle);
		m_info.isRelease[kind] = isRelease;
	}

	// ファイルを閉じる
	FileRead_close(fileHandle);
}

//セーブデータの保存
void SaveDataManager::Save() const
{
	// ファイルを開く
	FILE* fp;
	auto err = fopen_s(&fp, kSaveDataFilename, "wb");
	if (err != 0)
	{
		assert(false && "書き込み失敗");
		return;
	}	

	// クリアタイムを書き込む
	fwrite(&m_info.clearTime, sizeof(int), 1, fp);
	// 解放情報を書き込む
	for (int i = 0; i < static_cast<int>(e_PlayerKind::kMax); i++)
	{
		e_PlayerKind kind = static_cast<e_PlayerKind>(i);
		bool isRelease = m_info.isRelease.at(kind);
		fwrite(&isRelease, sizeof(bool), 1, fp);
	}

	// ファイルを閉じる
	fclose(fp);
}

//セーブデータの初期化
void SaveDataManager::Init()
{
	// クリアタイムを初期化
	m_info.clearTime = 0;
	// 解放情報を初期化
	for (int i = 0; i < static_cast<int>(e_PlayerKind::kMax); i++)
	{
		e_PlayerKind kind = static_cast<e_PlayerKind>(i);
		m_info.isRelease[kind] = false;
	}
}
