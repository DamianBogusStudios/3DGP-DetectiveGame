:: Copyright LifeEXE. All Rights Reserved.
@echo off

set SourceCodePath=%~dp0\Source

FOR /R "%SourceCodePath%" %%f IN (*.cpp, *.h, *.cs) DO (
    clang-format -i "%%f"
    echo %%f
)
