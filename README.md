PEAL
====

A NES remake of the TI-99/4a game.

### Compiling

* Download the latest cc65 from http://cc65.github.io/cc65/.

* Place cc65 in ../ relative from this source code.

* Of course you have Ruby installed. No? On windows use the [installer](http://rubyinstaller.org/), otherwise the fantastic [rvm](http://rvm.io/).

* Install rake on the command line: `gem install rake`.

* type `rake` at the command line and see it build a beautiful `peal.nes`.

### Running

* Run the `peal.nes` using a NES emulator (usually in the file open menu). It has been tested with [FCEUX](http://www.fceux.com/) and [Nestopia](http://nestopia.sourceforge.net/).

* If you want to use the `start_peal.bat` file to start the game: Run the `peal.nes` using a NES emulator. Set the default program to either [FCEUX](http://www.fceux.com/) or [Nestopia](http://nestopia.sourceforge.net/) (depends on which program you use). After that, you can always double-click `start_peal.bat` to start the game. (The reason why `start_peal.bat` pings 127.0.0.1 is because we want to wait so `peal.nes` actually contains the new code).
