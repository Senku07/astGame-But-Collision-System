@echo off
md build
pushd build
cl ../src/main.cpp /Zi /nologo /MD /I"../dp" /link opengl32.lib user32.lib kernel32.lib gdi32.lib ../dp/glfw3.lib shell32.lib
main.exe
popd 