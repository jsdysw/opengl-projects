# OpenGL Projects
* With openGL, I made a swimming cube man model.
* Not only moving animation but also implemented texture and light effect(GOURAUD, PHONG).
* And finally render a realistic bag model with Cook-Torrance BRDF lighting. (physically based rendering, advanced rendering)

### Soure codes
* source codes are at /src.
* You can see the source code documentation report through swimming-man.pdf or swimming-man-texture-light.pdf file. (Language: Korean)

### How to Compile
* Whole visual studio project folders are at /vs-project-folder.

* OS type : Windows
* Used Language : ISO C++14 Standard
* Unzip the project file first and then open ".sln" file with Visual Studio 2017 -> click Debug tab -> click Start Without Debugging
* **swimming-man : debug, Win32**
* **swimming-man-texture-light : debug, x86**

# swimming man
<img src="./readme-images/swimming-man-left.png" width="350px" height="350px"/><img src="./readme-images/swimming-man-top.png" width="350px" height="350px"/>

* Keboard input 'c' shows the model from its left view point.
* Keboard input 'v' shows the model from its top view point.
* Keboard input 'q', quits the program.

* You can see the source code documentation report through swimming-man.pdf. (Language: Korean)

# swimming man texture, light

<img src="./readme-images/swimming-man2-top.png" width="350px" height="250px"/>

* Keboard input 'c' shows the model from its left view point.
* Keboard input 'v' shows the model from its top view point.
* Keboard input 'l' shows NO_LIGHT, GOURAUD, PHONG, NUM_LIGHT_MODE in order.
* Keboard input 'q', quits the program.

* You can see the source code documentation report through swimming-man-texture-light.pdf. (Language: Korean)

# Cook-Torrance Light(advanced rendering)

<img src="https://user-images.githubusercontent.com/76895949/154519399-d38e9ecb-10f0-45fd-ab2b-cd8d90f9525c.png" width="400px" height="400px"/>

* Load model file with texture map and calculate cook-torrance light.
* You can see the source code documentation report through advanced-rendering.pdf. (Language: Korean)
