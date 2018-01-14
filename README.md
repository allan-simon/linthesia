linthesia
=========

tentative to fix/rewrite linthesia , an open-source synthesia/ "piano-hero" like game
(scroll down to see screenshot and "manual")

it should compile for at least Windows/Linux/MacOS X (I have tested only Linux though)

## Note:

Current Master can be used for casual gaming, and I strongly encourage people to use it
to provide feedback

if you want to find the old linthesia checkout the release 0.4.3 (see release section in github)

## Contributing 

Even though this project hasn't moved a lot for a while, I'm still around to merge things :)

I'm very open to suggestions/ideas, and should be quite responsive to issues which are posted here
So if you get any problem to compile and/or you want to see some specific feaures, just add an issue
here or send me an email  allan.simon |at| supinfo.com


## Dependencies
For version 0.5 and above (including current master) you simply need these package (on Ubuntu)


     clang (or g++)
     libtool
     autoconf
     automake
     make

     libsfml-dev
     librtmidi-dev
     
So for example (with build-dependencies):
     
     sudo aptitude install clang libtool autoconf automake make libsfml-dev librtmidi-dev libgtkmm-2.4-dev libgconfmm-2.6-dev libgtkglextmm-x11-1.2-dev libasound2-dev

we also use [tinydir](https://github.com/cxong/tinydir), for cross-plateform file/directory
operation but as a header so no need to install anything

you may also need to install Timidiy or FluidSynth on Linux in order to have something to play out the midi

## Compiling it (version 0.5 and above)

    autoreconf -i
    ./configure
    make

## Launching it

    cd src ; ./linthesia

## In game control

 * space to play/pause the song
 * left/right to decrease/increase the song's speed
 * escape to go back to previous screen (for example to leave a game)

(latter we will make it more obvious inside the game itself)

## Screenshots 

![main page](https://raw.githubusercontent.com/allan-simon/linthesia/master/docs/screenshots/main.png)
![game](https://raw.githubusercontent.com/allan-simon/linthesia/master/docs/screenshots/game.png)
![choose song](https://raw.githubusercontent.com/allan-simon/linthesia/master/docs/screenshots/choose_song.png)
