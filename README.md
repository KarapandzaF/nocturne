# nocturne: bringing music to your terminal

**nocturne** is a simple crossplatform music TUI written in C to bring flac and mp3 playing funcitonality directly into the terminal. It utilizes [notcurses](https://github.com/dankamongmen/notcurses/tree/master) for it's rendering and [miniaudio](https://github.com/mackron/miniaudio) for playing audio. After selecting a path for your music you can jump straight into the main menu.

Nocturne is a (maybe) fast and perfromative music player giving you the bare minimum to play high quality music files from your own curated selection of files.

![Alt text](/assets/main_menu.png)
![Alt text](/assets/menu_playing.png)

## Usage:
Launch nocturne in your terminal by simply typing `nocturne`. After launching  for the first time you will be prompted to provide a directory where nocturne will search for your music. This is thens saved into a config which lives at `.config/nocturne/nocturne.conf` on UNIX systems and `AppData\nocturne\nocturne.conf` on windows and can always be edited from there. 

After nocturne scans through folders and files in a directory you can scroll through the list using vim style motions with `j and k` and play a song using `enter or a`. For the rest of keys consult the Key guide.

### Key guide:

| Key         | Action                      |
|-------------|-----------------------------|
| a / enter   | Play hovered music file     |
| tab         | Exit music file             |
| q           | Quit the application        |
| j           | Move down in main menu      |
| k           | Move up in main menu        |
| p           | Pause/ unpause music        |
| -           | Turn down volume by 5%      |
| =           | Turn up volume by 5%        |
| Left arrow  | Skip foward by 5 seconds    |
| Right arrow | Skip backwards by 5 seconds |


## Install:
```
mkdir build && cd build
cmake ..
cmake --build
sudo cmake --install
```

### Notes:
This is a personal projected that I made in my free time and shouldn't be taken super seriously. This was my first attempt at coding anything semi seriously in C so alot of this code probably isn't the best but hey I had fun doing it.