# Quick Start
Automatic installation  
NOTE: only tested on Archlinux
```bash
chomd +x install.sh uninstall.sh     # make scripts executable 
sudo ./install.sh                    # run install script
tictactoe                            # play game with new command
```
Uninstall
```bash
sudo ./uninstall.sh
```

# Manual Installation
```bash
gcc -Werror -Wall -std=c99 main.c tictactoe.c -lncurses -o tictactoe
./tictactoe                         # test application
sudo cp tictactoe /usr/local/bin    # or /usr/bin depending on distro
tictactoe                           # play game with new command
```

Created by David Vella, November 2019
