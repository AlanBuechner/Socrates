#include "Validate.h"
#include <filesystem>
#include <vector>
#include <iostream>

namespace fs = std::filesystem;

static std::vector<fs::path> paths;

bool hasPath(const fs::path &path) {
  for (auto &p : paths) {
    if (p == path)
      return true;
  }
  return false;
}

void ProcessDirectory(const fs::path &path) {
  for (auto &p : fs::directory_iterator(path)) {
    if (p.is_directory()) {
      ProcessDirectory(p.path());
    } else {
      fs::path filename = p.path().filename();
      if (!hasPath(filename))
        paths.push_back(filename);
    }
  }
}

void Validate() {
  ProcessDirectory("src");

  for (auto &p : fs::directory_iterator("generated")) {
    fs::path path = p.path();
    if (path.extension() == ".reflected") {
      if (!hasPath(path.filename().stem())) {
        fs::remove(path);
      }
    }
  }
}
