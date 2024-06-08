<h1 align="center"> STEP-WRITER </h1>

<h3 align="center">🤓 Shared Text Editor Program - Writer.<br/>
A Lightweight & Simple C++ TUI Text Editor. </h3>
<p align="center">
<img src="Docs/ThemesStacked.png" alt="Simple Images" title="Simple Image" width="700"/>
</p>

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
- Syntax Highlighting using Tree Sitter. (Will improve)
- Saving file. `ctrl + p` saves and quits.
- File Explorer navigation with collapsible tabs.
- Status Bar indicating the mode (INSERT, NORMAL, VISUAL)
<p align="center">
<img src="Docs/Screenshot.png" alt="Simple showcase Gif" title="Simple Image" width="700"/>
<p align="center">Figure 1. Overview of the project using Wezterm</p>
</p>

I'll try to update this [blog](https://www.brianmoniaga.com/Blog/posts/stepwriter/) which should contain a better documentation and progress of the state of the application. (Though, I have a bad schedule of updating said site.)

# Installation.
1. Clone the project
2. Once cloned, create a `build` directory by typing `mkdir build` and go to that directory by running `cd build`
3. Next, run `cmake ..`
4. Make the program by running `make`
5. Finally, run the program `./step-writer` from the build directory.

Note: I haven't tested how to build in the recent version as I had to manually install tree-sitter. Hopefully the process remains simple for the user. 😔

### Todo (soo many things to do ;-;)
- [x] Better file view hierarchy enabling users to see project structures and directory contents.
- [x] Fix simple line deletion bugs.
- [ ] A working terminal.
- [ ] Better cursor movement.
- [ ] Syntax Highlighting (Sort of).
  - [x] Basic syntax highlighting.
  - [ ] Implement colors for ALL types.
  - [ ] Update syntax tree when user inputs a line.
- [ ] Use a ~~rope~~ gap buffer data structure.
- [ ] Search functionality
- [ ] `ctrl + c` and `ctrl + v` functionality. The OP copy + paste
- [ ] REFACTOR, REFACTOR, REFACTOR! So many cleanup to do 😭
- [ ] Custom keybindings
- [ ]  Settings menu
- [ ]  `ctrl + z` UNDO functionality.

# Acknowledgements
The project uses [FTXUI](https://github.com/ArthurSonzogni/FTXUI) for the terminal UI components, 
it is a very cool TUI library that anyone planning to make a TUI application in C++ should consider using. Aside from that it also uses [Tree-Sitter](https://github.com/tree-sitter/tree-sitter) which is an incremental parsing system that I use for syntax highlighting.

# OLD DEMOS
<p align="center">
<img src="Docs/Overview.gif" alt="Simple showcase Gif" title="Simple Showcase Gif" width="700"/>
<p align="center">Overview of the project using Warp</p>
</p>
<p align="center">
<img src="Docs/Warp-StepWriter.png" alt="Simple showcase Gif" title="Simple Showcase Gif" width="700"/>
<p align="center">Overview of the project using Warp</p>
</p>