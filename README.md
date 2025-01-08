# Simple Text Editor

A simple text editor written in C++, using OpenGL, GLEW and GLFW to render text and handle windows.

## Dependancies

Install CMake [here](https://cmake.org/)

**Linux**

Run the following commands in a terminal:

```bash
sudo apt-get update
sudo apt-get install libglfw3-dev
sudo apt-get install libglew-dev
sudo apt-get install libfreetype6-dev
```

**macOS**

Install Homebrew (if not already installed)

```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

Then run:
```bash
brew install glfw
brew install glew
brew install freetype
```

**Windows**

Install GLFW [here](https://www.glfw.org/)
Install GLEW [here](https://glew.sourceforge.net/)
Install FreeType [here](https://freetype.org/)

**CMake Configuration on Windows**
1. Open CMake GUI.
2. Set the source code directory to the location of your project.
3. Set the build directory to a location where you want to build your project (e.g., C:/path/to/build).
4. Click "Configure".
5. Specify the paths to the GLFW, GLEW, FreeType, and SDL2 libraries and include directories in the CMake GUI.
6. Click "Generate".
7. Open the generated project in your preferred IDE (e.g., Visual Studio) and build the project.

## Installation

Run the following command to build the project:

```bash
mkdir build
cd build
cmake ..
make
```

## Running

```bash
./Text_Editor [filename] 
```
should run the text editor on a file.
```bash
./Text_Editor
```
on its own will create a generic file to store the text in.

![Alt text](assets/hello_world.png)
![Alt text](assets/video.mkv)

Enjoy!