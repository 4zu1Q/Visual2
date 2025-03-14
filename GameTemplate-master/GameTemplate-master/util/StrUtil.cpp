#include "StrUtil.h"
#include <cassert>

#include <fstream>
#include <sstream>

namespace
{
	
}

namespace StrUtil
{
	std::vector<std::string> splitString(const std::string str)
	{
		std::vector<std::string>	result;

		std::string temp = str;
		int byteCount = 0;
		while (1)
		{
			if (byteCount > temp.size())
			{
				// �I��
				break;
			}

			std::string chr;	// 1�������̃f�[�^

			// �����̃o�C�g���`�F�b�N
			int size = 0;
			unsigned char charData = temp[byteCount];	// �`�F�b�N���镶��
			if (charData < 0x80)
			{
				size = 1;
			}
			else
			{
				size = 2;
			}
			// �K�v�ꕪ�̂ݐ؂���
			chr = temp.substr(byteCount, size);

			// �P�����i�߂�
			byteCount += size;
			if (chr != "\0")	// ���O���镶��
			{
				result.push_back(chr);
			}
		}

		return result;
	}
	std::vector<std::string> split(std::string& input, char delimiter)
	{
		std::istringstream stream(input);	// string��getline�Ŏg����f�[�^�ɕϊ��H
		std::string field;					// ��������������1�����i�[����H
		std::vector<std::string> result;	// ������̕�����̔z��

		while (getline(stream, field, delimiter))
		{
			result.push_back(field);
		}
		return result;
	}
}