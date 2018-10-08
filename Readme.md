# CSCI 3260 Assignment 1

> 1155123308 Wentao Zhu

## Description

As you may know, modern OpenGL is poorly supported on macOS 10.14. To be more specific, the issue of FreeGLUT and OpenGL 3+ (required by GLSL) can not be not fully resolved. Therefore, I used a open source framework [Glitter](https://github.com/Polytonic/Glitter), which used [GLFW](http://www.glfw.org/documentation.html) as UI library and support modern OpenGL in a plug-and-play fashion.



[Glitter](https://github.com/Polytonic/Glitter) features cross-platform reliability and is therefore recommended by [Learn OpenGL](https://learnopengl.com). Technically, it simply packs the relevant libraries and linked them properly. I used a different skeleton code with identical logic. Since only the UI library is different from the skeleton code provided, the logistic of core functions are quite similar.



### How you can test the project:

```bash
git clone --recursive https://github.com/Walter0807/CG-Assignment-1.git
cd Glitter
cd Build
```

Now generate a project file or makefile for your platform. If you want to use a particular IDE, make sure it is installed; don't forget to set the Start-Up Project in Visual Studio or the Target in Xcode.

```bash
# UNIX Makefile
cmake ..

# Mac OSX
cmake -G "Xcode" ..

# Microsoft Windows
cmake -G "Visual Studio 14" ..
cmake -G "Visual Studio 14 Win64" ..
...
```

Then put the glsl files to corresponding root directorty, compile and run.



Dependency: [cmake](http://www.cmake.org/download/)

You can check out the video [here](https://www.walterzhu.com/Resources/course_files/CGHW1Recording.mp4) (Download the video file).

If you have any questions, please [contact me](mailto:walterzhu@pku.edu.cn). I can also show it to you in person.

## Implementation

![](http://p3b1oqmkp.bkt.clouddn.com/67cbfb5399743d523c2470479592882a.jpg)

### Objects

- The flowers and the door are drawn using dots and lines.
- The plane (grass) and the road are drawn as a 2D object.
- The house is drawn as a combined 3D object.



### Interaction

- Use arrow keys or ASWD to move (translation).
- Use =(+) and - to scale.
- Use mouse to rotate.
- Use Esc to exit.



