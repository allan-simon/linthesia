linthesia
=========

tentative to fix/rewrite linthesia , an open-source synthesia/ "piano-hero" like game

##Note:

Current Master can be used for casual gaming, and I strongly encourage people to use it
to provide feedback

if you want to find the old linthesia checkout the release 0.4.3 (see release section in github)

I'm very open to suggestions/ideas, and should be quite responsive to issues which are posted here
So if you get any problem to compile and/or you want to see some specific feaures, just add an issue
here or send me an email  allan.simon |at| supinfo.com

##Dependencies

For version 0.4.3 on ubuntu you will need to install the following packages

     libgtkmm-2.4-dev
     libgconfmm-2.6-dev
     libgtkglextmm-x11-1.2-dev
     libasound2-dev

For version 0.5 and above (including current master) you simply need

     libsfml-dev
     librtmidi-dev

we also use [tinydir](https://github.com/cxong/tinydir), for cross-plateform file/directory
operation but as a header so no need to install anything


##Compiling it (version 0.5 and above)

    autoreconf -i
    ./configure
    make

##Launching it

    cd src ; ./linthesia

## In game control

 * space to play/pause the song
 * left/right to decrease/increase the song's speed
 * escape to go back to previous screen (for example to leave a game)

(latter we will make it more obvious inside the game itself)
