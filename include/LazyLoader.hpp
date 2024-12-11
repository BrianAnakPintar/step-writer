#ifndef LAZYLOADER_HPP
#define LAZYLOADER_HPP

#include "string"
#include "File.hpp"

class LazyLoader {
public:
  LazyLoader(const std::string& path);
  void LoadChildren(std::vector<File*>& children);

private:
  std::string file_path;
  bool loaded;
};

#endif // !LAZYLOADER_HPP
