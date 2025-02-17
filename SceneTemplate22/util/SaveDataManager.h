#pragma once
#include <unordered_map>
#include "util/Game.h"

// プレイヤーの種類を表す列挙型
namespace Game
{
	enum class e_PlayerKind;
}

// セーブデータを管理するクラス
class SaveDataManager
{
private:
	// セーブデータの情報を格納する構造体
	struct Info
	{
		int clearTime; // クリア時間
		std::unordered_map<Game::e_PlayerKind, bool> isRelease; // プレイヤーの種類ごとの解放状態

	};

private:
	// コンストラクタ
	SaveDataManager();
	// デストラクタ
	~SaveDataManager();

	// コピーコンストラクタを削除
	SaveDataManager(const SaveDataManager&) = delete;
	// ムーブコンストラクタを削除
	SaveDataManager(const SaveDataManager&&) = delete;
	// コピー代入演算子を削除
	SaveDataManager operator= (const SaveDataManager&) = delete;
	// ムーブ代入演算子を削除
	SaveDataManager operator= (const SaveDataManager&&) = delete;

public:
	// インスタンスを取得する関数
	static SaveDataManager& GetInstance();

	// セーブデータを読み込む関数
	void Load();
	// セーブデータを保存する関数
	void Save() const;

	// セーブデータを初期化する関数
	void Init();

	// クリア時間を更新する関数
	void UpdateClearTime() { ++m_info.clearTime; }
	// プレイヤーの種類を解放する関数
	void OnRelease(Game::e_PlayerKind kind) { m_info.isRelease[kind] = true; }

	// クリア時間を取得する関数
	int GetClearTime() const { return m_info.clearTime; }
	// プレイヤーの種類が解放されているかを確認する関数
	bool IsRelease(Game::e_PlayerKind kind) const { return m_info.isRelease.at(kind); }

private:
	// セーブデータの情報
	Info m_info;
};