#pragma once
#include <vector>
#include <string>
#include <map>

// シングルトンを使用したフォント管理クラス
class FontManager
{
private:
	FontManager() = default;
	virtual ~FontManager() = default;

	static FontManager* m_instance;
public:
	// コピーコンストラクタの禁止
	FontManager(const FontManager&) = delete;
	FontManager& operator=(const FontManager&) = delete;
	FontManager(FontManager&&) = delete;
	FontManager& operator=(FontManager&&) = delete;

	static FontManager& getInstance()
	{
		if (!m_instance)
		{
			m_instance = new FontManager;
		}
		return *m_instance;
	}
	static void destroy()
	{
		delete m_instance;
		m_instance = nullptr;
	}

public:

	// csv読み込み、フォントのロ度を行う
	void init();
	// initで生成したm_fontDataTblに登録されているフォントの生成
	void createFont();	
	// フォントの生成終了判定
	bool isCreateEnd();

	// 終了
	void end();

	// 指定したフォントIDのフォントハンドル取得
	int getHandle(std::string id) const;


//#ifdef _DEBUG
	// デバッグ用関数

	// フォントの再読み込み
	
	
	// 登録されているフォントの数を取得
	int getFontNum() const;
	// 登録番号のフォントIDを取得
	std::string getKey(int index) const;
	// 登録番号のフォントハンドルを取得
	int getHandle(int index) const;

//#endif

private:
	// 初期化関連処理
	void loadCsv();			// csvファイルを読み込んで必要な情報に変換
	void loadFontData();	// m_fileNameに登録されたフォントデータを読み込み

	// 読み込む必要のあるフォントのファイル名を追加する(すでに登録されている場合は追加しない)
	void addLoadFontFilename(std::string filename);

private:
	// フォント生成用データ構造体
	struct FontData
	{
		std::string name;
		int size;
		int thick;
		int type;
	};

private:

	// 読み込む必要のあるフォントファイル名
	std::vector<std::string>	m_fileName;

	// csvから展開した生成する必要のあるフォントの情報
	std::vector<FontData> m_fontDataTbl;

	// 生成したフォントハンドル
	std::vector<int>	m_handle;

	// フォントIDとフォント番号のデータ
	std::map<std::string, int>	m_idData;

};

