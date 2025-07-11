#pragma once
class Time
{
public:
	Time(int rimitTime);
	~Time();

	//タイムリミット
	void Reset();

	//タイム更新
	//リミット時間を超えたらtrueを返す
	bool IsUpdate();

private:
	int m_time;			//経った時間
	int m_rimitTime;	//リミット時間

};

