#pragma once

namespace MyLib 
{

	/// <summary>
	/// 当たり判定のデータ基底
	/// </summary>
	class ColliderData abstract
	{
	public:
		// 当たり判定種別
		enum class e_Kind
		{
			kSphere,	//球
			kCapsule,	//カプセル
			kLine,
		};

		// コンストラクタ
		ColliderData(e_Kind kind, bool isTrigger)
		{
			this->m_kind = kind;
			this->m_isTrigger = isTrigger;
		}

		virtual ~ColliderData() {}

		// 当たり判定種別取得
		e_Kind GetKind() const { return m_kind; }

		// トリガーかどうか
		bool IsTrigger() const { return m_isTrigger; }

	private:
		e_Kind	m_kind;
		bool	m_isTrigger;
	};

}