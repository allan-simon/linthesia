linthesia
=========

tentative to fix/rewrite linthesia , an open-source synthesia/ "piano-hero" like game

##Note:

Current Master does not provide for the moment a usable game, if you want to compile it to play
or to make a small modification, checkout the release 0.4.3 (see release section in github)

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


##Compiling it (version 0.5 and above)

    autoreconf -i
    ./configure
    make

##Launching it

    ./src/linthesia  PATH_TO_A_MIDI_FILE
