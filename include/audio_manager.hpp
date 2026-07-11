#pragma once
#include "miniaudio.h"
#include "constants.hpp"


class AudioManager{
    private:
    ma_engine mEngine;
    ma_sound mMusic;
    ma_sound mSfxBounce;
    ma_sound mSfxSpawnBall;
    ma_sound mSfxWidePlatform;
    ma_sound mSfxTest;
    bool mMusicLoaded = false;
    public:
    AudioManager();
    ~AudioManager();
    void playMusic(const char *audioFileName);
    void playSFX(const unsigned int &sfx);
    void stopMusic();
    void setVolumes(float music, float sfx);
};