# CMakeList.txt : CMake project for DogAteMyMouse, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.10)
project(DotAteMyMouse)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/lib/dear_imgui")
set(IMGUI_SOURCES 
  ${IMGUI_DIR}/imgui.cpp
  ${IMGUI_DIR}/imgui_demo.cpp
  ${IMGUI_DIR}/imgui_draw.cpp
  ${IMGUI_DIR}/imgui_tables.cpp
  ${IMGUI_DIR}/imgui_widgets.cpp
  ${IMGUI_DIR}/backend/imgui_impl_dx11.cpp
  ${IMGUI_DIR}/backend/imgui_impl_win32.cpp
)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Add source to this project's executable.
add_executable (${PROJECT_NAME} 
    "src/main.cpp"
    ${IMGUI_SOURCES}    
    "src/interfaces/imatchengine.h"  
    "src/matchengine.h"
    "src/interfaces/ikeygen.h"
)

target_include_directories(${PROJECT_NAME} PUBLIC
  ${IMGUI_DIR}
  ${IMGUI_DIR}/backend
)

target_link_libraries(${PROJECT_NAME}
  d3d11
  dxgi
  ${OpenCV_LIBS}
)