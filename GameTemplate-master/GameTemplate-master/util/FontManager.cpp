#include "FontManager.h"
#include "DxLib.h"

#include "StrUtil.h"

#include <cassert>
#include <fstream>
#include <sstream>

namespace
{
	const char* const kFontFilePath = "data/font/";
}

FontManager* FontManager::m_instance = nullptr;

void FontManager::init()
{
	loadCsv();
	loadFontData();
}

void FontManager::createFont()
{
	// フォントの生成
	for (auto& data : m_fontDataTbl)
	{
		int handle = CreateFontToHandle(data.name.c_str(), data.size, data.thick, data.type);
		m_handle.push_back(handle);
	}
}

bool FontManager::isCreateEnd()
{
	// 生成終わっている判定
	for (auto& handle : m_handle)
	{
		if (CheckHandleASyncLoad(handle))	return false;
	}
	return true;
}

void FontManager::end()
{
	// フォントの削除
	for (auto& handle : m_handle)
	{
		DeleteFontToHandle(handle);
	}
	m_handle.clear();

	// フォントのアンロード
	for (auto& name : m_fileName)
	{
		std::string filePath = kFontFilePath + name;
		if (RemoveFontResourceEx(filePath.c_str(), FR_PRIVATE, NULL)) {
		}
		else {
			MessageBox(NULL, "remove failure", "", MB_OK);
		}
	}
	m_fileName.clear();

	m_idData.clear();
}

int FontManager::getHandle(std::string id) const
{
	assert(m_idData.count(id) > 0);	// 存在しないIDを指定した

	int index = m_idData.at(id);
	return m_handle[index];
}

int FontManager::getFontNum() const
{
	return m_idData.size();
}

std::string FontManager::getKey(int index) const
{
	for (auto& data : m_idData)
	{
		if (index <= 0)	return data.first;

		index--;
	}
	assert(-1);
	return "";
}

int FontManager::getHandle(int index) const
{
	for (auto& data : m_idData)
	{
		if (index <= 0)
		{
			return m_handle[data.second];
		}

		index--;
	}
	assert(-1);
	return -1;
}

void FontManager::loadCsv()
{
	// ファイル情報の読み込み
	std::ifstream ifs("data/csv/fontData.csv");
	std::string line;

	getline(ifs, line);	// 1行目は読み捨て

	while (getline(ifs, line))
	{
		// csvデータ1行を','で複数の文字列に分割
		std::vector<std::string>	strvec = StrUtil::split(line, ',');
		// strvec[0]	: フォントID	
		// strvec[1]	: ファイル名	
		// strvec[2]	: フォント名	
		// strvec[3]	: サイズ	
		// strvec[4]	: 太さ	
		// strvec[5]	: エッジ(ふち)
		// strvec[6]	: 同じフォント	
		// strvec[7]	: メモ(この項目の内容は使用しない)

		std::string fontId = strvec[0];	// 登録したいフォントのID
		std::string refId = strvec[6];	// 参照先フォントのID
		if (refId == "")
		{
			// フォントの新規登録
			int newIndex = static_cast<int>(m_fontDataTbl.size());
			// データの登録
			FontData temp;
			temp.name = strvec[2];
			temp.size = stoi(strvec[3]);
			temp.thick = stoi(strvec[4]);
			if (stoi(strvec[5]))
			{
				temp.type = DX_FONTTYPE_ANTIALIASING_EDGE_8X8;	// ふちあり
			}
			else
			{
				temp.type = DX_FONTTYPE_ANTIALIASING_8X8;	// ふちなし
			}
			m_fontDataTbl.push_back(temp);

			assert(m_idData.count(fontId) == 0);	// 登録しようとしているIDがすでに存在する

			m_idData[fontId] = newIndex;

			// フォントファイルの読み込みデータ追加
			if (strvec[1] != "")
			{
				addLoadFontFilename(strvec[1]);
			}
		}
		else
		{
			// すでに登録しているフォントを使用する
			assert(m_idData.count(refId) > 0);	// 存在しないフォントを参照しようとした
			m_idData[fontId] = m_idData[refId];	// 参照先フォントのIDを使用する
		}

	}
}

void FontManager::loadFontData()
{
	// フォントのロード
	for (auto& name : m_fileName)
	{
		std::string filePath = kFontFilePath + name;
		if (AddFontResourceEx(filePath.c_str(), FR_PRIVATE, NULL) > 0) {
		}
		else {
			// フォント読込エラー処理
			MessageBox(NULL, "フォント読込失敗", "", MB_OK);
		}
	}
}

void FontManager::addLoadFontFilename(std::string filename)
{
	for (auto& name : m_fileName)
	{
		// すでに登録済み
		if (name == filename)	return;
	}

	m_fileName.push_back(filename);
}
