# Physics Engine

### Design
At first, this project started as a way to learn how to build a game engine from scratch. It was developed using an ***ECS design pattern***. As I realized I wanted to implement dynamic physics into the game engine, I thought I might as well learn how physics engines work in order to add it to the game engine.  
This physics engine project uses the same ***ECS*** pattern.

### Purpose and goals
The purpose of this project is to build a physics engine from scratch. I want to learn the logic behind how physics engines work and to dive into the common code architectures and designs that are efficient for the engine. My ultimate goal is to have a decent and efficient 2D/3D physics engine that I can then use to develop simple games.

Additionally, my current goal does ***NOT*** include monetizing or competing with other game/physic engines (others are way better anyways). I just want to learn and understand what happens under the hood in these types of engines in order for myself to use them more efficiently. The best way to do that (in my opinion) is to build it from the bottom up.

### Disclaimer
At the moment, only 3D physics is supported. Once most of the 3D physics code is done, a 2D translation will be added.

# Resources
I am following this great and beginner friendly book: ***Game Physics Engine Development*** by Ian Millington to help with learning the physics engine structure.

For rendering, I am using **raysan5's** ***raylib*** library. It is fetched in the root level CMakeList.txt of this project. 


#### Ian Millington's GitHub (physics)
- https://github.com/idmillington/cyclone-physics.git

#### Raysan5's GitHub (rendering)
- https://github.com/raysan5/raylib.git