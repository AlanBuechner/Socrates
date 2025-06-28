cd build
cmake -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_PROJECTS="clang" -DLLVM_ENABLE_RUNTIMES="compiler-rt" -G "Visual Studio 17 2022" -A x64 -Thost=x64 ../llvm
cd ../