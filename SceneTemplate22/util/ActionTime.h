#pragma once
class ActionTime
{
public:
	ActionTime(int rimitTime);
	~ActionTime();

	//タイムリセット
	void Reset();

	//タイム更新
	//リミット時間を超えたらtrueを返す
	bool IsUpdate();

private:
	int m_time;			//経った時間
	int m_rimitTime;	//リミット時間

	bool m_isAction;	//アクションを実行したかどうか
};

