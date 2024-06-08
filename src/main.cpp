#include <iostream>
#include "Editor.h"

int main(int argc, char *argv[]) {
  Editor& editor = Editor::GetInstance();
  editor.StartApplication();
  std::cout << "Good Bye!" << std::endl;
  return 0;
}

