set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Specify the cross-compiler
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)

# Prevent CMake from trying to compile a test program (it will fail without a linker script)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Target-specific flags for the LM3S6965 (Cortex-M3)
set(ARM_FLAGS "-mcpu=cortex-m3 -mthumb -nostartfiles -ffreestanding -DBAREMETAL")
set(CMAKE_C_FLAGS "${ARM_FLAGS}" CACHE STRING "" FORCE)
