# CMakeList.txt: проект CMake для Lab4Cmake; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

# Включение горячей перезагрузки для компиляторов MSVC, если поддерживается.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Lab4Cmake")

# Добавьте источник в исполняемый файл этого проекта.
file(GLOB_RECURSE SOURCES "src/*.cpp"  "include/*.h")
#add_library(muParser ${SOURCES})
link_directories(${CMAKE_SOURCE_DIR}/lib)
add_executable (Lab4Cmake "Lab4Cmake.cpp" "Lab4Cmake.h" )
target_link_libraries(Lab4Cmake muparser)
target_include_directories(Lab4Cmake PRIVATE "include" )

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Lab4Cmake PROPERTY CXX_STANDARD 20)
endif()
set(DESTINATION_DIR ${CMAKE_BINARY_DIR})
file(COPY "${CMAKE_SOURCE_DIR}/lib/muparser.dll" DESTINATION ${DESTINATION_DIR})
# TODO: Добавьте тесты и целевые объекты, если это необходимо.
