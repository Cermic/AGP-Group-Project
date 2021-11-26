# AGP-Group-Project

## Information:
Created using Visual Studio using C++ and GLSL for the rendering aspect and SDL to display.

### Dependencies:
This project requires [GLM](https://glm.g-truc.net/0.9.9/index.html), [GLEW](http://glew.sourceforge.net/) and [SDL](https://www.libsdl.org/download-2.0.php) as dependencies. It is recommended to use [Vcpkg](https://vcpkg.io/en/index.html) to download these dependencies and you will be able to then use the provided CMakeLists file to assemble the dependencies as part of the build process. No linking and fiddling needed!

All you need to do is run a CMake generation cycle and the output will be a fully generated Visual Studio project, or a Visual Code project if run from within Visual Code using the CMake tools extension. Make sure you point cmake to your vcpkg cmake file, as this is required to build. e.g:
`cmake -DCMAKE_TOOLCHAIN_FILE=C:/Dev-Tools/vcpkg/scripts/buildsystems/vcpkg.cmake -S . -B build`
Running this command from your project root directory (Where your **CMakeLists.txt** lives) will point cmake to your vcpkg cmake to allow it to load the dependencies required by this repo and then create a full build in a seperate build folder ready for use.

## Purpose:
In this project we used C++, SDL, OpenGL and GLSL to implement dynamic and multiple lighting effects, collision detection, normal mapping, particle effects, camera movement and skybox rendering.

## Link to Demonstration:
[![AGP Group Project](https://img.youtube.com/vi/edkvU8lrvSk/0.jpg)](https://www.youtube.com/watch?v=edkvU8lrvSk "AGP Group Project")
