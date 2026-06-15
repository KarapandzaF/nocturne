#include "../includes/audio.h"
#include "../includes/miniaudio.h"

i32 audio_init(Audio *audio){
    ma_result result = ma_engine_init(NULL, &audio -> engine);

    if(result != MA_SUCCESS){
	fprintf(stderr, "Couldn't load Audio engine");
	return -1;
    }

    return 0;
}

void audio_play(Audio *audio, const char *path){
    ma_sound_init_from_file(&audio -> engine, path, 0, NULL, NULL, &audio -> sound);
    ma_sound_start(&audio -> sound);
    audio -> playing = true;
}

void audio_stop(Audio *audio){
    ma_sound_stop(&audio -> sound);
    ma_sound_uninit(&audio -> sound);
}

void audio_cleanup(Audio *audio){
    ma_sound_uninit(&audio -> sound);
    ma_engine_uninit(&audio -> engine);
}

static void audio_set_volume(Audio *audio, f32 volume){
    ma_sound_set_volume(&audio -> sound, volume);
}
void audio_change_volume(Audio *audio, i8 sign){
    f32 increment = 0 < sign ? 0.05 : -0.05;
    f32 current = ma_sound_get_volume(&audio -> sound);

    if((current + increment) < 0.0 ){
	audio_set_volume(audio, 0.0);
    }else if ((current + increment) > 1.0) {
	audio_set_volume(audio, 1.0);
    }else{
	audio_set_volume(audio, current + increment);
    }
}

void audio_pause(Audio *audio){
    if (audio -> playing == true){
	ma_sound_stop(&audio -> sound);
	audio -> playing = false;
    } else {
	ma_sound_start(&audio -> sound);
	audio -> playing = true;
    }
}

void audio_set_second(Audio *audio, i32 increment){
    f32 current;
    f32 length;

    ma_sound_get_cursor_in_seconds(&audio -> sound, &current);
    ma_sound_get_length_in_seconds(&audio -> sound, &length);
    f32 second = current + increment ;
    if(second > length){
	ma_sound_seek_to_second(&audio -> sound, length);
    }else{
	ma_sound_seek_to_second(&audio -> sound, second);
    }

}
