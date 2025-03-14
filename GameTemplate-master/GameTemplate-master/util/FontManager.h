#pragma once
#include <vector>
#include <string>
#include <map>

// �V���O���g�����g�p�����t�H���g�Ǘ��N���X
class FontManager
{
private:
	FontManager() = default;
	virtual ~FontManager() = default;

	static FontManager* m_instance;
public:
	// �R�s�[�R���X�g���N�^�̋֎~
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

	// csv�ǂݍ��݁A�t�H���g�̃��x���s��
	void init();
	// init�Ő�������m_fontDataTbl�ɓo�^����Ă���t�H���g�̐���
	void createFont();	
	// �t�H���g�̐����I������
	bool isCreateEnd();

	// �I��
	void end();

	// �w�肵���t�H���gID�̃t�H���g�n���h���擾
	int getHandle(std::string id) const;


//#ifdef _DEBUG
	// �f�o�b�O�p�֐�

	// �t�H���g�̍ēǂݍ���
	
	
	// �o�^����Ă���t�H���g�̐����擾
	int getFontNum() const;
	// �o�^�ԍ��̃t�H���gID���擾
	std::string getKey(int index) const;
	// �o�^�ԍ��̃t�H���g�n���h�����擾
	int getHandle(int index) const;

//#endif

private:
	// �������֘A����
	void loadCsv();			// csv�t�@�C����ǂݍ���ŕK�v�ȏ��ɕϊ�
	void loadFontData();	// m_fileName�ɓo�^���ꂽ�t�H���g�f�[�^��ǂݍ���

	// �ǂݍ��ޕK�v�̂���t�H���g�̃t�@�C������ǉ�����(���łɓo�^����Ă���ꍇ�͒ǉ����Ȃ�)
	void addLoadFontFilename(std::string filename);

private:
	// �t�H���g�����p�f�[�^�\����
	struct FontData
	{
		std::string name;
		int size;
		int thick;
		int type;
	};

private:

	// �ǂݍ��ޕK�v�̂���t�H���g�t�@�C����
	std::vector<std::string>	m_fileName;

	// csv����W�J������������K�v�̂���t�H���g�̏��
	std::vector<FontData> m_fontDataTbl;

	// ���������t�H���g�n���h��
	std::vector<int>	m_handle;

	// �t�H���gID�ƃt�H���g�ԍ��̃f�[�^
	std::map<std::string, int>	m_idData;

};

