#include "../includes/ui.h"
#include <pthread.h>

void draw_ui(App *app){

    u32 termcols, termrows;
    ncplane_dim_yx(app -> stdplane, &termrows, &termcols);

    i32 artrows = termrows * 0.6;
    i32 artcols = artrows * 2;

    i32 align = artcols + 15;
    
    ncplane_erase(app->stdplane);

    render_text(app -> stdplane, NCSTYLE_ITALIC, 0x2E5BA1, 0, 1, "nocturne_");
    destroy_format(app -> stdplane);

    // if nothing is playing the menu should be displayed
    if(app -> playing == false){
	// cleanup imageplane if still present alsoe destroy the style from having something playing
	if(app -> imageplane){
	    ncplane_destroy(app -> imageplane);
	    app -> imageplane = NULL;

	}

	//check if update in selection and spawn thread to render
	if(app -> last_selected != app -> selected){
	    app -> last_selected = app -> selected;
	    app -> needs_blit = true;

	    pthread_mutex_lock(&app -> mutex);
	    if(app -> ncv){
		ncvisual_destroy(app -> ncv);
		app -> ncv = NULL;
	    }
	    pthread_mutex_unlock(&app -> mutex);

	    if(!app -> render_thread){
		app -> render_thread = true;
		pthread_t t;
		pthread_create(&t, NULL, render_thread, app);
		pthread_detach(t);
	    }

	}

	//create plane if it doesnt exsist
	if(app -> artplane == NULL){
	    struct ncplane_options iopts = {
		.y = 1,
		.x = 50,
		.rows = artrows,
		.cols = artcols,
	    };

	    app -> artplane = ncplane_create(app -> stdplane, &iopts);
	}

	//render artwork
	pthread_mutex_lock(&app -> mutex);
	if(app -> needs_blit && app -> ncv){

	    struct ncvisual_options vopts = {
		.y = 1,
		.x = 1,
		.n = app -> artplane,
		.scaling = NCSCALE_SCALE,
		.blitter = NCBLIT_2x1,
	    }; 
	    if(app -> ncv){
		ncvisual_blit(app -> nc, app -> ncv, &vopts);
	    }
	    app -> needs_blit = false;

	}
	pthread_mutex_unlock(&app -> mutex);

	//render nonselected tracks
	for(i32 i = 0; i < app -> tracks.count; i++){
	    if(i != app -> selected){
		Track *track = &app -> tracks.items[i];
		ncplane_putstr_yx(app -> stdplane, i + 1, 2, track -> title);
	    }
	}

	//render selected track + info
	Track *track = &app -> tracks.items[app -> selected];
	render_text(app -> stdplane, NCSTYLE_ITALIC, 0xFFFFFF, app -> selected + 1, 4,  track -> title);
	render_text(app -> stdplane, NCSTYLE_BOLD, 0xFFFFFF, 20, (artcols * 2 - 21), track -> title);
	render_text(app -> stdplane, NCSTYLE_NONE, 0xFFFFFF, 21, (artcols * 2 - 21), track -> artist);
	render_text(app -> stdplane, NCSTYLE_ITALIC, 0xFFFFFF, 22, (artcols * 2 - 21), track -> album);


	destroy_format(app -> stdplane);
    }

    //if something is playing display mode while playing 
    if (app -> playing == true){
	
	if(app -> artplane){
	    ncplane_destroy(app -> artplane);
	    app -> artplane = NULL;

	}
	Track *track = &app -> tracks.items[app -> selected];


	// only render cover art once
	if(app -> needs_blit){
	    //only puly image from file if there has been an update.
	    if(app -> last_selected != app -> selected){
		ncvisual_destroy(app -> ncv);
		struct ncvisual *ncv = ncvisual_from_file(track -> path);
		app -> ncv = ncv;
	    }else{
		app -> last_selected = app -> selected;
	    }

	    if(app -> ncv){
		app -> needs_blit = false;

		if(app -> imageplane == NULL){
		    struct ncplane_options iopts = {
			.y = 1,
			.x = 2,
			.rows = artrows,
			.cols = artcols,
		    };

		    app -> imageplane = ncplane_create(app -> stdplane, &iopts);
		    app -> needs_blit = true;
		}
	    

		struct ncvisual_options vopts = {
		    .y = 1,
		    .x = 1,
		    .n = app -> imageplane,
		    .scaling = NCSCALE_SCALE,
		    .blitter = NCBLIT_2x1,
		}; 

		ncvisual_blit(app -> nc, app -> ncv, &vopts);
		app -> needs_blit = false;
	    }else{
		app -> needs_blit = false;
		ncplane_putstr_yx(app -> stdplane, app -> tracks.count + 1, 10, "No Album Cover present");
	    }
	}
	//place all the important text and render icons should string not be empty
	render_text(app -> stdplane, NCSTYLE_BOLD, 0xFFFFFF, 4, align, track -> title);
	if(track -> title[0] != '\0'){
	    ncplane_putstr_yx(app -> stdplane, 4, align - 2, "♫ ");
	}
	render_text(app -> stdplane, NCSTYLE_NONE, 0xB4B4B4, 6, align, track -> artist);
	if(track -> artist[0] != '\0'){
	    ncplane_putstr_yx(app -> stdplane, 6, align - 2, "☾ ");
	}
	render_text(app -> stdplane, NCSTYLE_ITALIC, 0xA0C9D9, 7, align, track -> album);
	if(track -> album[0] != '\0'){
	    ncplane_putstr_yx(app -> stdplane, 7, align - 2,  "✦ ");
	}

	destroy_format(app -> stdplane);

	draw_volume(app, 20, 10, align);
	draw_progress(app, 40, 14, align);

    }


}

static void render_text(struct ncplane *n, u32 stylebits, i32 RGB, i32 y, i32 x, const char *text){

    ncplane_set_styles(n, stylebits);
    ncplane_set_fg_rgb(n, RGB);
    ncplane_putstr_yx(n, y, x, text);

}

static void destroy_format(struct ncplane *n){
    
    ncplane_set_styles(n, NCSTYLE_NONE);
    ncplane_set_fg_default(n);
    ncplane_set_bg_default(n);

}



static void render_bar(App *app, i32 barlen, i32 counter, i32 y, i32 x){

    ncplane_putstr_yx(app -> stdplane, y, x, "▕");
    ncplane_putstr_yx(app -> stdplane, y, x + barlen, "▏");

    for (int i = 1; i < barlen; i++){
	ncplane_putstr_yx(app->stdplane, y + 1, x + i, "▔");
	ncplane_putstr_yx(app -> stdplane, y - 1, x + i, "▁");
	if( i < counter ){
	    ncplane_putstr_yx(app -> stdplane, y, x + i, "█");
	}
    }
}

static void render_text_bar(App *app, i32 barlen, const char *string, i32 y, i32 x){
    
    i32 len = strlen(string);
    ncplane_putstr_yx(app->stdplane, y-1, x + ((barlen - len) / 2), string);

}

static void draw_volume(App *app, i32 barlen, i32 y, i32 x){

    f32 volume = ma_sound_get_volume(&app -> audio.sound);
    i32 filled = (i32)(volume * barlen);

    ncplane_set_fg_rgb(app -> stdplane, 0x666666);
    render_bar(app, 20, filled, y, x);

    char volumestr[32];
    snprintf(volumestr, sizeof(volumestr), " %d%% ", ((int) (volume * 100)));
    ncplane_putstr_yx(app -> stdplane, y, x - 2, "⋊");

    render_text_bar(app, barlen, volumestr, y, x);

    destroy_format(app -> stdplane);
}


static void draw_progress(App *app, i32 barlen, i32 y, i32 x){

    ma_sound_get_cursor_in_seconds(&app -> audio.sound, &app -> cursor);
    ma_sound_get_length_in_seconds(&app -> audio.sound, &app -> length);

    i32 filled = (int)(app -> cursor / app -> length * barlen);

    ncplane_set_fg_rgb(app -> stdplane, 0xFFFFFF);

    render_bar(app, barlen, filled, y, x);

    char timestr[32];

    snprintf(timestr, sizeof(timestr), " %d:%02d / %d:%02d ",
        (i32)app -> cursor / 60, (i32)app -> cursor % 60,
        (i32)app -> length / 60, (i32)app -> length % 60);

    render_text_bar(app, barlen, timestr, y, x);
    
    destroy_format(app -> stdplane);

    if (ma_sound_is_playing(&app -> audio.sound) == MA_TRUE) {
	ncplane_putstr_yx(app -> stdplane, y, x - 2, "⏸");
    }else{
	ncplane_putstr_yx(app -> stdplane, y, x - 2, "▶");
    }
}


void *render_thread(void *arg){
    App *app = (App *) arg;

    pthread_mutex_lock(&app -> mutex);
    Track *track = &app -> tracks.items[app -> selected];
    pthread_mutex_unlock(&app -> mutex);

    struct ncvisual *vcv = ncvisual_from_file(track -> path);

    pthread_mutex_lock(&app -> mutex);
    app -> ncv = vcv;
    app -> needs_blit = true;
    app -> render_thread = false;
    pthread_mutex_unlock(&app -> mutex);

    return NULL;
}

