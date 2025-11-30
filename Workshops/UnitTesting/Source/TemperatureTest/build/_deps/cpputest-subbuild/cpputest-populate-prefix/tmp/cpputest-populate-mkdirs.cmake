# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/Users/jakorten/Library/Mobile Documents/com~apple~CloudDocs/Documents/Documenten - MacBook Pro van JA/HAN/HAN Engineering/S3 Programming - Software Engineering/Repo2425/ESE_PROG/Workshops/UnitTesting/Source/TemperatureTest/build/_deps/cpputest-src")
  file(MAKE_DIRECTORY "/Users/jakorten/Library/Mobile Documents/com~apple~CloudDocs/Documents/Documenten - MacBook Pro van JA/HAN/HAN Engineering/S3 Programming - Software Engineering/Repo2425/ESE_PROG/Workshops/UnitTesting/Source/TemperatureTest/build/_deps/cpputest-src")
endif()
file(MAKE_DIRECTORY
  "/Users/jakorten/Library/Mobile Documents/com~apple~CloudDocs/Documents/Documenten - MacBook Pro van JA/HAN/HAN Engineering/S3 Programming - Software Engineering/Repo2425/ESE_PROG/Workshops/UnitTesting/Source/TemperatureTest/build/_deps/cpputest-build"
  "/Users/jakorten/Library/Mobile Documents/com~apple~CloudDocs/Documents/Documenten - MacBook Pro van JA/HAN/HAN Engineering/S3 Programming - Software Engineering/Repo2425/ESE_PROG/Workshops/UnitTesting/Source/TemperatureTest/build/_deps/cpputest-subbuild/cpputest-populate-prefix"
  "/Users/jakorten/Library/Mobile Documents/com~apple~CloudDocs/Documents/Documenten - MacBook Pro van JA/HAN/HAN Engineering/S3 Programming - Software Engineering/Repo2425/ESE_PROG/Workshops/UnitTesting/Source/TemperatureTest/build/_deps/cpputest-subbuild/cpputest-populate-prefix/tmp"
  "/Users/jakorten/Library/Mobile Documents/com~apple~CloudDocs/Documents/Documenten - MacBook Pro van JA/HAN/HAN Engineering/S3 Programming - Software Engineering/Repo2425/ESE_PROG/Workshops/UnitTesting/Source/TemperatureTest/build/_deps/cpputest-subbuild/cpputest-populate-prefix/src/cpputest-populate-stamp"
  "/Users/jakorten/Library/Mobile Documents/com~apple~CloudDocs/Documents/Documenten - MacBook Pro van JA/HAN/HAN Engineering/S3 Programming - Software Engineering/Repo2425/ESE_PROG/Workshops/UnitTesting/Source/TemperatureTest/build/_deps/cpputest-subbuild/cpputest-populate-prefix/src"
  "/Users/jakorten/Library/Mobile Documents/com~apple~CloudDocs/Documents/Documenten - MacBook Pro van JA/HAN/HAN Engineering/S3 Programming - Software Engineering/Repo2425/ESE_PROG/Workshops/UnitTesting/Source/TemperatureTest/build/_deps/cpputest-subbuild/cpputest-populate-prefix/src/cpputest-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/jakorten/Library/Mobile Documents/com~apple~CloudDocs/Documents/Documenten - MacBook Pro van JA/HAN/HAN Engineering/S3 Programming - Software Engineering/Repo2425/ESE_PROG/Workshops/UnitTesting/Source/TemperatureTest/build/_deps/cpputest-subbuild/cpputest-populate-prefix/src/cpputest-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/jakorten/Library/Mobile Documents/com~apple~CloudDocs/Documents/Documenten - MacBook Pro van JA/HAN/HAN Engineering/S3 Programming - Software Engineering/Repo2425/ESE_PROG/Workshops/UnitTesting/Source/TemperatureTest/build/_deps/cpputest-subbuild/cpputest-populate-prefix/src/cpputest-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
