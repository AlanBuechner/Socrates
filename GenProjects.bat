cd build
cmake -DLLVM_ENABLE_PROJECTS="clang" -G "Visual Studio 17 2022" -A x64 -Thost=x64 ../llvm
cd ../