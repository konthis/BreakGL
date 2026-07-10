#pragma once
#include "miniaudio.h"


class AudioManager{
    private:
    ma_engine mEngine;
    ma_sound mMusic;
    bool mMusicLoaded = false;
    public:
    AudioManager();
    ~AudioManager();
    void playMusic(const char *audioFileName);
    void playSFX(const char *audioFileName);
    void stopMusic();
};