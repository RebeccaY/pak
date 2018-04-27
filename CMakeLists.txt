cmake_minimum_required(VERSION 2.6)
project(pak)

add_executable(pak exceptionhandler.cpp 
func.cpp main.cpp pak.cpp directoryentry.cpp
treeitem.cpp pakexception.cpp)
set (PACKAGE pak)
set (VERSION 0.1.1)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCLI")
install(TARGETS pak RUNTIME DESTINATION bin)
INSTALL(DIRECTORY ${CMAKE_SOURCE_DIR}/man1/ DESTINATION share/man/man1)
INSTALL(DIRECTORY ${CMAKE_SOURCE_DIR}/doc/ DESTINATION share/doc/${PACKAGE})

# The version number.
set (pak_VERSION \"0.1.1\")

configure_file(version.h.in ${CMAKE_SOURCE_DIR}/version.h)
configure_file(pak.spec.in ${CMAKE_SOURCE_DIR}/pak.spec)

set(MINGW_PREFIX "i686-w64-mingw32-")

set(CPACK_PACKAGE_VERSION ${VERSION})
set(CPACK_GENERATOR "TGZ;RPM;DEB")
set(CPACK_PACKAGE_NAME "pak")
set(CPACK_PACKAGE_RELEASE 1)
set(CPACK_PACKAGE_CONTACT "Dennis Katsonis")
set(CPACK_PACKAGE_VENDOR "Dekay Software")
set(CPACK_STRIP_FILES "bin/pak")
#set(CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST_ADDITION "/usr/share/man")
set(CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST_ADDITION "/usr/share/man/man1" "/usr/share/man")
set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
#set(CPACK_RPM_USER_BINARY_SPECFILE "${CMAKE_SOURCE_DIR}/pak.spec")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_PACKAGE_RELEASE}.${CMAKE_SYSTEM_PROCESSOR}")
include(CPack)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()
