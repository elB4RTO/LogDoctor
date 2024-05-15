cmake_minimum_required(VERSION 3.5)

project(LogDoctor VERSION 4.01 LANGUAGES CXX)

include(cmake/project.cmake)
logdoc_set_cmake_options()

include(cmake/dependencies.cmake)
logdoc_find_required_packages()

qt_add_executable(LogDoctor MANUAL_FINALIZATION)

include(cmake/sources.cmake)
logdoc_add_target_sources()

include(cmake/build.cmake)
logdoc_set_compiler_options()
logdoc_set_linker_options()
logdoc_set_target_properties()

include(cmake/translations.cmake)
logdoc_add_lupdate_target()
#logdoc_add_lrelease_target()

qt_finalize_executable(LogDoctor)


# Test
include(cmake/tests.cmake)
logdoc_add_tests()


# Install
include(cmake/install.cmake)
logdoc_add_install_directives()
