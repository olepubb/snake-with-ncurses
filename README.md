# snake-with-ncurses
Terminal snake game using ncurses and C.
<br />
<p align="center">
<img src="https://media0.giphy.com/media/BBEr9OREh4kLj8YKPN/giphy.gif" alt="Game gif"/>
</p>



## Build
Clone the repository.

```
git clone https://github.com/olepubb/snake-with-ncurses.git
```

Change the compiler in the Makefile to use your C compiler (uses clang by default).

```
CC = your_compiler
```

Build the binary.
```
make
```
I made this on a mac that has ncurses installed by default, if for some reason you dont have it you will need to get it somehow.

## Controls
Arrow keys for movment.\
'e' to quit the game.

