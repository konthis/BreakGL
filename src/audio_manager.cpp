#include "audio_manager.hpp"

AudioManager::AudioManager(){
    ma_result result;

    result = ma_engine_init(NULL, &mEngine);
    if (result != MA_SUCCESS) {
        throw "failed to load audio engine";
    }

}

AudioManager::~AudioManager(){
    ma_engine_uninit(&mEngine);
    if(mMusicLoaded){
        ma_sound_uninit(&mMusic);
    }
}

void AudioManager::playMusic(const char* audioFileName){
    if(mMusicLoaded){
        ma_sound_uninit(&mMusic);
    }
    ma_result result = ma_sound_init_from_file(&mEngine, audioFileName,
        MA_SOUND_FLAG_STREAM, NULL, NULL, &mMusic);
    if(result != MA_SUCCESS) throw "failed to load music";
    ma_sound_set_looping(&mMusic, MA_TRUE);
    ma_sound_start(&mMusic);
    mMusicLoaded = true;
}

void AudioManager::stopMusic(){
    if(mMusicLoaded){
        ma_sound_stop(&mMusic);
        ma_sound_uninit(&mMusic);
        mMusicLoaded = false;
    }
}

void AudioManager::playSFX(const char *audioFileName){
    ma_result result;
    result = ma_engine_play_sound(&mEngine, audioFileName, NULL);
    if (result != MA_SUCCESS){
        throw "failed to play audio file";
    }
}