PEAL
====

A NES remake of the TI-99/4a game.

### Compiling

* Download the latest cc65 from http://cc65.github.io/cc65/.

* Place cc65 in ../ relative from this source code.

* run `compile.bat` to produce the `peal.nes` file.

### Running

* Run the `peal.nes` using a NES emulator (usually in the file open menu). It has been tested with [FCEUX](http://www.fceux.com/) and [Nestopia](http://nestopia.sourceforge.net/).

* If you want to use the `start_peal.bat` file to start the game: Run the `peal.nes` using a NES emulator. Set the default program to either [FCEUX](http://www.fceux.com/) or [Nestopia](http://nestopia.sourceforge.net/) (depends on which program you use). After that, you can always double-click `start_peal.bat` to start the game. (The reason why `start_peal.bat` pings 127.0.0.1 is because we want to wait so `peal.nes` actually contains the new code).

