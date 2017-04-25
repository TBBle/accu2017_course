mkdir build-vs2017-clangc2
pushd build-vs2017-clangc2
cmake -G "Visual Studio 15" -A x64 -T v141_clang_c2 -DBOOST_ROOT=c:\\Boost\\boost_1_64_0_b2 ..
popd
cmake --build build-vs2017-clangc2
