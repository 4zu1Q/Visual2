#include "ConfigData.h"
//#include "DxLib.h"	// DxLib_Init()より前に使用するのでDxLib使用できない
#include "game.h"

#include <cassert>
#include <fstream>
ConfigData* ConfigData::m_instance = nullptr;

namespace
{
	// セーブデータのバージョン
	constexpr int kCurrentSaveVersion = 0;
	// セーブデータファイル名
	const char* const kSaveDataFilename = "save/config.dat";

	// 音量の段階	音量がkVolMaxValueの場合100%再生
	constexpr int kVolMaxValue = 5;
	constexpr int kVolDefaultValue = 3;
}

void ConfigData::load()
{
	// データのクリア
	clearData();

	std::ifstream ifs;
	ifs.open(kSaveDataFilename, std::ios_base::binary);
	if (ifs.fail())
	{
		// ファイル読み込み失敗 セーブデータを作る
	//	printfDx("失敗\n");
		createNewData();
		return;
	}
	else
	{
		DataCore data;
		ifs.read((char*)&data, sizeof(DataCore));
	//	printfDx("version %d\n", data.version);
		ifs.close();

		if (data.version == kCurrentSaveVersion)
		{
			// 読み込んだデータを実際に使用するデータにコピー
			m_data = data;
		}
		else
		{
			// リリース後にセーブデータのバージョンが変わったら実装しよう
			assert(false);
		}
	}
}

void ConfigData::write()
{
	assert(m_data.version == kCurrentSaveVersion);
	std::ofstream ofs(kSaveDataFilename, std::ios_base::binary);
	ofs.write((char*)&m_data, sizeof(DataCore));
}


int ConfigData::getLanguage() const
{
	return m_data.language;
}

void ConfigData::setLanguage(int lang)
{
	m_data.language = lang;
}

int ConfigData::getBgmVol() const
{
	return m_data.volBgm;
}

int ConfigData::getSeVol() const
{
	return m_data.volSe;
}

int ConfigData::getMaxBgmVol() const
{
	return kVolMaxValue;
}

int ConfigData::getMaxSeVol() const
{
	return kVolMaxValue;
}

void ConfigData::setBgmVol(int vol)
{
	assert(vol >= 0);
	assert(vol <= getMaxBgmVol());
	m_data.volBgm = vol;
}

void ConfigData::setSeVol(int vol)
{
	assert(vol >= 0);
	assert(vol <= getMaxSeVol());
	m_data.volSe = vol;
}

float ConfigData::getBgmVolRate() const
{
	return static_cast<float>(getBgmVol()) / static_cast<float>(getMaxBgmVol());
}

float ConfigData::getSeVolRate() const
{
	return static_cast<float>(getSeVol()) / static_cast<float>(getMaxSeVol());
}

void ConfigData::incBgmVol(bool isLoop)
{
	m_data.volBgm++;
	if (m_data.volBgm > kVolMaxValue)
	{
		if(isLoop)		m_data.volBgm = 0;
		else			m_data.volBgm = kVolMaxValue;
	}
}

void ConfigData::decBgmVol(bool isLoop)
{
	m_data.volBgm--;
	if (m_data.volBgm < 0)
	{
		if (isLoop)		m_data.volBgm = kVolMaxValue;
		else			m_data.volBgm = 0;
	}
}

void ConfigData::incSeVol(bool isLoop)
{
	m_data.volSe++;
	if (m_data.volSe > kVolMaxValue)
	{
		if (isLoop)		m_data.volSe = 0;
		else			m_data.volSe = kVolMaxValue;
	}
}

void ConfigData::decSeVol(bool isLoop)
{
	m_data.volSe--;
	if (m_data.volSe < 0)
	{
		if (isLoop)		m_data.volSe = kVolMaxValue;
		else			m_data.volSe = 0;
	}
}

void ConfigData::flagOn(Flag flag)
{
	setFlag(flag, true);
}

void ConfigData::flagOff(Flag flag)
{
	setFlag(flag, false);
}

bool ConfigData::isOn(Flag flag) const
{
	assert(flag < 32);
	return (m_data.flag & (1 << flag));
}

void ConfigData::createNewData()
{
	clearData();

	std::ofstream ofs(kSaveDataFilename, std::ios_base::binary);
	ofs.write((char*)&m_data, sizeof(DataCore));
}

#ifdef _DEBUG
void ConfigData::setAllFlag()
{
	m_data.flag = 0xffffffff;

	std::ofstream ofs(kSaveDataFilename, std::ios_base::binary);
	ofs.write((char*)&m_data, sizeof(DataCore));
}
#endif

void ConfigData::clearData()
{
	m_data.version = kCurrentSaveVersion;
	m_data.language = 0;
	m_data.volBgm = kVolDefaultValue;		// BGM音量
	m_data.volSe = kVolDefaultValue;		// SE音量
	m_data.reserved0 = 0;

	m_data.flag = 0;
	// 初期でONの項目
	m_data.flag |= (1 << Vibration);
	if (Game::kDefaultWindowMode)
	{
		m_data.flag |= (1 << WindowMode);
	}

	m_data.reserved1 = 0;	// 予備領域

}

void ConfigData::setFlag(Flag flag, bool isOn)
{
	if (isOn)
	{
		// フラグ立てる
		m_data.flag |= (1 << flag);
	}
	else
	{
		unsigned int mask = ~(1 << flag);
		m_data.flag &= mask;
	}
}
