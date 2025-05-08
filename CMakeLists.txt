cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(dozerog4)

find_package(Geant4 REQUIRED ui_all vis_all)

include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

add_executable(sim sim.cc ${sources} ${headers})
target_link_libraries(sim ${Geant4_LIBRARIES})

file(GLOB MACRO_FILE 
	"*.mac"
)
file(COPY ${MACRO_FILE} DESTINATION ${PROJECT_BINARY_DIR})

file(GLOB MACRO_FILE 
	"*.dat"
)
file(COPY ${DATA_FILE} DESTINATION ${PROJECT_BINARY_DIR})

add_custom_target(dozerog4 DEPENDS sim)
