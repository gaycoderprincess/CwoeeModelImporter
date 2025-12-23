cmake_minimum_required(VERSION 3.27)
project(CwoeeModelImporter)

SET(CMAKE_SYSTEM_NAME Windows)

SET(CMAKE_C_COMPILER i686-w64-mingw32-gcc)
SET(CMAKE_CXX_COMPILER i686-w64-mingw32-g++)
SET(CMAKE_RC_COMPILER i686-w64-mingw32-windres)
SET(CMAKE_RANLIB i686-w64-mingw32-ranlib)

SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static -s")

link_directories(${CMAKE_SOURCE_DIR}/../nya-common/lib32)

add_executable(CwoeeModelImporter main.cpp)
set_target_properties(CwoeeModelImporter PROPERTIES SUFFIX "_gcp.exe")
target_include_directories(CwoeeModelImporter PUBLIC ../nya-common)
target_include_directories(CwoeeModelImporter PUBLIC ../nya-common/3rdparty)

target_link_libraries(CwoeeModelImporter assimp)
target_link_libraries(CwoeeModelImporter zlib)
target_link_libraries(CwoeeModelImporter pugixml)
target_link_libraries(CwoeeModelImporter polyclipping)
target_link_libraries(CwoeeModelImporter minizip)
target_link_libraries(CwoeeModelImporter kubazip)
target_link_libraries(CwoeeModelImporter poly2tri)
target_link_libraries(CwoeeModelImporter draco)