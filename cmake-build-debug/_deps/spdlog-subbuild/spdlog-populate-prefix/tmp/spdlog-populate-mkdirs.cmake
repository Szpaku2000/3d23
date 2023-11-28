# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/ProgramowanieGrafiki3D/cmake-build-debug/_deps/spdlog-src"
  "D:/ProgramowanieGrafiki3D/cmake-build-debug/_deps/spdlog-build"
  "D:/ProgramowanieGrafiki3D/cmake-build-debug/_deps/spdlog-subbuild/spdlog-populate-prefix"
  "D:/ProgramowanieGrafiki3D/cmake-build-debug/_deps/spdlog-subbuild/spdlog-populate-prefix/tmp"
  "D:/ProgramowanieGrafiki3D/cmake-build-debug/_deps/spdlog-subbuild/spdlog-populate-prefix/src/spdlog-populate-stamp"
  "D:/ProgramowanieGrafiki3D/cmake-build-debug/_deps/spdlog-subbuild/spdlog-populate-prefix/src"
  "D:/ProgramowanieGrafiki3D/cmake-build-debug/_deps/spdlog-subbuild/spdlog-populate-prefix/src/spdlog-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/ProgramowanieGrafiki3D/cmake-build-debug/_deps/spdlog-subbuild/spdlog-populate-prefix/src/spdlog-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/ProgramowanieGrafiki3D/cmake-build-debug/_deps/spdlog-subbuild/spdlog-populate-prefix/src/spdlog-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
