#include "ActionTime.h"

ActionTime::ActionTime(int rimitTime) :
	m_time(0),
	m_rimitTime(rimitTime),
	m_isAction(false)
{
}

ActionTime::~ActionTime()
{
}

void ActionTime::Reset()
{
	//アクションをリセット
	m_isAction = false;
	m_time = 0;
}

bool ActionTime::IsUpdate()
{
	//アクションが実行されていたらfalseを返す
	if (m_isAction) return false;

	//リミット時間を超えたらアクションをtrueにしてtrueを返す
	if (m_time >= m_rimitTime)
	{
		m_isAction = true;
		return true;
	}
	m_time++;

	return false;
}
