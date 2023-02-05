#include "Validate.h"
#include "ReflectionData.h"
#include "CodeGen.h"
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <queue>

namespace fs = std::filesystem;

int main(int argc, char **argv) {
  // validate all reflection files are present
  Validate();
  
  // get arguments
  std::string projectDir = argv[1];
  std::string sysIncludesStr = argv[2];
  std::string intLocation = argv[3];

  if (projectDir[projectDir.size() - 1] == '\\')
    projectDir = projectDir.substr(0, projectDir.size() - 1);

  std::string projectName = std::filesystem::path(projectDir).filename().string();
  std::string pchLocation = projectDir + "\\" + intLocation + projectName + ".pch";

  // get reflection data from files
  std::cout << "building reflection data" << std::endl;
  ReflectionData data = GetReflectionDataFromFolder("generated");

  // write reflection code
  fs::path gen = "generated/generated.cpp";
  WriteCode(gen, projectName, data);

  // compile reflection code
  //std::stringstream ss;

  //std::vector<std::string> isys;
  //uint32_t lastloc = 0;
  //for (uint32_t i = 0; i < sysIncludesStr.size(); i++) {
  //  if (sysIncludesStr[i] == ';') {
  //    isys.push_back(sysIncludesStr.substr(lastloc, i - lastloc));
  //    lastloc = i + 1;
  //  }
  //}
  //isys.push_back(sysIncludesStr.substr(lastloc, sysIncludesStr.size() - lastloc));

  //ss << "clang-cl.exe -cc1 -triple x86_64-pc-windows-msvc19.34.31935";
  //ss << " -x c++ -std=c++17";
  //ss << " -O2";
  //ss << " -emit-obj";
  //ss << " -include-pch " << pchLocation;
  //ss << " -mincremental-linker-compatible";
  //ss << " --mrelax-relocations";
  //ss << " -disable-free";
  //ss << " -clear-ast-before-backend";
  //ss << " -disable-llvm-verifier";
  //ss << " -discard-value-names";
  //ss << " -main-file-name generated/generated.cpp";
  //ss << " -mrelocation-model pic";
  //ss << " -pic-level 2";
  //ss << " -mframe-pointer=none";
  //ss << " -relaxed-aliasing";
  //ss << " -fmath-errno";
  //ss << " -ffp-contract=on";
  //ss << " -fno-rounding-math";
  //ss << " -mconstructor-aliases";
  //ss << " -funwind-tables=2";
  //ss << " -target-cpu x86-64";
  //ss << " -mllvm";
  //ss << " -x86-asm-syntax=intel";
  //ss << " -tune-cpu generic";
  //ss << " -mllvm";
  //ss << " -treat-scalable-fixed-error-as-warning";
  //ss << " -D_MT";
  //ss << " -flto-visibility-public-std";
  //ss << " --dependent-lib=libcmt";
  //ss << " --dependent-lib=oldnames";
  //ss << " -stack-protector 2";
  //ss << " -fdefault-calling-conv=cdecl";
  //ss << " -fdiagnostics-format msvc";
  //ss << " -gno-column-info";
  //ss << " -gcodeview";
  //ss << " -debug-info-kind=constructor";
  //ss << " -ffunction-sections";
  //ss << " -fcoverage-compilation-dir=\"" << projectDir << "\"";
  //ss << " -resource-dir C:\\Users\\abuechner\\source\\repos\\ubiq\\vendor\\Compiler\\lib\\clang\16";
  //ss << " -O2";
  //ss << " -Wall";
  //ss << " -Wno-error";
  //ss << " -fdeprecated-macro";
  //ss << " -fdebug-compilation-dir=\"" << projectDir << "\"";
  //ss << " -fno-use-cxa-atexit";
  //ss << " -fdelayed-template-parsing";
  //ss << " -fno-caret-diagnostics";
  //ss << " -vectorize-loops";
  //ss << " -vectorize-slp";
  //ss << " -faddrsig";
  //ss << " -ferror-limit 19";
  //ss << " -fcxx-exceptions";
  //ss << " -fexceptions";
  //ss << " -fexternc-nounwind";
  //ss << " -fms-volatile";
  //ss << " -fms-extensions";
  //ss << " -fms-compatibility";
  //ss << " -fms-compatibility-version=19.34.31935";
  //ss << " -I \"" << projectDir << "\"";
  //ss << " -I src";
  //ss << " -I vendor";
  //ss << " -I vendor\\spdlog\\include";
  //ss << " -I vendor\\Glad\\include";
  //ss << " -I vendor\\imgui";
  //ss << " -I vendor\\Glm";
  //ss << " -I vendor\\stb_image";
  //ss << " -I vendor\\entt\\include";
  //ss << " -I vendor\\yaml-cpp\\include";
  //ss << " -I vendor\\ImGuizmo";
  //ss << " -I vendor\\Box2D\\Include";
  //ss << " -I vendor\\Assimp\\include";
  //ss << " -I vendor\\json\\single_include";
  //ss << " -I vendor\\dxc\\inc";
  //ss << " -I vendor\\pix\\include";
  //ss << " -D _CRT_SECURE_NO_WARNINGS";
  //ss << " -D RELEASE";
  //ss << " -D _UNICODE";
  //ss << " -D UNICODE";
  //for (auto& s : isys)
  //  ss << " -internal-isystem \"" << s << "\"";
  //ss << " generated/generated.cpp -o generated/generated.obj";

  //std::cout << ss.str() << std::endl;

  //system(ss.str().c_str());

  //system("lld-link.exe generated/generated.obj generated/generated.lib");

  return 0;
}
