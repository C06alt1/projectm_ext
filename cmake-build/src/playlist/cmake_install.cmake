# Install script for directory: D:/Coding/projectm/src/playlist

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "D:/Coding/projectm-install")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Devel" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Coding/projectm/cmake-build/src/playlist/Debug/projectM-4-playlistd.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Coding/projectm/cmake-build/src/playlist/Release/projectM-4-playlist.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Coding/projectm/cmake-build/src/playlist/MinSizeRel/projectM-4-playlist.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Coding/projectm/cmake-build/src/playlist/RelWithDebInfo/projectM-4-playlist.lib")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Runtime" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/Coding/projectm/cmake-build/src/playlist/Debug/projectM-4-playlistd.dll")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/Coding/projectm/cmake-build/src/playlist/Release/projectM-4-playlist.dll")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/Coding/projectm/cmake-build/src/playlist/MinSizeRel/projectM-4-playlist.dll")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/Coding/projectm/cmake-build/src/playlist/RelWithDebInfo/projectM-4-playlist.dll")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Devel" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/projectM-4" TYPE FILE FILES
    "D:/Coding/projectm/cmake-build/src/playlist/include/projectM-4/projectM_playlist_export.h"
    "D:/Coding/projectm/src/playlist/api/projectM-4/playlist.h"
    "D:/Coding/projectm/src/playlist/api/projectM-4/playlist_callbacks.h"
    "D:/Coding/projectm/src/playlist/api/projectM-4/playlist_core.h"
    "D:/Coding/projectm/src/playlist/api/projectM-4/playlist_filter.h"
    "D:/Coding/projectm/src/playlist/api/projectM-4/playlist_items.h"
    "D:/Coding/projectm/src/playlist/api/projectM-4/playlist_memory.h"
    "D:/Coding/projectm/src/playlist/api/projectM-4/playlist_playback.h"
    "D:/Coding/projectm/src/playlist/api/projectM-4/playlist_types.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Devel" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/projectM4Playlist" TYPE FILE FILES
    "D:/Coding/projectm/cmake-build/src/playlist/libprojectMPlaylist/projectM4PlaylistConfigVersion.cmake"
    "D:/Coding/projectm/cmake-build/src/playlist/libprojectMPlaylist/projectM4PlaylistConfig.cmake"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Devel" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/projectM4Playlist/projectM4PlaylistTargets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/projectM4Playlist/projectM4PlaylistTargets.cmake"
         "D:/Coding/projectm/cmake-build/src/playlist/CMakeFiles/Export/fce45fea70d0901e2efaeb2cc31668a0/projectM4PlaylistTargets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/projectM4Playlist/projectM4PlaylistTargets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/projectM4Playlist/projectM4PlaylistTargets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/projectM4Playlist" TYPE FILE FILES "D:/Coding/projectm/cmake-build/src/playlist/CMakeFiles/Export/fce45fea70d0901e2efaeb2cc31668a0/projectM4PlaylistTargets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/projectM4Playlist" TYPE FILE FILES "D:/Coding/projectm/cmake-build/src/playlist/CMakeFiles/Export/fce45fea70d0901e2efaeb2cc31668a0/projectM4PlaylistTargets-debug.cmake")
  endif()
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/projectM4Playlist" TYPE FILE FILES "D:/Coding/projectm/cmake-build/src/playlist/CMakeFiles/Export/fce45fea70d0901e2efaeb2cc31668a0/projectM4PlaylistTargets-minsizerel.cmake")
  endif()
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/projectM4Playlist" TYPE FILE FILES "D:/Coding/projectm/cmake-build/src/playlist/CMakeFiles/Export/fce45fea70d0901e2efaeb2cc31668a0/projectM4PlaylistTargets-relwithdebinfo.cmake")
  endif()
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/projectM4Playlist" TYPE FILE FILES "D:/Coding/projectm/cmake-build/src/playlist/CMakeFiles/Export/fce45fea70d0901e2efaeb2cc31668a0/projectM4PlaylistTargets-release.cmake")
  endif()
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "D:/Coding/projectm/cmake-build/src/playlist/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
