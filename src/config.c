#include "../includes/config.h"

#if defined(__APPLE__) || defined(__linux__)

#include <sys/stat.h>
#include <unistd.h>

void config_create(Config *config, const char *path){
    const char *home_dir = getenv("HOME");

    char directory[512];
    snprintf(directory, sizeof(directory), "%s/.config/nocturne", home_dir);

    mkdir(directory, 0755);
    char filepath[512];

    snprintf(filepath, sizeof(filepath), "%s/nocturne.conf", directory);
    snprintf(config -> music_dir, sizeof(config -> music_dir), "%s", path);

    FILE *file = fopen(filepath, "a");
    if(!file) {return;}

    fseek(file, 0, SEEK_END);
    if(ftell(file) == 0){
        fprintf(file, "music_dir = %s", path);
    }

    fclose(file);
}

bool config_exsist(){

    const char *home_dir = getenv("HOME");
    char filepath[512];
    snprintf(filepath, sizeof(filepath), "%s/.config/nocturne/nocturne.conf", home_dir);

    if(access(filepath, F_OK) == 0){
	return true;
    }else{
	return false;
    }

}

void config_read(Config *config){

    const char *home_dir = getenv("HOME");
    char filepath[512];
    snprintf(filepath, sizeof(filepath), "%s/.config/nocturne/nocturne.conf", home_dir);

    FILE *file = fopen(filepath, "r");
    if(!file) { return; }
    char conf_content[512];
    char local_music_dir[512];

    fgets(conf_content, sizeof(conf_content), file);
    sscanf(conf_content, "music_dir = %s", local_music_dir);

    snprintf(config -> music_dir, sizeof(config -> music_dir), "%s", local_music_dir);

    fclose(file);

}

#elif defined(_WIN32)


void config_create(Config *config, const char *path){
    const char *app_data = getenv("APPDATA");

    char directory[512];
    snprintf(directory, sizeof(directory), "%s\\nocuturne", app_data);

    mkdir(directory, 0755);
    char filepath[512];

    snprintf(filepath, sizeof(filepath), "%s\\nocturne.conf", directory);
    snprintf(config -> music_dir, sizeof(config -> music_dir), "%s", path);

    FILE *file = fopen(filepath, "a");
    if(!file) {return;}

    fseek(file, 0, SEEK_END);
    if(ftell(file) == 0){
        fprintf(file, "music_dir = %s", path);
    }

    fclose(file);
}

bool config_exsist(){

    const char *app_data = getenv("APPDATA");
    char filepath[512];
    snprintf(filepath, sizeof(filepath), "%s\\nocturne\\nocturne.conf", app_data);

    if(access(filepath, F_OK) == 0){
	return true;
    }else{
	return false;
    }

}

void config_read(Config *config){

    const char *app_data = getenv("APPDATA");
    char filepath[512];
    snprintf(filepath, sizeof(filepath), "%s/nocturne/nocturne.conf", app_data);

    FILE *file = fopen(filepath, "r");
    if(!file) { return; }
    char conf_content[512];
    char local_music_dir[512];

    fgets(conf_content, sizeof(conf_content), file);
    sscanf(conf_content, "music_dir = %s", local_music_dir);

    snprintf(config -> music_dir, sizeof(config -> music_dir), "%s", local_music_dir);

    fclose(file);

}

#endif


