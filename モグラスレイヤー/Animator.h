#pragma once
#include "DxLib.h"

template<typename T>    //�C�ӂ̃A�j���[�V����enum
class Animator
{
private:
    T attachAnime;    //�Đ����̃A�j���[�V������
    int attachAnimeIndex;           //�A�j���[�V�����̃A�^�b�`���ꂽ�ԍ�
    float animeTimer;               //�A�j���[�V�����̃^�C�}�[
    float animeStopTime;            //�A�j���[�V�����̏I���^�C�}�[
    bool isLoopAnime;               //���[�v����A�j���[�V�������ۂ�

public:
    //�R���X�g���N�^
    Animator()
    {
        attachAnime = static_cast<T>(-1);
        attachAnimeIndex = -1;
        animeStopTime = 0.0f;
        animeTimer = 0.0f;
        isLoopAnime = false;
    }

    //�f�X�g���N�^
    ~Animator()
    {
        MV1DetachAnim(modelHandle, attachAnimeIndex);
    }

    /// <summary>
    /// �A�j���[�V�����̃A�^�b�`
    /// </summary>
    /// <param name="modelHandle">���f���̃n���h��</param>
    /// <param name="anime">animation enum</param>
    /// <param name="isLoop">���[�v�w��</param>
    void SetAnime(const int& modelHandle, const T& anime, const bool& isLoop)
    {
        //���ݍĐ����̃A�j���[�V�������w�肳��Ă��Đݒ肵�Ȃ�
        if (anime == attachAnime) return;

        int anim = static_cast<int>(anime);

        MV1DetachAnim(modelHandle, attachAnimeIndex);

        attachAnimeIndex = MV1AttachAnim(modelHandle, anim, -1, FALSE);
        attachAnime = anime;
        animeStopTime = MV1GetAttachAnimTotalTime(modelHandle, attachAnimeIndex);
        animeTimer = 0.0f;
        isLoopAnime = isLoop;
    }

    /// <summary>
    /// �w�肳�ꂽ�A�j���[�V�������Đ�����������폜
    /// </summary>
    /// <param name="modelHandle">���f���̃n���h��</param>
    /// <param name="anime">animation enum</param>
    void RemoveAnime(const int& modelHandle, const T& anime)
    {
        if (attachAnimeIndex == -1) return;
        if (anime != attachAnime) return;

        MV1DetachAnim(modelHandle, attachAnimeIndex);
        attachAnime = static_cast<T>(-1);
        attachAnimeIndex = -1;
        animeStopTime = 0.0f;
        animeTimer = 0.0f;
        isLoopAnime = false;
    }

    /// <summary>
    /// �A�j���[�V�����̍Đ�
    /// </summary>
    /// <param name="modelHandle">���f���̃n���h��</param>
    void PlayAnime(const int& modelHandle)
    {
        if (attachAnimeIndex == -1) return;

        if (animeStopTime <= animeTimer)
        {
            if (isLoopAnime)
            {   //���[�v�A�j���[�V�����̏ꍇ
                animeTimer = 0.0f;
            }
            else
            {
                RemoveAnime(modelHandle);
            }
        }

        MV1SetAttachAnimTime(modelHandle, attachAnimeIndex, animeTimer);

        animeTimer += 0.5f;
    }

    const T getNowAnime() const { return attachAnime; };

    const bool isEndAnimation(const T& anime) const
    {
        if (anime == attachAnime) return animeStopTime <= animeTimer;
        else return false;
    };
};