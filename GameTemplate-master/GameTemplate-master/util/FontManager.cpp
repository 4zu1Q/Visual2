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
	// �t�H���g�̐���
	for (auto& data : m_fontDataTbl)
	{
		int handle = CreateFontToHandle(data.name.c_str(), data.size, data.thick, data.type);
		m_handle.push_back(handle);
	}
}

bool FontManager::isCreateEnd()
{
	// �����I����Ă��锻��
	for (auto& handle : m_handle)
	{
		if (CheckHandleASyncLoad(handle))	return false;
	}
	return true;
}

void FontManager::end()
{
	// �t�H���g�̍폜
	for (auto& handle : m_handle)
	{
		DeleteFontToHandle(handle);
	}
	m_handle.clear();

	// �t�H���g�̃A�����[�h
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
	assert(m_idData.count(id) > 0);	// ���݂��Ȃ�ID���w�肵��

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
	// �t�@�C�����̓ǂݍ���
	std::ifstream ifs("data/csv/fontData.csv");
	std::string line;

	getline(ifs, line);	// 1�s�ڂ͓ǂݎ̂�

	while (getline(ifs, line))
	{
		// csv�f�[�^1�s��','�ŕ����̕�����ɕ���
		std::vector<std::string>	strvec = StrUtil::split(line, ',');
		// strvec[0]	: �t�H���gID	
		// strvec[1]	: �t�@�C����	
		// strvec[2]	: �t�H���g��	
		// strvec[3]	: �T�C�Y	
		// strvec[4]	: ����	
		// strvec[5]	: �G�b�W(�ӂ�)
		// strvec[6]	: �����t�H���g	
		// strvec[7]	: ����(���̍��ڂ̓��e�͎g�p���Ȃ�)

		std::string fontId = strvec[0];	// �o�^�������t�H���g��ID
		std::string refId = strvec[6];	// �Q�Ɛ�t�H���g��ID
		if (refId == "")
		{
			// �t�H���g�̐V�K�o�^
			int newIndex = static_cast<int>(m_fontDataTbl.size());
			// �f�[�^�̓o�^
			FontData temp;
			temp.name = strvec[2];
			temp.size = stoi(strvec[3]);
			temp.thick = stoi(strvec[4]);
			if (stoi(strvec[5]))
			{
				temp.type = DX_FONTTYPE_ANTIALIASING_EDGE_8X8;	// �ӂ�����
			}
			else
			{
				temp.type = DX_FONTTYPE_ANTIALIASING_8X8;	// �ӂ��Ȃ�
			}
			m_fontDataTbl.push_back(temp);

			assert(m_idData.count(fontId) == 0);	// �o�^���悤�Ƃ��Ă���ID�����łɑ��݂���

			m_idData[fontId] = newIndex;

			// �t�H���g�t�@�C���̓ǂݍ��݃f�[�^�ǉ�
			if (strvec[1] != "")
			{
				addLoadFontFilename(strvec[1]);
			}
		}
		else
		{
			// ���łɓo�^���Ă���t�H���g���g�p����
			assert(m_idData.count(refId) > 0);	// ���݂��Ȃ��t�H���g���Q�Ƃ��悤�Ƃ���
			m_idData[fontId] = m_idData[refId];	// �Q�Ɛ�t�H���g��ID���g�p����
		}

	}
}

void FontManager::loadFontData()
{
	// �t�H���g�̃��[�h
	for (auto& name : m_fileName)
	{
		std::string filePath = kFontFilePath + name;
		if (AddFontResourceEx(filePath.c_str(), FR_PRIVATE, NULL) > 0) {
		}
		else {
			// �t�H���g�Ǎ��G���[����
			MessageBox(NULL, "�t�H���g�Ǎ����s", "", MB_OK);
		}
	}
}

void FontManager::addLoadFontFilename(std::string filename)
{
	for (auto& name : m_fileName)
	{
		// ���łɓo�^�ς�
		if (name == filename)	return;
	}

	m_fileName.push_back(filename);
}
