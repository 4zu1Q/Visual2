#pragma once
class Time
{
public:
	Time(int rimitTime);
	~Time();

	//�^�C�����~�b�g
	void Reset();

	//�^�C���X�V
	//���~�b�g���Ԃ𒴂�����true��Ԃ�
	bool IsUpdate();

private:
	int m_time;			//�o��������
	int m_rimitTime;	//���~�b�g����

};

