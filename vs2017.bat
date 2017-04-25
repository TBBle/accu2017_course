mkdir build-vs2017
pushd build-vs2017
cmake -G "Visual Studio 15" -A x64 -T v141 -DBOOST_ROOT=c:\\Boost\\boost_1_64_0_b2 ..
popd
cmake --build build-vs2017
