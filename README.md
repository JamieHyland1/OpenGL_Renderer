# OpenGL_Renderer
A 3D renderer built with OpenGL, I started this because I finished the pikuma 3D graphics course. It was a really fun test in building a 3d engine from scratch, so now Im going to try integrate OpenGL! Wish me Luck :')


# Hello Triangle
So I started my journey into openGL by doing the old faithful "Hello Triangle". It actually took a longer to get up and running than I imagined. The tutorial uses GLFW for its setup and window initialization but I went with SDL2 as its what I used for my CPU based renderer and I found it hard to find tutorials whenever I got stuck. 

![Image of hello triangle](https://github.com/JamieHyland1/OpenGL_Renderer/assets/15105021/a14de6e5-ce82-4c30-971e-99ede4cee01f)


I wanted to do a bit more for my first commit in this project. So I decided to write a wrapper class to handle loading my vertex and fragment shaders with some functions to handle updating uniforms when running shaders. I'm excited to learn more and have plenty of ideas of things I want to try implement!


# Textures! 
So continuing on with [Learn OpenGL's](https://learnopengl.com/) tutorials on learning OpenGL I began learning about how to apply textures to vertices, this wasnt as difficult as I thought it would be, in my previous renderer filling a triangle with a color and then moving to applying textures to models with warping and the like was a big issue! (Im still scared of barycentric coordinates). Much like the shader struct, I made a texture struct along with a variety of helper functions in order to clean my code up a bit more.

![Image of luffy being repeated across a plane](https://github.com/JamieHyland1/OpenGL_Renderer/assets/15105021/9b8a1064-a0e2-46e1-8046-fc79a852b58d)


Once I got my textures loading properly I took some time to mess around with the various properties in the shaders I had written for this model, and made some weird effects! I had some experience writing shaders in Unity for various game jams and so I was getting in the groove with manipulating the vertex and fragment shaders! :)
