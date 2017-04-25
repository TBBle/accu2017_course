mkdir -p build-mingw64-clang
pushd build-mingw64-clang
cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Release -DBOOST_ROOT=/mingw64/include/boost -DNANA_CMAKE_NANA_FILESYSTEM_FORCE=ON -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ ..
popd
cmake --build build-mingw64-clang
