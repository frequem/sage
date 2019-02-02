set(CMAKE_SYSTEM_NAME Android)
#set(CMAKE_ANDROID_ARCH_ABI arm64-v8a)#pass via command line
#set(CMAKE_SYSTEM_VERSION 27)# don't specify to use latest
#set(CMAKE_ANDROID_NDK /opt/android-ndk)# don't specify to use ANDROID_NDK/ANDROID_NDK_ROOT environment vars
set(CMAKE_ANDROID_STL_TYPE gnustl_static)

set(CMAKE_ANDROID_NDK_TOOLCHAIN_VERSION clang)
#set(CMAKE_CROSS_COMPILING TRUE)

