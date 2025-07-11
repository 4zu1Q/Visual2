#pragma once
#include <list>
#include <vector>

namespace MyLib 
{

	/// <summary>
	/// デバッグ用の描画情報をまとめ、後で表示するためのクラス
	/// </summary>
	class DebugDraw
	{
	public:

		/// <summary>
		/// 描画の削除
		/// </summary>
		static void Clear();

		/// <summary>
		/// 描画
		/// </summary>
		static void Draw();

		/// <summary>
		/// ラインの描画
		/// </summary>
		/// <param name="start"> 始点 </param>
		/// <param name="end"> 終点 </param>
		/// <param name="color"> 色 </param>
		static void DrawLine(const VECTOR& start, const VECTOR& end, int color);

		/// <summary>
		/// 球体の描画
		/// </summary>
		/// <param name="center">球体の座標</param>
		/// <param name="radius">半径</param>
		/// <param name="color">色</param>
		/// <param name="alpha"></param>
		/// <param name="isWire"></param>
		static void DrawSphere(const VECTOR& center, float radius, int color);

		/// <summary>
		/// カプセルの描画
		/// </summary>
		/// <param name="posDown">カプセルの下座標</param>
		/// <param name="posUp">カプセルの上座標</param>
		/// <param name="radius">半径</param>
		/// <param name="color">色</param>
		/// <param name="alpha"></param>
		/// <param name="isWire"></param>
		static void DrawCapsule(const VECTOR& downPos, const VECTOR& upPos, float radius, int color);

	private:

		enum class e_HandleKind : int
		{
			kSphere = 0,
			kSphereWire,
			kCapsule,
			kCapsuleWire,
			kNum
		};

		//ライン情報
		struct LineInfo
		{
			VECTOR startPos_;	//始点
			VECTOR endPos_;		//終点
			unsigned int color_; //色
		};

		//球体の描画情報
		struct SphereInfo
		{
			VECTOR center_;		//中心座標
			float radius_;		//半径
			unsigned int color_; //色

		};

		//カプセルの描画情報
		struct CapsuleInfo
		{
			VECTOR downPos_;		//カプセルの下座標
			VECTOR upPos_;		//カプセルの上座標
			float radius_;		//半径
			unsigned int color_;	//色

		};

		//ラインの描画情報リスト
		static std::vector<LineInfo> m_lineInfo;

		//球体の描画情報リスト
		static std::vector<SphereInfo> m_sphereInfo;

		//カプセルの描画情報リスト
		static std::vector<CapsuleInfo> m_capsuleInfo;

	};

}