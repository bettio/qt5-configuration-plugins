project(hemera-qt5-configuration-plugins CXX)

cmake_minimum_required(VERSION 2.8.9)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/")
find_package(CommonModules REQUIRED)

#################################################################################################
# Modify these variables to what suits the application best. Remember, no dashes in the version!
set(HEMERA_QT5_SDK_CONFIGURATION_PLUGINS_MAJOR_VERSION 0)
set(HEMERA_QT5_SDK_CONFIGURATION_PLUGINS_MINOR_VERSION 91)
set(HEMERA_QT5_SDK_CONFIGURATION_PLUGINS_RELEASE_VERSION 99)
option(HEMERA_QT5_SDK_CONFIGURATION_PLUGINS_DEVELOPMENT_RELEASE "Must be ON unless we're releasing" ON)

option(ENABLE_WERROR "Enables WError. Always enable when developing, and disable when releasing." ON)

#################################################################################################

set(HEMERA_QT5_SDK_CONFIGURATION_PLUGINS_VERSION ${HEMERA_QT5_SDK_CONFIGURATION_PLUGINS_MAJOR_VERSION}.${HEMERA_QT5_SDK_CONFIGURATION_PLUGINS_MINOR_VERSION}.${HEMERA_QT5_SDK_CONFIGURATION_PLUGINS_RELEASE_VERSION})
if (HEMERA_QT5_SDK_CONFIGURATION_PLUGINS_DEVELOPMENT_RELEASE)
    include(GetGitRevisionDescription)
    get_git_head_revision(GIT_REFSPEC GIT_SHA1)
    STRING(SUBSTRING ${GIT_SHA1} 0 8 GIT_SHA1_SMALL)
    set(HEMERA_QT5_SDK_CONFIGURATION_PLUGINS_VERSION_STRING ${HEMERA_QT5_SDK_CONFIGURATION_PLUGINS_MAJOR_VERSION}.${HEMERA_QT5_SDK_CONFIGURATION_PLUGINS_MINOR_VERSION}.${HEMERA_QT5_SDK_CONFIGURATION_PLUGINS_RELEASE_VERSION}+git${GIT_SHA1_SMALL})
else (HEMERA_QT5_SDK_CONFIGURATION_PLUGINS_DEVELOPMENT_RELEASE)
    set(HEMERA_QT5_SDK_CONFIGURATION_PLUGINS_VERSION_STRING ${HEMERA_QT5_SDK_CONFIGURATION_PLUGINS_MAJOR_VERSION}.${HEMERA_QT5_SDK_CONFIGURATION_PLUGINS_MINOR_VERSION}.${HEMERA_QT5_SDK_CONFIGURATION_PLUGINS_RELEASE_VERSION})
endif (HEMERA_QT5_SDK_CONFIGURATION_PLUGINS_DEVELOPMENT_RELEASE)

find_package(Qt5 COMPONENTS Core Concurrent Network DBus Qml REQUIRED)
find_package(HemeraQt5SDK 0.8.90 COMPONENTS Core Generators Qml REQUIRED)
find_package(HyperspaceQt5 0.90.0 COMPONENTS Core REQUIRED)
find_package(Gravity 0.90.0 COMPONENTS Supermassive REQUIRED)

# Check if Qt SDK private headers have been installed
unset(QT_SDK_PVTHDRS CACHE)
find_path(QT_SDK_PVTHDRS private/HemeraCore/hemeraasyncinitobject_p.h HINTS ${HemeraQt5SDKCore_INCLUDE_DIRS})
if (QT_SDK_PVTHDRS STREQUAL QT_SDK_PVTHDRS-NOTFOUND)
    message(FATAL_ERROR "Hemera Qt5 SDK's private headers have not been found.\nYou need to compile Hemera Qt5 SDK with -DHEMERA_QT5_SDK_PRIVATE_COMPONENTS=ON to build Gravity.")
endif ()

include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR})

set(CMAKE_AUTOMOC TRUE)
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

include(EnableCoverageReport)
include(EnableSlocCount)
include(GenerateCppcheck)
include(GitArchive)
include(CompilerWarnings)
include(TestCXXAcceptsFlag)

# When building these plugins, we want every symbol to be resolvable immediately even if we're building a shared library.
add_definitions(-Wl,-z,defs)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -z defs")
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -z defs")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -z defs")

# Warnings. Enable WError, this component is critical.
if(${ENABLE_WERROR})
    set(RET_W 1)
else(${ENABLE_WERROR})
    set(RET_W 0)
endif(${ENABLE_WERROR})

set(desired
    all
    extra
    sign-compare
    pointer-arith
    format-security
    init-self
    non-virtual-dtor)
set(undesired
    missing-field-initializers
    unused-parameter)
compiler_warnings(CMAKE_CXX_FLAGS_WARNINGS cxx ${RET_W} "${desired}" "${undesired}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_WARNINGS}")

# C++11
if (CMAKE_COMPILER_IS_GNUCXX)
   execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
   if (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7)
        message(STATUS "C++11 activated.")
        add_definitions("-std=gnu++11")
   elseif (GCC_VERSION VERSION_GREATER 4.3 OR GCC_VERSION VERSION_EQUAL 4.3)
        message(WARNING "C++0x activated. If you get any errors update to a compiler which fully supports C++11")
        add_definitions("-std=gnu++0x")
   else ()
        message(FATAL_ERROR "C++11 needed. Therefore a gcc compiler with a version higher than 4.3 is needed.")
   endif()
else (CMAKE_COMPILER_IS_GNUCXX)
   add_definitions("-std=c++0x")
endif (CMAKE_COMPILER_IS_GNUCXX)

# We want explicit literals all over the place, and fast concat
add_definitions(-DQT_NO_CAST_FROM_ASCII -DQT_USE_QSTRINGBUILDER)

# Definitions
add_definitions(-DHEMERA_QT5_SDK_CONFIGURATION_PLUGINS_VERSION="${HEMERA_QT5_SDK_CONFIGURATION_PLUGINS_VERSION_STRING}")

# Config file
configure_file(pluginsconfig.h.in "${CMAKE_CURRENT_BINARY_DIR}/pluginsconfig.h" @ONLY)

# Sources
add_subdirectory(compiler)
add_subdirectory(configure)
add_subdirectory(generator)
add_subdirectory(properties-gen)
add_subdirectory(qml)
add_subdirectory(tools)

if (ENABLE_HEMERA_QT5_SDK_CONFIGURATION_PLUGINS_TESTS)
    enable_testing()
    #add_subdirectory(tests)
endif (ENABLE_HEMERA_QT5_SDK_CONFIGURATION_PLUGINS_TESTS)

# Add these targets only if we are in the root dir
if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
    # --- sloccount ---

    enable_sloccount(FOLDERS src testApp)

    # --- cppcheck ---

    generate_cppcheck(SOURCES src testApp
                      ENABLE_IDS all
                      INLINE_SUPPRESSION)

    # --- coverage ---

    if (ENABLE_HEMERA_QT5_SDK_CONFIGURATION_PLUGINS_COVERAGE)
        #enable_coverage_report(TARGETS Socializer
        #                       TESTS   socializer-test
        #                       FILTER "tests/* src/*")
        #add_definitions(-fprofile-arcs -ftest-coverage)
    endif (ENABLE_HEMERA_QT5_SDK_CONFIGURATION_PLUGINS_COVERAGE)
endif (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})

# Archive
add_make_dist_target(hemera-qt5-configuration-plugins ${HEMERA_QT5_SDK_CONFIGURATION_PLUGINS_VERSION_STRING})

# uninstall target
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
