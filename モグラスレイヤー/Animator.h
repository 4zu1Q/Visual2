#pragma once
#include "DxLib.h"

template<typename T>    //任意のアニメーションenum
class Animator
{
private:
    T attachAnime;    //再生中のアニメーション名
    int attachAnimeIndex;           //アニメーションのアタッチされた番号
    float animeTimer;               //アニメーションのタイマー
    float animeStopTime;            //アニメーションの終了タイマー
    bool isLoopAnime;               //ループするアニメーションか否か

public:
    //コンストラクタ
    Animator()
    {
        attachAnime = static_cast<T>(-1);
        attachAnimeIndex = -1;
        animeStopTime = 0.0f;
        animeTimer = 0.0f;
        isLoopAnime = false;
    }

    //デストラクタ
    ~Animator()
    {
        MV1DetachAnim(modelHandle, attachAnimeIndex);
    }

    /// <summary>
    /// アニメーションのアタッチ
    /// </summary>
    /// <param name="modelHandle">モデルのハンドル</param>
    /// <param name="anime">animation enum</param>
    /// <param name="isLoop">ループ指定</param>
    void SetAnime(const int& modelHandle, const T& anime, const bool& isLoop)
    {
        //現在再生中のアニメーションを指定されても再設定しない
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
    /// 指定されたアニメーションが再生中だったら削除
    /// </summary>
    /// <param name="modelHandle">モデルのハンドル</param>
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
    /// アニメーションの再生
    /// </summary>
    /// <param name="modelHandle">モデルのハンドル</param>
    void PlayAnime(const int& modelHandle)
    {
        if (attachAnimeIndex == -1) return;

        if (animeStopTime <= animeTimer)
        {
            if (isLoopAnime)
            {   //ループアニメーションの場合
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