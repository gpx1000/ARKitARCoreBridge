cmake_minimum_required(VERSION 3.4.1)

add_subdirectory(${CMAKE_SOURCE_DIR}/../Common ${CMAKE_CURRENT_BINARY_DIR}/Common)
add_library( native-lib SHARED src/main/cpp/jni_interface.cc )
find_library( log-lib log )
find_library( android-lib android )
target_link_libraries( native-lib ${log-lib} ${android-lib} Common )