# STEP-Writer
### ~~Shared~~ Scuffed Text Editor Program - Writer.
#### A C++ TUI Text Editor 
Yeah, I'm proud of my acronyms.
___

# Overview.
Step-Writer is a TUI text editor. Which *simulates* vim motions and other customizable features. 
I am doing this because the remote servers at UBC does not have neovim installed... So why not make my own text editor.

**Implemented Features:**
- Selectable title screen logos!
- `h`, `j`, `k`, `l` movements in normal mode.
- Enter insert mode by pressing `i`.
- Press `enter` on the file in the explorer in order to open the file.
- Press `q` to return to main menu. (Probably should change that...)
- You can add your own features by going into the `TextFileViewer.cpp` file.

![](Docs/Overview.gif)

The project uses FTXUI for the terminal UI components.

# Installation.
1. Clone the project
2. Once cloned, create a `build` directory by typing `mkdir build` and go to that directory by running `cd build`
3. Next, run `cmake ..`
4. Make the program by running `make`
5. Finally, run the program `./step-writer` from the build directory.

### Todo (soo many things to do ;-;)
1. Better file view hierarchy enabling users to see project structures and directory contents.
2. Fix simple line deletion bugs.
3. Proper cursor movement.
4. Syntax Highlighting.
5. REFACTOR, REFACTOR, REFACTOR! So many cleanup to do 😭