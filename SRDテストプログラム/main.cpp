#include <iostream>
//#include <pthread.h>
using namespace std;

/*1���*/
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

/* 2��� */
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

/* 3��� */
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

/* 4��� */
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

/* 5��� */
//int main()
//{
//	//�~A
//	float ax = 50;
//	float ay = 20;
//	float ar = 10;
//
//	//�~B
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


/* 6��� */

//namespace 
//{
//    constexpr int kNum = 3;
//}
//
//int dataNum = 0;
//int d[kNum] = { 0,1,2 };
//
//int main()
//{
//
//    int insert_index = 0;
//    int value = 0;
//
//    cin >> insert_index;
//    cin >> value;
//
//	//�f�[�^�����z��Ɋi�[������Ȃ��ꍇ
//    if (kNum <= dataNum)
//    {
//        printf("�z��Ɋi�[�\�ȏꏊ������܂���");
//        return;
//    }
//
//	//�w�肵�Ă���C���f�b�N�X�����̐���������
//    if (insert_index < 0)
//    {
//        printf("�C���f�b�N�X�w�肪���ł�");
//        return;
//    }
//
//	//�w��ʒu���i�[�����傫���ꍇ�͊i�[�ꍇ
//    if(insert_index >= dataNum) 
//    {
//        //�i�[�����ɒǉ�����
//        d[dataNum++] = value;
//    }
//    //�w��ʒu�Ƀf�[�^���i�[�ς݂̏ꍇ
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

/* 7��� */

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

/* 8��� */

//namespace
//{
//	//�萔
//	constexpr int N = 3;
//}
//
////�O���[�o���ϐ�
//int d[N] = { 0,0,0 };
//
//int main()
//{
//	cin >> d[0];
//	cin >> d[1];
//	cin >> d[2];
//
//	int i = 0;
//	int k = 0;
//	int tmp = 0;
//	int keep_index = 0;
//
//	/* ���̃R�[�h */
//for (i = 0; i < (N - 1); ++i)
//{
//	keep_index = 0;
//
//	for (k = i + 1; k < (N - 1); ++k)
//	{
//		int div_num = 10;
//		int check_num0 = 0;
//		int check_num1 = 0;
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
//	}
//	tmp = d[i];
//	d[i] = d[keep_index];
//	d[keep_index] = tmp;
//}
//
//	/* �C���̃R�[�h */
//	for (i = 0; i < N; i++) // �C��
//	{  
//		  keep_index = i;	// �C��
//
//		for (k = i + 1; k < N; k++) // �C��
//		{  
//			int div_num = 10;
//			int check_num0 = 0;
//			int check_num1 = 0;
//
//			while (1) 
//			{
//				 check_num0 = d[i] % div_num;  // �C��
//				check_num1 = d[k] % div_num;
//				if (check_num0 == check_num1) 
//				{
//					div_num = div_num * 10;
//				}
//				else 
//				{
//					if (check_num0 < check_num1)// �C��
//					{
//						keep_index = k;
//					}
//					break;
//				}
//			}
//		}
//		tmp = d[i];
//		d[i] = d[keep_index];
//		d[keep_index] = tmp;
//	}
//
//	cout << d[0] << endl;
//	cout << d[1] << endl;
//	cout << d[2] << endl;
//
//	return 0;
//}

/* 9��� */

//namespace
//{
//	//�萔
//	constexpr int M = 3;
//	constexpr int N = 5;
//}
//
////�O���[�o���ϐ�
//int d[M][N] = { { 2, 2, 2, 2, 2 },
//                { 5, 6, 7, 8, 9 },
//                {10,11,12,13,14 } };
//int total[N];
//
//int main()
//{
//	// �Q�[��������l��
//	int member_index = 0;
//
//	// �Q�[��������l�����̃X�R�A��������
//	for (member_index = 0; member_index < N; ++member_index)
//	{
//		total[member_index] = 0;
//	}
//
//	// �Q�[���̐�
//	int game_index;
//
//	// �Q�[���̐����������[�v
//	for (game_index = 0; game_index < M; ++game_index)
//	{
//
		//int min_card = 1000000; //�J�[�h�̍ŏ��l
  //      int max_card = -1;      //�J�[�h�̍ő�l
		//int min_count = 0;      //�ŏ������������l�̐�
		//int max_count = 0;      //�ő吔���o�����l�̐�
		//bool all_same = false;   //�����o�[�S���������J�[�h�������Ă��邩�ǂ����̃t���O

		//// �Q�[��������l�����������[�v
  //      for (member_index = 0; member_index < N; member_index++)
  //      {
		//	//�����o�[�̃J�[�h���擾
  //          int card = d[game_index][member_index];

		//	//���ݎ擾���Ă���J�[�h���ŏ��l��菬�����ꍇ
  //          if (card < min_card)
  //          {
  //              min_card = card;
  //              min_count = 1;
  //          } 
		//	//���ݎ擾���Ă���J�[�h���ŏ��l�Ɠ����ꍇ
  //          else if (card == min_card)
  //          {
  //              min_count++;
  //          }

		//	//���ݎ擾���Ă���J�[�h���ő�l���傫���ꍇ
  //          if (card > max_card) 
  //          {
  //              max_card = card;
  //              max_count = 1; 
  //          }
		//	//���ݎ擾���Ă���J�[�h���ő�l�Ɠ����ꍇ
  //          else if (card == max_card)
  //          {
  //              max_count++;
  //          }

		//	//�S���������J�[�h�������Ă���ꍇ�̓t���O�𗧂Ă�
  //          if (card != d[game_index][0])
  //          {
  //              all_same = true;
  //          }
  //      }

		//// �S���������J�[�h�������Ă���ꍇ�̓X�R�A�����Z���Ȃ�
  //      if (!all_same) continue;

		////�������������o�[�����炦��X�R�A���v�Z
  //      int score = (max_card - min_card) * max_count / min_count;

		////�X�R�A��1�����̏ꍇ��1�ɂ���
  //      score = score < 1 ? 1 : score;

  //      //�����o�[�S�������[�v
  //      for (member_index = 0; member_index < N; member_index++)
  //      {
  //          //�������������o�[�̃J�[�h���ŏ��l�J�[�h�Ɠ����ꍇ
  //          if (d[game_index][member_index] == min_card)
  //          {
  //              //�ŏ��l�������Ă��郁���o�[�̃X�R�A�����Z
  //              total[member_index] += score;
  //          }
  //      }
//	}
//
//	for (member_index = 0; member_index < N; ++member_index)
//	{
//		cout << total[member_index] << endl;
//	}
//
//	return 0;
//}

/* 10��� */

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

struct Node
{
	Node* mPrevNode = nullptr;
	Node* mNextNode = nullptr;
	int mValue = -1;
};

//int main(Node* node0 , Node* node1)
//{
//
//	cout << "Before:" << node1 << endl;
//
//    // ����ւ��p�̃|�C���^��ێ�
//    Node* node0Prev = node0->mPrevNode;
//    Node* node0Next = node0->mNextNode;
//    Node* node1Prev = node1->mPrevNode;
//    Node* node1Next = node1->mNextNode;
//
//    // node0 �̑O��� node1 ���Ȃ�
//    if (node0Prev) {
//        node0Prev->mNextNode = node1;
//    }
//    if (node0Next) {
//        node0Next->mPrevNode = node1;
//    }
//
//    // node1 �̑O��� node0 ���Ȃ�
//    if (node1Prev) {
//        node1Prev->mNextNode = node0;
//    }
//    if (node1Next) {
//        node1Next->mPrevNode = node0;
//    }
//
//    // �אڃm�[�h�̓���ւ��ɑΉ�������ʂȏ���
//    if (node0Next == node1) {
//        // node0 �� node1 ���אځinode0 ���O�j
//        node1->mPrevNode = node0Prev;
//        node1->mNextNode = node0;
//        node0->mPrevNode = node1;
//        node0->mNextNode = node1Next;
//    }
//    else if (node1Next == node0) {
//        // node1 �� node0 ���אځinode1 ���O�j
//        node0->mPrevNode = node1Prev;
//        node0->mNextNode = node1;
//        node1->mPrevNode = node0;
//        node1->mNextNode = node0Next;
//    }
//    else {
//        // �אڂ��Ă��Ȃ��ꍇ
//        node0->mPrevNode = node1Prev;
//        node0->mNextNode = node1Next;
//        node1->mPrevNode = node0Prev;
//        node1->mNextNode = node0Next;
//    }
//
//    cout << "After:" << node1 << endl;
//
//    return 0;
//}



void func(Node* node0, Node* node1)
{
	if ((node0 == node1) || (node0 == nullptr) || (node1 == nullptr)) return;
	if ((node0->mPrevNode == nullptr) && (node0->mNextNode == nullptr)) return;
	if ((node1->mPrevNode == nullptr) && (node1->mNextNode == nullptr)) return;

	Node* prev0 = node0->mPrevNode;
	Node* next0 = node0->mNextNode;
	Node* prev1 = node1->mPrevNode;
	Node* next1 = node1->mNextNode;

	if (next0 == node1)
	{
		if (prev0)
		{
			prev0->mNextNode = node1;
		}
		node0->mPrevNode = node1;
		node0->mNextNode = next1;
		node1->mPrevNode = prev0;
		node1->mNextNode = node0;
	}
	else if (next1 == node0)
	{
		if (prev1)
		{
			prev1->mNextNode = node0;
		}
		node1->mPrevNode = node0;
		node1->mNextNode = next0;
		node0->mPrevNode = prev1;
		node0->mNextNode = node1;
	}
	else
	{
		if (prev0)
		{
			prev0->mNextNode = node1;
		}
		if (prev1)
		{
			prev1->mNextNode = node0;
		}
		node0->mPrevNode = prev1;
		node0->mNextNode = next1;
		node1->mPrevNode = prev0;
		node1->mNextNode = next0;
	}
}

void printList(Node* head)
{
	Node* temp = head;
	while (temp != nullptr)
	{
		std::cout << temp->mValue << " ";
		temp = temp->mNextNode;
	}
	std::cout << std::endl;
}


int main()
{
	// �m�[�h�̐���
	Node* node1 = new Node{ nullptr, nullptr, 1 };
	Node* node2 = new Node{ nullptr, nullptr, 2 };
	Node* node3 = new Node{ nullptr, nullptr, 3 };
	Node* node4 = new Node{ nullptr, nullptr, 4 };

	// �o�������X�g���쐬
	node1->mNextNode = node2;
	node2->mPrevNode = node1;
	node2->mNextNode = node3;
	node3->mPrevNode = node2;
	node3->mNextNode = node4;
	node4->mPrevNode = node3;

	// ���X�g�̕\��
	std::cout << "Before swapping:" << std::endl;
	printList(node1);

	// �m�[�h�̈ʒu�����ւ���
	func(node1, node2);

	// ����ւ���̃��X�g�\��
	std::cout << "After swapping:" << std::endl;
	printList(node1);

	return 0;
}