cmake_minimum_required(VERSION 3.22)
project(image
  HOMEPAGE_URL "https://taiwan.timwalter.ch/"
  DESCRIPTION "Projects from Taiwan"
)

set(CMAKE_CXX_STANDARD 20)
add_subdirectory(cpp)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

find_package(PkgConfig REQUIRED)

pkg_check_modules(GTK4 REQUIRED gtk4)
pkg_check_modules(adwaita REQUIRED libadwaita-1)

include_directories(${GTK4_INCLUDE_DIRS} ${adwaita_INCLUDE_DIRS})
link_directories(${GTK4_LIBRARY_DIRS} ${adwaita_LIBRARY_DIRS})
add_definitions(${GTK4_CFLAGS_OTHER} ${adwaita_CFLAGS_OTHER})

# Find gio-2.0 and glib-2.0
pkg_check_modules(GIO REQUIRED gio-2.0 glib-2.0)

# Add include directories and compile options
include_directories(${GIO_INCLUDE_DIRS})
link_directories(${GIO_LIBRARY_DIRS})
add_definitions(${GIO_CFLAGS_OTHER})



target_link_libraries(hello ${OpenCV_LIBS})