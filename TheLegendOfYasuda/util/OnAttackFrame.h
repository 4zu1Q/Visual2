#pragma once
class OnAttackFrame
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="startAtk">攻撃の出だしフレーム</param>
	/// <param name="endAtk">攻撃の終了フレーム</param>
	/// <param name="isAttack">攻撃中かどうかのポインタを受け取る</param>
	OnAttackFrame(int startAtk, int endAtk, bool& isAttack);
	virtual ~OnAttackFrame() {};

	void Update();

	//フレームリセット
	void ResetFrame();

private:
	int m_startAttackFrame;	//攻撃の出だしフレーム
	int m_endAttackFrame;	//攻撃の終了フレーム

	int m_nowFrame;			//今のフレーム

	bool* m_isAttack;		//攻撃ポインタ
};

