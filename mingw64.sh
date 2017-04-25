mkdir -p build-mingw64
pushd build-mingw64
cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Release -DBOOST_ROOT=/mingw64/include/boost -DNANA_CMAKE_NANA_FILESYSTEM_FORCE=ON ..
popd
cmake --build build-mingw64
