#ifndef UI_H
#define UI_H

#include "app.h"

void draw_ui(App *app);

static void render_text(struct ncplane *n, u32 stylebits, i32 RGB,  i32 y, i32 x, const char *text);
static void destroy_format(struct ncplane *n);
static void render_bar(App *app, i32 length, i32 counter, i32 y, i32 x);
static void draw_volume(App *app, i32 barlen, i32 y, i32 x);
static void draw_progress(App *app, i32 barlen, i32 y, i32 x);
static void render_text_bar(App *app, i32 barlen, const char *string, i32 y, i32 x);
static void render_selection(App *app, i32 artrows, i32 artcols);
void *render_thread(void *arg);

#endif
