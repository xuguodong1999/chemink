cd 3rdparty/zlib && git checkout v1.2.11
cd ../..
mkdir lib\zlib build\zlib
cd build\zlib
cmake ../../3rdparty/zlib -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../../lib/zlib -DBUILD_TESTING=OFF
cmake --build . -j 8 --target install --config Release
cd ../..

cd 3rdparty/libxml2 && git checkout 1e7851b
cd ../..
mkdir lib\libxml2 build\libxml2
cd build\libxml2
cmake ../../3rdparty/libxml2 -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../../lib/libxml2 -DBUILD_TESTING=OFF
cmake --build . -j 8 --target install --config Release
cd ../..

cd 3rdparty/openbabel && git checkout openbabel-3-1-1
cd ../..
mkdir lib\openbabel build\openbabel
cd build\openbabel
cmake ../../3rdparty/openbabel -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../../lib/openbabel -DBUILD_TESTING=OFF -DRAPIDJSON_INCLUDE_DIRS=../../include/rapidjson -DBUILD_GUI=OFF -DWITH_INCHI=OFF -DWITH_STATIC_INCHI=OFF -DOPENBABEL_USE_SYSTEM_INCHI=OFF -DWITH_MAEPARSER=OFF -DZLIB_LIBRARY_RELEASE=../../lib/zlib/lib/zlib.lib -DZLIB_INCLUDE_DIR=../../lib/zlib/include -DLIBXML2_INCLUDE_DIR=../../lib/libxml2/include/libxml2 -DLIBXML2_LIBRARY=../../lib/libxml2/lib/libxml2.lib -DWITH_STATIC_LIBXML=OFF -DBUILD_SHARED=OFF
REM cmake --build . -j 8 --target install --config Debug && move ..\..\lib\openbabel\bin\openbabel-3.lib ..\..\lib\openbabel\bin\openbabel-3d.lib
cmake --build . -j 8 --target install --config Release
cd ../..

cd 3rdparty/opencv && git checkout 4.5.0
cd ../..
mkdir lib\opencv build\opencv
cd build\opencv
cmake ../../3rdparty/opencv -DCMAKE_INSTALL_PREFIX=../../lib/opencv -DBUILD_opencv_python_tests=OFF -DBUILD_opencv_python_bindings_generator=OFF -DBUILD_JAVA=OFF -DBUILD_opencv_java_bindings_generator=OFF -DBUILD_TESTS=OFF -DBUILD_PREF_TESTS=OFF -DWITH_ADE=OFF -DWITH_FFMPEG=OFF -DWITH_IPP=OFF -DBUILD_SHARED_LIBS=OFF -DWITH_EIGEN=OFF -DBUILD_opencv_world=OFF
cmake --build . -j 8 --target install --config Release
cd ../..

