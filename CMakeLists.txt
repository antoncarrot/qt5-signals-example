cmake_minimum_required(VERSION 2.8)
project(qt-signals-example)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -fPIC -Wno-unknown-pragmas -Wno-invalid-offsetof")

### begin qt5 block
set(QT5_ROOT_PATH "")
if(DEFINED ENV{QT5_ROOT_PATH})
  set(QT5_ROOT_PATH "$ENV{QT5_ROOT_PATH}")
elseif(EXISTS "/opt/qt5")
  set(QT5_ROOT_PATH "/opt/qt5")
endif()

set(CMAKE_AUTOMOC ON)

set(Qt5Core_DIR "${QT5_ROOT_PATH}/lib/cmake/Qt5Core")
find_package(Qt5Core)

include_directories(${Qt5Core_INCLUDE_DIRS})

### begin build block
set(SOURCE_FILES example.cpp)
add_executable(example ${SOURCE_FILES})
target_link_libraries(example ${Qt5Core_LIBRARIES})
