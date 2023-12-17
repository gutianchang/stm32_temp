cmake_minimum_required(VERSION 3.16)
set(PROJ_NAME stm32-freertos)
set(STM32_CMAKE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

# This must come before the project call!
set(CMAKE_TOOLCHAIN_FILE ${STM32_CMAKE_PATH}/cmake/stm32_gcc.cmake)
project(${PROJ_NAME} CXX C ASM)
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

set(HAL_COMP_LIST RCC GPIO CORTEX)
set(CMSIS_COMP_LIST "")
set(FREERTOS_COMP_LIST "")
set(FREERTOS_NAMESPACE FreeRTOS)

add_definitions(-DHSE_VALUE=16000000U)
list(APPEND CMSIS_COMP_LIST STM32F765II)
list(APPEND HAL_COMP_LIST STM32F765II)
list(APPEND FREERTOS_COMP_LIST ARM_CM7)

# The device family needs to be supplied as a component to use the Cube FreeRTOS sources
list(APPEND FREERTOS_COMP_LIST STM32F7)
set(FREERTOS_F7_NAMESPACE ${FREERTOS_NAMESPACE}::STM32::F7)

find_package(CMSIS COMPONENTS ${CMSIS_COMP_LIST} REQUIRED)
find_package(HAL COMPONENTS ${HAL_COMP_LIST} REQUIRED)
find_package(FreeRTOS COMPONENTS ${FREERTOS_COMP_LIST} REQUIRED)

set(PROJECT_SOURCES
    src/main.cpp
    src/syscalls.c
)

set(INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set(TARGET_NAME ${PROJ_NAME}-f765ii)
add_executable(${TARGET_NAME})

target_sources(${TARGET_NAME} PRIVATE ${PROJECT_SOURCES})
target_include_directories(${TARGET_NAME} PRIVATE ${INCLUDE_DIRS})
target_link_libraries(${TARGET_NAME} PRIVATE
    ${FREERTOS_F7_NAMESPACE}::Timers
    ${FREERTOS_F7_NAMESPACE}::Heap::1
    ${FREERTOS_F7_NAMESPACE}::ARM_CM7
    HAL::STM32::F7::RCC
    HAL::STM32::F7::GPIO
    HAL::STM32::F7::CORTEX
    CMSIS::STM32::F765II
    STM32::NoSys
)

stm32_print_size_of_target(${TARGET_NAME})
stm32_generate_binary_file(${TARGET_NAME})
stm32_generate_hex_file(${TARGET_NAME})
