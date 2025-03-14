#include "game.h"
#include "SceneTest.h"
#include "SceneDebug.h"

#include "Pad.h"

#include "FontManager.h"

#include <string>
#include <cassert>

namespace
{
}

SceneTest::SceneTest():
	m_frameCount(0),
	m_objPos(VGet(0.0f, 0.0f, 0.0f))
{
	// �V�[���ݒ�
	setDispLoading(false);	// ���[�h���\�����s��Ȃ�
}

void SceneTest::startLoad()
{
	// �񓯊��ǂݍ��݂��J�n����
	SetUseASyncLoadFlag(true);


	// �f�t�H���g�ɖ߂�
	SetUseASyncLoadFlag(false);
}

bool SceneTest::isLoaded() const
{
	// �S�Ẵ��\�[�X�̃��[�h���������������m�F����
	//if (CheckHandleASyncLoad(m_handle))	return false;

	return true;
}

void SceneTest::init()
{
	/* 3D�֘A�̏����� */

	//Z�o�b�t�@��L���ɂ���
	SetUseZBuffer3D(true);
	SetWriteZBuffer3D(true);

	//�w�ʃJ�����O��L���ɂ���
	SetUseBackCulling(true);	

	//�J�����̐ݒ�
	SetCameraNearFar(5.0f, 2800.0f);
	//�J�����̎���p��ݒ�(���W�A��)
	SetupCamera_Perspective(60.0f * DX_PI_F / 180.0f);
	//�J�����̈ʒu�A�ǂ������Ă��邩��ݒ肷��
	SetCameraPositionAndTarget_UpVecY(VGet(0.0f, 300.0f, -800.0f), VGet(0.0f, 0.0f, 0.0f));

	//�n�`�f�[�^�̍쐬
	//���炩���ߊe���_�̈ʒu���݂̂��쐬����
	std::vector<VECTOR> fieldPos;
	for (int z = -500; z <= 500; z += 100)
	{
	
		for (int x = -500; x <= 500; x += 100)
		{
			VECTOR pos = VGet(x, GetRand(128) - 64, z);
			fieldPos.push_back(pos);
		}

	}

	//1���C���̒��_��
	constexpr int kLineVtxNum = 11;

	//200�̃|���S���\���Ɏg�p���钸�_�f�[�^���쐬����
	//1�|���S��3���_�Ȃ̂ŁA200�|���S���Ȃ�600���_

	for (int indexZ = 0; indexZ < kLineVtxNum - 1; indexZ++)
	{
		for (int indexX = 0; indexX < kLineVtxNum - 1; indexX++)
		{
			VERTEX3D vtx[3];
			//1�|���S��3���_�̃f�[�^���쐬

			//indexZ,indexX�Ŏw�肳�ꂽ���_�ԍ���fieldPos��Index�ɕϊ�����
			vtx[0].pos = fieldPos[indexZ * kLineVtxNum + indexX];
			vtx[1].pos = fieldPos[indexZ * kLineVtxNum + indexX + kLineVtxNum];
			vtx[2].pos = fieldPos[indexZ * kLineVtxNum + indexX + 1];

			for (int i = 0; i < 3; i++)
			{
				VECTOR tempVec1 = VSub(vtx[1].pos, vtx[0].pos);
				VECTOR tempVec2 = VSub(vtx[2].pos, vtx[1].pos);

				vtx[i].norm = VNorm(VCross(tempVec1, tempVec2));
				vtx[i].dif = GetColorU8(255, 255, 0, 255);
				vtx[i].spc = GetColorU8(0, 0, 0, 0);
				vtx[i].u = 0.0f;
				vtx[i].v = 0.0f;
				vtx[i].su = 0.0f;
				vtx[i].sv = 0.0f;
			}
			//1�|���S�����̃f�[�^��ǉ�
			m_fieldVtx.push_back(vtx[0]);
			m_fieldVtx.push_back(vtx[1]);
			m_fieldVtx.push_back(vtx[2]);
		}

	}

	for (int indexZ = 0; indexZ < kLineVtxNum - 1; indexZ++)
	{
		for (int indexX = 0; indexX < kLineVtxNum - 1; indexX++)
		{
			VERTEX3D vtx[3];
			//1�|���S��3���_�̃f�[�^���쐬

			//indexZ,indexX�Ŏw�肳�ꂽ���_�ԍ���fieldPos��Index�ɕϊ�����
			vtx[0].pos = fieldPos[indexZ * kLineVtxNum + indexX + 1];
			vtx[1].pos = fieldPos[indexZ * kLineVtxNum + indexX + kLineVtxNum];
			vtx[2].pos = fieldPos[indexZ * kLineVtxNum + indexX + kLineVtxNum + 1];

			for (int i = 0; i < 3; i++)
			{
				VECTOR tempVec1 = VSub(vtx[1].pos, vtx[0].pos);
				VECTOR tempVec2 = VSub(vtx[2].pos, vtx[1].pos);

				vtx[i].norm = VNorm(VCross(tempVec1, tempVec2));
				vtx[i].dif = GetColorU8(255, 255, 0, 255);
				vtx[i].spc = GetColorU8(0, 0, 0, 0);
				vtx[i].u = 0.0f;
				vtx[i].v = 0.0f;
				vtx[i].su = 0.0f;
				vtx[i].sv = 0.0f;
			}
			//1�|���S�����̃f�[�^��ǉ�
			m_fieldVtx.push_back(vtx[0]);
			m_fieldVtx.push_back(vtx[1]);
			m_fieldVtx.push_back(vtx[2]);
		}

	}

}

void SceneTest::end()
{

}

void SceneTest::update()
{
	if (getFadeProgress() > 0.0f)
	{
		return;
	}
	m_frameCount++;

	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);

	//2D���W�ł���}�E�X�|�C���^�̈ʒu��3D���W�ɕϊ�����K�v������

	//2D���W��3D���W�ɕϊ�����Ƃ�
	//ConvScreenPosToWorldPos(VECTOR ScreenPos);
	//���g�p����
	//������ScreenPos��x,y�̓X�N���[�����W���w�肷��(����̓}�E�X�|�C���^�̈ʒu)
	//z�ɂ�0.0f�`1.0f�̒l���w�肷��
	//z=0.0f�̏ꍇ��near�̈ʒu�ɂ���3D���W���擾�ł���
	//z=1.0f�̏ꍇ��far�̈ʒu�ɂ���3D���W���擾�ł���

	//���������̊J�n�ʒu�A�I���ʒu���擾����
	// ��������Ԓ����Ɗe�|���S���̌���������s���A�ŏI�I�ɒn�`�̂ǂ��ɒu���������肷��
	VECTOR eyeStart = ConvScreenPosToWorldPos(VGet(mouseX, mouseY, 0.0f));
	VECTOR eyeEnd = ConvScreenPosToWorldPos(VGet(mouseX, mouseY, 1.0f));

	//���������̒����Ɗe�n�`�|���S�����\������O�p�`�Ƃ̌���������s���A�ŏI�I�Ȉʒu�ɂ����肷��


	if( Pad::isTrigger(PAD_INPUT_1) )
	{
		// ��x�Ă񂾂������x�Ă΂Ȃ��悤�ɁI
		// �Ă񂾂�t�F�[�h�A�E�g���n�܂�̂ł��̃\�[�X�ł�isFading�ł͂����Ă���
		setNextScene(new SceneDebug);
	}
}

void SceneTest::draw()
{
	// ���\�[�X�̃��[�h���I���܂ł͕`�悵�Ȃ��̂��悳����
	// (�ǂ���ɂ���t�F�[�h�d�؂��Ă���̂ŉ��������Ȃ��͂�)
	if (!isLoaded())	return;

	//�}�E�X�J�[�\���̂����Ă���ʒu�ɕ��̂�\������
	DrawCone3D(VAdd(m_objPos, VGet(0, 64, 0)), VGet(0, 0, 0), 16.0f, 64, 0x00ffff, 0x00ffff, true);

	DrawPolygon3D(m_fieldVtx.data(), m_fieldVtx.size() / 3, DX_NONE_GRAPH, false);

	drawGrid();
	DrawFormatString(0, 0, 0xffffff, "SceneTest FRAME:%d", m_frameCount);

}

void SceneTest::drawGrid()
{

	for (float x = -500.0f; x <= 500.0f; x += 100.0f)
	{
		VECTOR start = VGet(x, 0.0f, -500.0f);
		VECTOR end = VGet(x, 0.0f, 500.0f);
		DrawLine3D(start, end, 0xff00ff);
	}

	for (float z = -500.0f; z <= 500.0f; z += 100.0f)
	{
		VECTOR start = VGet(-500.0f, 0.0f, z);
		VECTOR end = VGet(500.0f, 0.0f, z);
		DrawLine3D(start, end, 0x0000ff);
	}
}
