#include "../includes/app.h"

int main(){
    App app;

    app_init(&app);
    audio_init(&app.audio);
    app_run(&app);
    app_cleanup(&app);

    return 0;
}

