# CMakeList.txt for the ROOTExamples project
# CMake version 3 is required
cmake_minimum_required (VERSION 3.12 FATAL_ERROR)
project(bigrips_sort CXX)

# Add in CMAKE_PREFIX_PATH the installation prefix for ROOT 
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
# You need to add COMPONENTS according to what you want to do.
find_package(ROOT REQUIRED COMPONENTS Core RIO Net Hist Tree XMLIO XMLParser)
# Include ROOT cmake macros
include(${ROOT_USE_FILE})

set(ANAROOT_DIR $ENV{TARTSYS} CACHE PATH "anaroot install dir")

# Set the default install prefix to [source_dir]/install
# (You usually don't want to install this project to /usr/local/)
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install CACHE PATH "install dir" FORCE)

add_subdirectory(src)

file (WRITE ${PROJECT_BINARY_DIR}/setup_BRSvme.sh
"BRSvme_SYS=" ${CMAKE_INSTALL_PREFIX} "\n"
"source " ${ANAROOT_DIR}"/scripts/setup.sh\n"
"export LD_LIBRARY_PATH=${BRSvme_SYS}/lib:${LD_LIBRARY_PATH}\n"
"export PATH=${BRSvme_SYS}/bin:${PATH}\n"
"export ROOT_INCLUDE_PATH=${BRSvme_SYS}/include:${ROOT_INCLUDE_PATH}\n"
)

install(FILES
	afterproc_LJ.sh
	${PROJECT_BINARY_DIR}/setup_BRSvme.sh
	DESTINATION .)

install(DIRECTORY db matrix DESTINATION .)
