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
    //ƒŠƒ~ƒbƒgŠÔ‚ğ’´‚¦‚½‚çtrue‚ğ•Ô‚·
    if (m_time >= m_rimitTime) return true;

    m_time++;

    return false;
}
