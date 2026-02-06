# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/home/fasyaais/ravelware/intern-project/01-esp32-fudamental/managed_components/marcel-cd__etlcpp/etl")
  file(MAKE_DIRECTORY "/home/fasyaais/ravelware/intern-project/01-esp32-fudamental/managed_components/marcel-cd__etlcpp/etl")
endif()
file(MAKE_DIRECTORY
  "/home/fasyaais/ravelware/intern-project/01-esp32-fudamental/bin/v0.0.1/esp-idf/marcel-cd__etlcpp/etl"
  "/home/fasyaais/ravelware/intern-project/01-esp32-fudamental/bin/v0.0.1/esp-idf/marcel-cd__etlcpp"
  "/home/fasyaais/ravelware/intern-project/01-esp32-fudamental/bin/v0.0.1/esp-idf/marcel-cd__etlcpp/tmp"
  "/home/fasyaais/ravelware/intern-project/01-esp32-fudamental/bin/v0.0.1/esp-idf/marcel-cd__etlcpp/src/etl_build-stamp"
  "/home/fasyaais/ravelware/intern-project/01-esp32-fudamental/bin/v0.0.1/esp-idf/marcel-cd__etlcpp/src"
  "/home/fasyaais/ravelware/intern-project/01-esp32-fudamental/bin/v0.0.1/esp-idf/marcel-cd__etlcpp/src/etl_build-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/fasyaais/ravelware/intern-project/01-esp32-fudamental/bin/v0.0.1/esp-idf/marcel-cd__etlcpp/src/etl_build-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/fasyaais/ravelware/intern-project/01-esp32-fudamental/bin/v0.0.1/esp-idf/marcel-cd__etlcpp/src/etl_build-stamp${cfgdir}") # cfgdir has leading slash
endif()
