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

		/// <summary>
		/// ステージの床に触れているかどうか
		/// </summary>
		/// <returns>ステージの床に触れていたら</returns>
		bool IsGround() { return m_isGround; }

		/// <summary>
		/// ステージの床に触れているかどうかを設定する
		/// </summary>
		/// <param name="flag">ステージの床に触れていたら</param>
		void SetIsGround(bool flag) { m_isGround = flag; }

	private:
		//当たり判定の種類
		e_Kind	m_kind;
		//物理挙動をしないときにtrueにする
		bool m_isTrigger;
		//地面とぶつかってる時にtrueにする
		bool m_isGround = true;
	};

}