#pragma once
// シングルトンを使用したコンフィグデータ管理クラス
class ConfigData
{
public:
	// 設定
	enum Flag
	{
		WindowMode = 0,	// フルスクリーン設定
		Vibration = 1,	// 振動ONOFF
	};
	
private:
	ConfigData() = default;
	virtual ~ConfigData() = default;

	static ConfigData* m_instance;

public:
	// コピーコンストラクタの禁止
	ConfigData(const ConfigData&) = delete;
	ConfigData& operator=(const ConfigData&) = delete;
	ConfigData(ConfigData&&) = delete;
	ConfigData& operator=(ConfigData&&) = delete;

	static ConfigData& getInstance()
	{
		if (!m_instance)
		{
			m_instance = new ConfigData;
		}
		return *m_instance;
	}
	static void destroy()
	{
		delete m_instance;
		m_instance = nullptr;
	}
public:
	// データの読み込み
	void load();
	// セーブデータの書き込み
	void write();

	// コンフィグ
	int getLanguage() const;
	void setLanguage(int lang);

	int getBgmVol() const;	// 0~getMaxBgmVol()
	int getSeVol() const;	// 0~getMaxSeVol()
	int getMaxBgmVol() const;	// 音量最大値
	int getMaxSeVol() const;	// 

	void setBgmVol(int vol);
	void setSeVol(int vol);

	float getBgmVolRate() const;	// 0.0~1.0
	float getSeVolRate() const;	// 0.0~1.0

	// 音量変更	変更後セーブデータ書き込む必要あり
	void incBgmVol(bool isLoop);	// 音量アップ	isLoop = trueの場合最大値を超えると0に
	void decBgmVol(bool isLoop);	// 音量ダウン	isLoop = trueの場合最大値を超えると0に
	void incSeVol(bool isLoop);
	void decSeVol(bool isLoop);

	// 各項目の設定変更/取得
	bool isWindowMode() const { return isOn(Flag::WindowMode); }
	void setWindowMode(bool isWindowMode) { setFlag(Flag::WindowMode, isWindowMode); }

	bool isVibration() const { return isOn(Flag::Vibration); }
	void setVibration(bool isVibration)  { setFlag(Flag::Vibration, isVibration); }


private:
	// プレイデータの設定	書き込みは行わないので設定が終わったらwrite()する
	void flagOn(Flag flag);
	void flagOff(Flag flag);

	bool isOn(Flag flag) const;

	// セーブデータを新規作成して上書き(新規保存)
	void createNewData();

#ifdef _DEBUG
	// 全てのフラグをONにする
	void setAllFlag();
#endif
	// データのクリア
	void clearData();
	// フラグON,OFF
	// isOn:　trueでフラグを立てる、falseで落とす
	void setFlag(Flag flag, bool isOn);

private:

	// 保存するセーブデータ本体
	struct DataCore
	{
		int version = 0;		// データのバージョン

		// コンフィグ
		char language = 0;
		char volBgm = 3;		// BGM音量	0~5
		char volSe = 3;		// SE音量	0~5
		unsigned char reserved0 = 0;		// 予備領域

		unsigned int flag = 0;
		unsigned int reserved1 = 0;	// 予備領域
	};

private:
	DataCore m_data;
};

