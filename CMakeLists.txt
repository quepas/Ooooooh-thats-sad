cmake_minimum_required(VERSION 2.8.11)

project(Ooooooh-thats-sad)

# libraries path
set (RECFUSION_INC "D:/Programowanie/Biblioteki/RecFusion SDK 1.2.11/include")
set (RECFUSION_LIB "D:/Programowanie/Biblioteki/RecFusion SDK 1.2.11/lib")
# set (ASSIMP_INC "D:/Programowanie/Biblioteki/assimp/include")
# set (ASSIMP_LIB "D:/Programowanie/Biblioteki/assimp/lib")
 #set (VCG_INC "D:/Programowanie/Biblioteki/vcglib")

add_definitions("/wd4996 /wd4522 /wd4267")
# set libraries
include_directories(${RECFUSION_INC})
link_directories(${RECFUSION_LIB})


file (GLOB RAW_sources src/*.cpp)
file (GLOB RAW_headers src/*.h)

add_executable(Ooooooh-thats-sad ${RAW_sources} ${RAW_headers}) # ${VCG_INC}/wrap/ply/plylib.cpp
target_link_libraries(Ooooooh-thats-sad RecFusion)
