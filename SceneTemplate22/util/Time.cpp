#include "Time.h"

Time::Time(int rimitTime) :
    m_time(0),
    m_rimitTime(rimitTime)
{
}

Time::~Time()
{
}

void Time::Reset()
{
    m_time = 0;
}

bool Time::IsUpdate()
{
    //リミット時間を超えたらtrueを返す
    if (m_time >= m_rimitTime) return true;

    m_time++;

    return false;
}
