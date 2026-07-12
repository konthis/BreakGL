#include "audio_manager.hpp"

AudioManager::AudioManager(){
    ma_result result;

    result = ma_engine_init(NULL, &mEngine);
    if (result != MA_SUCCESS) {
        throw "failed to load audio engine";
    }

    ma_sound_init_from_file(&mEngine, SFX_BOUNCE, 0, NULL, NULL, &mSfxBounce);
    ma_sound_init_from_file(&mEngine, SFX_SPAWN_BALL, 0, NULL, NULL, &mSfxSpawnBall);
    ma_sound_init_from_file(&mEngine, SFX_WIDE_PLATFORM, 0, NULL, NULL, &mSfxWidePlatform);
    ma_sound_init_from_file(&mEngine, SFX_TEST, 0, NULL, NULL, &mSfxTest);

}

AudioManager::~AudioManager(){
    if(mMusicLoaded){
        ma_sound_uninit(&mMusic);
    }
    ma_sound_uninit(&mSfxBounce);
    ma_sound_uninit(&mSfxWidePlatform);
    ma_sound_uninit(&mSfxSpawnBall);
    ma_sound_uninit(&mSfxTest);
    ma_engine_uninit(&mEngine);
}
   
void AudioManager::playMusic(const char* audioFileName){
    if(mMusicLoaded){
        ma_sound_uninit(&mMusic);
    }
    ma_result result = ma_sound_init_from_file(&mEngine, audioFileName,
        MA_SOUND_FLAG_STREAM, NULL, NULL, &mMusic);
    if(result != MA_SUCCESS) throw "failed to load music";
    ma_sound_set_looping(&mMusic, MA_TRUE);
    // delay, 
    ma_uint64 startTime = ma_engine_get_time_in_milliseconds(&mEngine) + 400;
    ma_sound_set_start_time_in_milliseconds(&mMusic, startTime);
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

void AudioManager::playSFX(const unsigned int &sfx){
    switch(sfx){
        case 0:{
            ma_sound_seek_to_pcm_frame(&mSfxBounce, 0);
            ma_sound_start(&mSfxBounce);
            break;
        }
        case 1:{
            ma_sound_seek_to_pcm_frame(&mSfxSpawnBall, 0);
            ma_sound_start(&mSfxSpawnBall);
            break;
        }
        case 2:{
            ma_sound_seek_to_pcm_frame(&mSfxWidePlatform, 0);
            ma_sound_start(&mSfxWidePlatform);
            break;
        }
        case 3:{
            ma_sound_seek_to_pcm_frame(&mSfxTest, 0);
            ma_sound_start(&mSfxTest);
            break;
        }
    }
}

void AudioManager::setVolumes(float music, float sfx){
    if(mMusicLoaded) ma_sound_set_volume(&mMusic, music);
    ma_sound_set_volume(&mSfxBounce, sfx);
    ma_sound_set_volume(&mSfxSpawnBall, sfx);
    ma_sound_set_volume(&mSfxWidePlatform, sfx);
    ma_sound_set_volume(&mSfxTest, sfx);
}