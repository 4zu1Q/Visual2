#include "OnAttackFrame.h"
#include<DxLib.h>

OnAttackFrame::OnAttackFrame(int startAtk, int endAtk, bool& isAttack) :
	m_startAttackFrame(startAtk),
	m_endAttackFrame(endAtk),
	m_nowFrame(0),
	m_isAttack(&isAttack)
{
}

void OnAttackFrame::Update()
{
	m_nowFrame = min(m_nowFrame++, m_endAttackFrame);

	//現在のフレームが攻撃開始フレームより大きくかつ、
	//攻撃終了フレームより小さい場合
	if (m_nowFrame >= m_startAttackFrame &&
		m_nowFrame < m_endAttackFrame)
	{
		*m_isAttack = true;
		return;
	}

	*m_isAttack = false;
}

void OnAttackFrame::ResetFrame()
{
	m_nowFrame = 0;
	*m_isAttack = false;
}
