#ifndef APP_H
#define APP_H

#include <notcurses/notcurses.h>
#include <stdint.h>
#include <pthread.h>
#include <stdbool.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <string.h>
#include <taglib/tag_c.h>
#include <unistd.h>
#include <locale.h>
#include "dynamic_array.h"
#include "config.h"
#include "audio.h"
#include "track.h"

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef uint8_t u8;
typedef uint32_t u32;
typedef uint64_t u64;

#define FILEND(a) (strstr(entry -> d_name, a))

typedef struct{
    struct notcurses *nc;
    struct ncplane *stdplane;

    Audio audio;
    
    array_t tracks;
    array_t shuffle_queue;

    i32 selected;
    i32 last_selected;

    f32 cursor;
    f32 length;
    
    struct ncplane *imageplane;
    struct ncplane *artplane;
    struct ncvisual *ncv;

    pthread_mutex_t mutex;

    bool render_thread;
    bool needs_blit;
    bool playing;
    bool audio_lock;
    bool running;

    Config config;
} App;

i32 app_init(App *app);
void app_run(App *app);
void app_cleanup(App *app);
void app_play(App *app);
static void app_queue(App *app, i32 *array, size_t array_s);
static void app_shuffle(App *app);

static void *input_loop(void *arg);
void input_prompt(App *app, char *out, size_t size);
void input_handle(App *app);

static void scan_music(App *app, const char *directory);


#endif
