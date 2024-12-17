@echo off
setlocal enabledelayedexpansion

set VK_SDK_EXE=3rdParty\VulkanSDK\Bin\glslangValidator.exe
set SHADER_DIR=src\Engine\System\RenderSystem\shader

:: 遍历 shader 文件夹下所有 .vert 和 .frag 文件
for %%f in (%SHADER_DIR%\*.vert) do (
    echo Compiling %%f ...
    "%VK_SDK_EXE%" -V "%%f" -o "%SHADER_DIR%\spv\%%~nf.vert.spv"
)

for %%f in (%SHADER_DIR%\*.frag) do (
    echo Compiling %%f ...
    "%VK_SDK_EXE%" -V "%%f" -o "%SHADER_DIR%\spv\%%~nf.frag.spv"
)

pause