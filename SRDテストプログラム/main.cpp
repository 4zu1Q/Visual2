#include <iostream>
//#include <pthread.h>
using namespace std;

/*1–â–Ú*/
//int main()
//{
//	int x = 0;
//
//	cin  >> x;
//
//	if (x < 0)
//	{
//		x = x * (-2);
//	}
//
//	cout << x << endl;
//
//	if (x > 2000)
//	{
//		x = x - 2000;
//		cout << x << endl;
//
//	}
//	else
//	{
//		if ((x % 5) == 0)
//		{
//			x = x / 5;
//			cout << x << endl;
//		}
//	}
//
//	return 0;
//}

/* 2–â–Ú */
//int main()
//{
//
//	int x = 0;
//	cin  >> x;
//
//	int d[] = { 1,2,3,4,5,6 };
//	int* e[] = { &d[5],&d[4],&d[3],&d[2],&d[1],&d[0] };
//	
//	int temp0 = *(*(&e[x] - 1));
//	int temp1 = *(e[x] + 2);
//
//	int ans = temp0 * temp1;
//
//	cout << ans << endl;
//
//	return 0;
//
//}

/* 3–â–Ú */
//int main()
//{
//	int n = 0;
//
//	cin >> n;
//	float rate = 0.f;
//
//
//	if (n > 0)
//	{
//		int i;
//
//		for (i = 0; i < n; ++i)
//		{
//			rate += (1 - pow(5.0 / 6.0, n)) / n;
//		}
//	}
//
//	cout << rate << endl;
//	return 0;
//
//}

/* 4–â–Ú */
//int main()
//{
//	float v = 3;
//	float t = 5;
//	float inc = 8;
//	float dec = -1;
//
//	float distance = 0.f;
//
//	distance = (v * v) / (2 * inc) + v * t + (v * v) / (2 * dec);
//
//	cout << distance << endl;
//
//	return 0;
//}

/* 5–â–Ú */
//int main()
//{
//	//‰~A
//	float ax = 50;
//	float ay = 20;
//	float ar = 10;
//
//	//‰~B
//	float bx = 0;
//	float by = 30;
//	float br = 10;
//
//	float center_dis2 = (ax - bx) * (ax - bx) + (ay - by) * (ay - by);
//	float radius_add_dis2 = (ar + br) * (ar + br);
//
//	if (center_dis2 < radius_add_dis2)
//	{
//		cout << "true" << endl;
//	}
//	else
//	{
//		cout << "false" << endl;
//	}
//
//	return 0;
//}


/* 6–â–Ú */
//int dataNum = 0;
//int d[5] = { 0,1,2,3,4 };
//
//int main()
//{
//
//    int insert_index = 20;
//    int value = 10;
//    int N = 3;
//
//    if (N <= dataNum)
//    {
//        printf("”z—ñ‚ÉŠi”[‰Â”\‚ÈêŠ‚ª‚ ‚è‚Ü‚¹‚ñ");
//        return;
//    }
//
//    if (insert_index < 0)
//    {
//        printf("ƒCƒ“ƒfƒbƒNƒXŽw’è‚ª•‰‚Å‚·");
//        return;
//    }
//
//    if(insert_index >= dataNum) 
//    {
//        d[dataNum++] = value;
//    }
//    else
//    {
//
//     int original = d[insert_index];
//
//     if (value > original)
//     {
//         d[dataNum++] = original;
//         d[insert_index] = value;
//     }
//     else
//     {
//         d[dataNum++] = value;
//     }
//
//    }
//
//	return 0;
//}

/* 7–â–Ú */

//namespace
//{
//	constexpr int kNum = 3;
//}
//
//int d[kNum];
//int dataNum = 4;
//
//int main()
//{
//	int data = 0;
//
//	if (dataNum < kNum)
//	{
//		d[dataNum] = data;
//		++dataNum;
//	}
//	
//}

/* 8–â–Ú */
//namespace
//{
//	constexpr int kNum = 3;
//}
//
//int d[kNum];
//
//int main()
//{
//	int i = 0;
//	int k = 0;
//	int tmp = 0;
//	int keep_index = 0;
//
//	for (i = 0; i < (kNum - 1); ++i)
//	{
//		int div_num = 10;
//		int check_num0 = 0;
//		int check_num1 = 0;
//
//		while (1)
//		{
//			check_num0 = d[keep_index] % div_num;
//			check_num1 = d[k] % div_num;
//			if (check_num0 == check_num1)
//			{
//				div_num = div_num * 10;
//			}
//			else
//			{
//				if (check_num0 > check_num1)
//				{
//					keep_index = k;
//				}
//				break;
//			}
//		}
//		tmp = d[i];
//		d[i] = d[keep_index];
//		d[keep_index] = tmp;
//	}
//}

/* 9–â–Ú */

//namespace
//{
//	constexpr int kM = 1;
//	constexpr int kN = 2;
//}
//
//int d[kM][kN];
//int total[kN];
//
//int main()
//{
//	int member_index = 0;
//	for (member_index = 0; member_index < N; ++member_index)
//	{
//		total[member_index] = 0;
//	}
//
//	int game_index;
//	for (game_index = 0; game_index < M; ++game_index)
//	{
//		
//	}
//
//}

/* 10–â–Ú */

//struct Node 
//{
//	Node* mPrevNode;
//	Node* mNextNode;
//
//	int mValue;
//};
//
//int main()
//{
//	Node* node0;
//	Node* node1;
//
//	if ((node0 == node1) || (node0 == nullptr) || (node1 == nullptr))
//	{
//		return;
//	}
//	if ((node0->mPrevNode == nullptr) && (node0->mNextNode == nullptr))
//	{
//		return;
//	}
//	if ((node1->mPrevNode == nullptr) && (node1->mNextNode == nullptr))
//	{
//		return;
//	}
//	
//
//
//}