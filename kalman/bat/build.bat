conan install .. --output-folder=.. --build=missing
cd ../build
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build .