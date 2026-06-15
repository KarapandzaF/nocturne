#ifndef AUDIO_H
#define AUDIO_H

#include "miniaudio.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef uint8_t u8;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

typedef struct{
    ma_engine engine; 
    ma_sound sound;
    bool playing;
} Audio;

i32 audio_init(Audio *audio);
void audio_play(Audio *audio, const char *path);
void audio_cleanup(Audio *audio);
void audio_stop(Audio *audio);
void audio_pause(Audio *audio);
static void audio_set_volume(Audio *audio, f32 volume);
void audio_change_volume(Audio *audio, i8 sign);
void audio_set_second(Audio *audio, i32 increment);

#endif
