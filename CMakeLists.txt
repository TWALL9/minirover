cmake_minimum_required(VERSION 3.10)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED true)

set(application_name "otomo")

project(${application_name}  
        VERSION 0.1
        LANGUAGES C
        )

# architecture/cross-compile
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(CMAKE_SIZE arm-none-eabi-size)
set(CMAKE_DEBUGGER arm-none-eabi-gdb)
set(CMAKE_DEBUGGER arm-none-eabi-gdb)
set(CMAKE_CPPFILT arm-none-eabi-c++filt)

set(hal_path lib/STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_)
set(hal_src 
    ${hal_path}adc_ex.c
    ${hal_path}adc.c
    ${hal_path}can.c
    ${hal_path}cec.c
    ${hal_path}cortex.c
    ${hal_path}crc.c
    ${hal_path}cryp_ex.c
    ${hal_path}cryp.c
    ${hal_path}dac_ex.c
    ${hal_path}dac.c
    ${hal_path}dcmi_ex.c
    ${hal_path}dcmi.c
    ${hal_path}dfsdm.c
)
add_library(stm32_hal ${hal_src})

#device family
target_compile_definitions(stm32_hal PUBLIC STM32F407xx)
target_include_directories(stm32_hal PUBLIC
 lib/STM32CubeF4/Drivers/CMSIS/Device/ST/STM32F4xx/Include/)
target_include_directories(stm32_hal PUBLIC
 lib/STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Inc/)

target_compile_options(stm32_hal PUBLIC 
                        -mcpu=cortex-m4
                        -mthumb
                        -mthumb-interwork
                        -mlittle-endian
                        -mfloat-abi=hard
                        -mfpu=fpuv4-sp-d16
)

#get_filename_component(LINKER_SCRIPT linker/STM32F407VG_FLASH.ld ABSOLUTE)

# if(CMAKE_BUILD_TYPE MATCHES debug)
#     message("Debug build")
# elseif(CMAKE_BUILD_TYPE MATCHES release)
#     message("Release build")
# elseif(NOT CMAKE_BUILD_TYPE)
#     message("No build type, defaulting to debug")
#     set(CMAKE_BUILD_TYPE Debug)
# endif()

# list(APPEND HAL_LIB "${PROJECT_SOURCE_DIR}/lib/STM32CubeF4/Drivers/STM32F4xx_HAL_Driver")
# list(APPEND SRC_DIR "${PROJECT_SOURCE_DIR}/src")
# list(APPEND INC_DIR "${PROJECT_SOURCE_DIR}/include")

# add_executable(otomo src/main.c)

# target_link_libraries(otomo PUBLIC ${HAL_LIB})

# target_include_directories(otomo PUBLIC
#                             "${PROJECT_BINARY_DIR}"
#                             ${SRC_DIR}
#                             ${INC_DIR}
#                             ${HAL_LIB}
#                             )
