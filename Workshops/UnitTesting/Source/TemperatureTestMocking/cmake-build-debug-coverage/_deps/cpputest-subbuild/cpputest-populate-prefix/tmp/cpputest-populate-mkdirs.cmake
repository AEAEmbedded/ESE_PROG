# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/jakorten/Library/Mobile Documents/com~apple~CloudDocs/Documents/Documenten - MacBook Pro van JA/HAN/HAN Engineering/S3 Programming - Software Engineering/Repo2425/ESE_PROG/Workshops/UnitTesting/Source/TemperatureTestMocking/cmake-build-debug-coverage/_deps/cpputest-src"
  "/Users/jakorten/Library/Mobile Documents/com~apple~CloudDocs/Documents/Documenten - MacBook Pro van JA/HAN/HAN Engineering/S3 Programming - Software Engineering/Repo2425/ESE_PROG/Workshops/UnitTesting/Source/TemperatureTestMocking/cmake-build-debug-coverage/_deps/cpputest-build"
  "/Users/jakorten/Library/Mobile Documents/com~apple~CloudDocs/Documents/Documenten - MacBook Pro van JA/HAN/HAN Engineering/S3 Programming - Software Engineering/Repo2425/ESE_PROG/Workshops/UnitTesting/Source/TemperatureTestMocking/cmake-build-debug-coverage/_deps/cpputest-subbuild/cpputest-populate-prefix"
  "/Users/jakorten/Library/Mobile Documents/com~apple~CloudDocs/Documents/Documenten - MacBook Pro van JA/HAN/HAN Engineering/S3 Programming - Software Engineering/Repo2425/ESE_PROG/Workshops/UnitTesting/Source/TemperatureTestMocking/cmake-build-debug-coverage/_deps/cpputest-subbuild/cpputest-populate-prefix/tmp"
  "/Users/jakorten/Library/Mobile Documents/com~apple~CloudDocs/Documents/Documenten - MacBook Pro van JA/HAN/HAN Engineering/S3 Programming - Software Engineering/Repo2425/ESE_PROG/Workshops/UnitTesting/Source/TemperatureTestMocking/cmake-build-debug-coverage/_deps/cpputest-subbuild/cpputest-populate-prefix/src/cpputest-populate-stamp"
  "/Users/jakorten/Library/Mobile Documents/com~apple~CloudDocs/Documents/Documenten - MacBook Pro van JA/HAN/HAN Engineering/S3 Programming - Software Engineering/Repo2425/ESE_PROG/Workshops/UnitTesting/Source/TemperatureTestMocking/cmake-build-debug-coverage/_deps/cpputest-subbuild/cpputest-populate-prefix/src"
  "/Users/jakorten/Library/Mobile Documents/com~apple~CloudDocs/Documents/Documenten - MacBook Pro van JA/HAN/HAN Engineering/S3 Programming - Software Engineering/Repo2425/ESE_PROG/Workshops/UnitTesting/Source/TemperatureTestMocking/cmake-build-debug-coverage/_deps/cpputest-subbuild/cpputest-populate-prefix/src/cpputest-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/jakorten/Library/Mobile Documents/com~apple~CloudDocs/Documents/Documenten - MacBook Pro van JA/HAN/HAN Engineering/S3 Programming - Software Engineering/Repo2425/ESE_PROG/Workshops/UnitTesting/Source/TemperatureTestMocking/cmake-build-debug-coverage/_deps/cpputest-subbuild/cpputest-populate-prefix/src/cpputest-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/jakorten/Library/Mobile Documents/com~apple~CloudDocs/Documents/Documenten - MacBook Pro van JA/HAN/HAN Engineering/S3 Programming - Software Engineering/Repo2425/ESE_PROG/Workshops/UnitTesting/Source/TemperatureTestMocking/cmake-build-debug-coverage/_deps/cpputest-subbuild/cpputest-populate-prefix/src/cpputest-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
