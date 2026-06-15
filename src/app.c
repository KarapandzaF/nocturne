#include "../includes/app.h"
#include "../includes/ui.h"

i32 app_init(App *app){

    memset(app, 0, sizeof(App));

    setlocale(LC_ALL, "");

    notcurses_options ncopt;
    memset(&ncopt, 0, sizeof(ncopt));

    app -> nc = notcurses_init(&ncopt, stdout);

    if(!app -> nc){
	fprintf(stderr, "Failed to init ncstruct \n");
    }

    app -> stdplane = notcurses_stdplane(app -> nc);

    if(!app -> stdplane){
	fprintf(stderr, "Failed to init stdplane \n");
    }

    if(config_exsist()){
	config_read(&app -> config);
    }else{
	char path[512];
	input_prompt(app, path, sizeof(path));
	config_create(&app -> config, path);
    }

    app -> last_selected = -1;

    app -> running = 1;

    app -> tracks = create_array(10);


    return 1;
}

#if defined(__APPLE__) || defined(__linux__)
void app_run(App *app){

    scan_music(app, app -> config.music_dir);

    pthread_t input_thread;
    pthread_create(&input_thread, NULL, input_loop, app);

    while(app -> running){

	draw_ui(app);

	notcurses_render(app -> nc);

	app_play(app);

	usleep(16000);
    }

    pthread_join(input_thread, NULL);


}

#elif defined(_WIN32)
#include <windows.h>

void app_run(App *app){

    scan_music(app, app -> config.music_dir);

    pthread_t input_thread;
    pthread_create(&input_thread, NULL, input_loop, app);

    while(app -> running){

	draw_ui(app);

	notcurses_render(app -> nc);

	app_play(app);

	Sleep(16);
    }

    pthread_join(input_thread, NULL);


}
#endif

void app_cleanup(App *app){

    audio_cleanup(&app -> audio);
    delete_array(&app -> tracks);
    notcurses_stop(app->nc);

}

static void *input_loop(void *arg){

    App *app = (App *)arg;
    while(app->running){
        input_handle(app);
    }
    return NULL;
}

void input_handle(App *app){

    struct ncinput ni;

    u32 c = notcurses_get(app -> nc, 0, &ni);
    if(ni.evtype == NCTYPE_RELEASE) { return; }

    switch(c){
	case 'q':
	    app -> running = false;
	    break;
	
	case NCKEY_UP:
	case 'k':
	    if(app -> selected > 0 && app -> playing == false){
		app -> selected--;
	    }
	    break;

	case NCKEY_DOWN:
	case 'j':
	    if(app -> selected < app -> tracks.count - 1 && app -> playing == false){
		app -> selected++;
	    }
	    break;
	case '\t':
	    pthread_mutex_lock(&app -> mutex);
	    app -> needs_blit = true;
	    app -> playing = false;
	    app -> audio_lock = false;
	    pthread_mutex_unlock(&app -> mutex);
	    audio_stop(&app -> audio);
	    break;
	case 'a':
	    app -> playing = true; 
	    break;
	case NCKEY_RETURN:
	    app -> playing = true; 
	    break;
	case 'p':
	    audio_pause(&app -> audio);
	    break;
	case '=':
	    audio_change_volume(&app -> audio, 1);
	    break;
	case '-':
	    audio_change_volume(&app -> audio, -1);
	    break;
	case NCKEY_LEFT:
	    audio_set_second(&app -> audio, -5);
	    break;
	case NCKEY_RIGHT:
	    audio_set_second(&app -> audio, 5);
	    break;
    default: ;
    }
}

static void scan_music(App *app, const char *directory){

    DIR *dir= opendir(directory);

    if(!dir){
	perror("opendir");
	return;
    }

    struct dirent *entry;

    while((entry = readdir(dir))){

	if(strcmp(entry -> d_name, ".") == 0 || strcmp(entry -> d_name, "..") == 0) { continue; }

	char path[1024];

	snprintf(path, sizeof(path), "%s/%s", directory, entry -> d_name);
	
	if(entry -> d_type == DT_DIR){

	    scan_music(app, path);

	    continue;
	}

	if(entry -> d_type != DT_REG){
	    continue;
	}

	if(!FILEND(".mp3") && !FILEND(".flac")) { continue; }

	Track track;

	strncpy(track.path, path, sizeof(track.path) - 1);
	track.path[sizeof(track.path) - 1] = '\0';

	TagLib_File *file = taglib_file_new(track.path);
	if(!file) continue;

	TagLib_Tag *tag = taglib_file_tag(file);

	const char *title = taglib_tag_title(tag);
	const char *artist = taglib_tag_artist(tag);
	const char *album = taglib_tag_album(tag);

	if(title){
	    strncpy(track.title, title, sizeof(track.title) - 1);
	    track.title[sizeof(track.title) - 1] = '\0';
	}else{
	    track.title[0] = '\0';
	}

	if(artist){
	    strncpy(track.artist, artist, sizeof(track.artist) - 1);
	    track.artist[sizeof(track.artist) - 1] = '\0';
	}else{
	    track.artist[0] = '\0';
	}


	if(album){
	    strncpy(track.album, album, sizeof(track.album) - 1);
	    track.album[sizeof(track.album) - 1] = '\0';
	}else{
	    track.album[0] = '\0';
	}

	append(&app -> tracks, track);

	taglib_free(file);
    }

    closedir(dir);

}

void app_play(App *app){

    if(app -> playing == true && app -> audio_lock == false){
	app -> needs_blit = true;
	Track *track = &app -> tracks.items[app -> selected];
	audio_play(&app ->audio, track -> path);

	app -> audio_lock = true;
    }else if(app -> playing == true && ma_sound_at_end(&app -> audio.sound)){
	ma_sound_uninit(&app -> audio.sound);
	if(app -> selected < app -> tracks.count - 1){ app -> selected++; }
	else { app -> selected = 0; }
	app -> audio_lock = false;
    }
}

void input_prompt(App *app, char *out, size_t out_size){
    char buf[512] = {0};
    int len = 0;
    struct ncinput ni;

    while(1){
	ncplane_erase(app -> stdplane);
	ncplane_putstr_yx(app -> stdplane, 1, 1, "enter your music directory:");
	ncplane_putstr_yx(app -> stdplane, 2, 1, buf);
	notcurses_render(app -> nc);

	u32 c = notcurses_get(app -> nc, 0, &ni);
	if(ni.evtype == NCTYPE_RELEASE) { continue; }

	if(c == NCKEY_RETURN){
	    snprintf(out, out_size, "%s", buf);
	    break;
	}else if(c == NCKEY_BACKSPACE || c == 127){
	    if(len > 0){
		buf[--len] = '\0';
	    }
	}else if(c >= 32 && c <= 127){

	    if(len < (int)out_size - 1){
		buf[len++] = (char)c;
		buf[len] = '\0';
	    }
	}
    }
}

