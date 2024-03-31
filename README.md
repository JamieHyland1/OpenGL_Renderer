# OpenGL_Renderer [05/02/24]
A 3D renderer built with OpenGL, I started this because I finished the pikuma 3D graphics course. It was a really fun test in building a 3d engine from scratch, so now Im going to try integrate OpenGL! Wish me Luck :')


# Hello Triangle [19/02/24]
So I started my journey into openGL by doing the old faithful "Hello Triangle". It actually took a longer to get up and running than I imagined. The tutorial uses GLFW for its setup and window initialization but I went with SDL2 as its what I used for my CPU based renderer and I found it hard to find tutorials whenever I got stuck. 

![Image of hello triangle](https://github.com/JamieHyland1/OpenGL_Renderer/assets/15105021/a14de6e5-ce82-4c30-971e-99ede4cee01f)


I wanted to do a bit more for my first commit in this project. So I decided to write a wrapper class to handle loading my vertex and fragment shaders with some functions to handle updating uniforms when running shaders. I'm excited to learn more and have plenty of ideas of things I want to try implement!


# Textures! [22/02/24]
So continuing on with [Learn OpenGL's](https://learnopengl.com/) tutorials on learning OpenGL I began learning about how to apply textures to vertices, this wasnt as difficult as I thought it would be, in my previous renderer filling a triangle with a color and then moving to applying textures to models with warping and the like was a big issue! (Im still scared of barycentric coordinates). Much like the shader struct, I made a texture struct along with a variety of helper functions in order to clean my code up a bit more.

![Image of luffy being repeated across a plane](https://github.com/JamieHyland1/OpenGL_Renderer/assets/15105021/9b8a1064-a0e2-46e1-8046-fc79a852b58d)


Once I got my textures loading properly I took some time to mess around with the various properties in the shaders I had written for this model, and made some weird effects! I had some experience writing shaders in Unity for various game jams and so I was getting in the groove with manipulating the vertex and fragment shaders! :)

# Transforms and camera and a series of cubes! [28/02/24]
A big update in this commit :D. Firstly I revised my transforms and coordinate systems, nothing really fancy to show there.
To make sure I had something nice to show in these updates I decided to go a bit further and add camera movement. To do that firstly, I needed a library to handle vectors and matrices. I had written a small vector and matrix library when writing my cpu based renderer but wanted to try using a more fully featured library. Because Im following [Learn OpenGL's](https://learnopengl.com/) tutorials they suggested using `GLM` but that library is written in `C++` and so I was forced to use a port of the library called `CGLM`. To be honest I'm not the biggest fan of it and how its structured so I might switch back to my small implementation (it was fun learning how to compile my own source code to use in my project though :D). Once I had that all successfully loaded I began rotating this cube around, you can see what its like below when you dont enable a depth buffer in  `OpenGL` its funny, what was a whole ordeal in my cpu based renderer was just added a simple flag to the beginning of my program. Its funny how much `OpenGL` will do behind the scenes for you, having write my own code to handle a depth buffer and discarding pixels that might overlap others otherwise I'm super greatful for this!

![silly cube](https://github.com/JamieHyland1/OpenGL_Renderer/assets/15105021/031acc4f-de59-4225-824b-844b73f8d7db)

Once I got the cube rotating I began writing my own camera class, again following [Learn OpenGL's](https://learnopengl.com/) tutorials they did things a bit differently than how I coded my own camera movement in my cpu renderer, It was nice to learn! I also messed around with drawing multiple instances of my cube, by putting `glDrawArrays` in a nested loop in my render function! Here are the results:

![finished cubes](https://github.com/JamieHyland1/OpenGL_Renderer/assets/15105021/583b6409-dcce-4f8c-883f-dc8313584da5)

With this I am finished the first getting started chapter of [Learn OpenGL's](https://learnopengl.com/) tutorials. Pretty happy with what I've got so far, I still have waaaaaaay more to go but I'm happy, progress is slow but steady. I think I still have to do some work on understanding `VAO's` and `VBO's` and all that, its a weird concept to get my head around if I'm honest. Next Ill start learning lighting >:D

# Lights Lights Lights! [31/03/24]
So took a break for a bit because work was hectic however! We. Are. Back! And this time I'm here to show off some dazzling lights! Honestly this was one of the funnest parts of the [Learn OpenG](https://learnopengl.com/) tutorials, felt finally like we had removed all the setup and now we're getting straight into shaders and fun things.
![First Light](https://github.com/JamieHyland1/OpenGL_Renderer/assets/15105021/2ae1dee7-a856-482c-a698-60dce90f276e)
There was a lot of tweaking shaders in this section of the tutorial, almost to an annoying degree lol having to recompile my code every time I altered a value in the lighting calculations was frustrating, I almost paused this section of the tutorials to implement a hot reloading system so I could see my changes to shaders on the fly, but I'll leave that to my next update!

![weird effect](https://github.com/JamieHyland1/OpenGL_Renderer/assets/15105021/39d83f8d-8acd-4493-b066-a810fe785d12)



I focused on implementing 3 main types of lights Directional lights (Sun), Point lights (light bulb), and spot lights(flash lights, lamps) and was surprised at how similar they are and at the same time completely different in the logic to implement them! You can see above an insteresting effect I ended up with when trying to implement the spotlight lol. I was finally able to get them all working, refactiored the code to make it easier to use and implemented a better first person controller and fixed a finicky bug with my fov!  

![final light scene](https://github.com/JamieHyland1/OpenGL_Renderer/assets/15105021/8c6e12b4-f540-4be4-94e8-2a7756138beb)

Next I think I'll focus on adding some much needed quality of life features if I'm to keep working on this renderer!


