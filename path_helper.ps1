if (Test-Path -Path .\build\Debug -PathType Container){
    Remove-Item -Path .\build\Debug -Recurse -Force
}

$env:STM32_TOOLCHAIN_PATH = "C:\ST\STM32CubeIDE_1.14.0\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.11.3.rel1.win32_1.1.100.202309141235\tools"
$env:STM32_TARGET_TRIPLET = "arm-none-eabi"
$env:STM32_CUBE_F1_PATH="C:\Users\tianc\Desktop\stm32_cmake_temp\lib\STM32F1\STM32CubeF1"
$env:STM32_CUBE_F7_PATH="C:\Users\tianc\Desktop\stm32_cmake_temp\lib\STM32F7\STM32CubeF7"

# cmake -B ./build/Release -G "Unix Makefiles" -S . -DCMAKE_BUILD_TYPE=Release
cmake -B ./build/Debug -G "Unix Makefiles" -S . -DCMAKE_BUILD_TYPE=Debug
# cmake --build .\build\Debug --target all --verbose
