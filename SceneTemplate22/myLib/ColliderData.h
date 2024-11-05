#pragma once

namespace MyLib {

	/// <summary>
	/// 当たり判定のデータ基底
	/// </summary>
	class ColliderData abstract
	{
	public:
		// 当たり判定種別
		enum class e_Kind
		{
			Circle3D,	//球
			Capsule3D,	//カプセル
			Line2D,
		};

		// コンストラクタ
		ColliderData(e_Kind kind, bool isTrigger)
		{
			this->kind = kind;
			this->isTrigger = isTrigger;
		}

		virtual ~ColliderData() {}

		// 当たり判定種別取得
		e_Kind GetKind() const { return kind; }

		// トリガーかどうか
		bool IsTrigger() const { return isTrigger; }

	private:
		e_Kind	kind;
		bool	isTrigger;
	};

}