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

# Hot reloading [04/04/24]
I diiiiiiiiiiiiiiiiiiiiiiiiiid it! Honestly what I thought would be a somewhat quick and easy task to include before I start working on adding 3D models from blender into my renderer was hot reloading. When working on basic lighting, I was getting quite annoyed having to constantly recompile my code just to see a difference in the vertex or fragment shaders, so before I continued I wanted to add this feature in so I could iterate on my shaders in a more painless manner. 

So I got the basic premise from [Anton Gerdelans](https://antongerdelan.net/opengl/shader_hot_reload.html) tutorial on hot reloading, and the basic algorithim is as follows:
1. Monitor the directory for wherever your shader files are
2. Once a change is detected created a new shader with the updated shader file 
3. If no errors were encountered when creating your new shader, destroy the old one
4. Replace it with your new shader!

I won't go into much more detail you can see the code in the repo, however, I did add one more step to the process copying something I noticed that Unity does when handling its shaders, I created a simple magenta shader for errors, and defined this shader as a sort of fallback error shader. If the program on start up or when noticing a change in a shader tries to load it into memory and encounters an error due to syntax or anything else, it simple uses this magenta shader instead so the program doesnt crash and I recieve the relevant information for the issue in the console. Its super handy! I wont even go into the hassle I had digging through the windows file api trying to get this to work *(I'll have to add hot reloading to work on mac and linux at some point but not now :/)*. Anyway here it is! 

![hot_reloading](https://github.com/JamieHyland1/OpenGL_Renderer/assets/15105021/1f5d18cb-ba42-4c1c-926a-38b418cb54cb)

I was also thinking of adding some sort of GUI to change uniforms and parameters in real time in the program, but after this I might just continue with [Learn OpenGL's](https://learnopengl.com/) tutorials, who knows?! I guess we'll find out in the next post :) If anyone actually reads this I appreciate it!

# Custom 3D models! [05/06/24]
So you may see the date of this post and the date of the last post and think *thats a long gap of time?* and youd be right! I had A LOT of issues with getting assimp, a 3d model loading library to work on my machine, I tried so many different options but every time I compiled the source I got different issues, anyway it left me feeling a bit dejected so I took some time off, I eventually got it working though! 

![multiple_models](https://github.com/JamieHyland1/OpenGL_Renderer/assets/15105021/bd241387-fafe-4d6d-8a70-da931707729e)

its not the fanciest of updates but this was the hardest thing I've done so far. I'm glad to have it done, although I did lose support for my lighting in the process but I will add that back in in the future!

# Post proecessing! [11/07/24]
So again another lengthy stretch between updates, I was quite sick with covid so excuse the absense! However! I have something fun to show, post processing! So if you're unaware post processing is the process of adding additional effects or techniques to an image before it reaches your screen! It usually involves some sort of improvement to the screen such as anti-aliasing, depth of field, bloom etc but I didnt't really add in any of that (yet ;) However! What I did manage to add was pretty neat if you dont mind me saying, so without further ado here it is: 
![CRT output](https://github.com/JamieHyland1/OpenGL_Renderer/assets/15105021/8fc5bb42-644f-413d-8ac1-42fe830e6cca)



Yes I decided to make a CRT shader! I really love the aestethic of old TV's and wanted to try recreate it now that I have frame buffers working! The main jist of the trick is that frame buffers allow me to rended the output of my scene onto a texture I then take that texture and apply it to a quad thats facing the camera, just like any other texture I can feed it into a shader and then do things to it to achieve certain effects. The main steps in this one are as follows:
![crt shader](https://github.com/JamieHyland1/OpenGL_Renderer/assets/15105021/ba1dda2c-aeda-4a68-a17f-f0e720efcbe6)
Apologies for the weird dancing around of the text! I'll try fix that in the future :D


# CubeMaps! [18/07/2024]

So this one is a bit shorter! Although I made something really cool this update so I'm quite happy :D This update was just getting cubemaps implemented in the renderer, they werent too hard to implement as I already had textures working, if you dont know what a cube map is, its essentially a giant texture that you wrap over a cube to create the sense of a world that your 3d objects live in, most cubemaps are just textures of skys and as they are usually the last thing drawn in a scene they are always far away and give an illusion of depth. However Skys are boring so I chose a city! 

![yokohama_cubemap](https://github.com/user-attachments/assets/eb1b5565-8f8c-47ea-9cd2-7bb5525155bd)

I chose a very nice cubemap of the city of Yokohama at night :D Then following [Learn OpenGL's](https://learnopengl.com/) tutorials, I added a sphere to my scene to test out sampling the cubemap for reflective surfaces which looked great! But I didnt stop there, I added my post processing effect from the last update to make a really cool vaporwave video!

![A_night_in_yokohama](https://github.com/user-attachments/assets/f274f92c-141e-44cf-bd22-fecf8adf9ee2)

Really happy with how this turned out! Its funny how something so simple can look so nice! The reception to it was also quite good! Very encouraging!