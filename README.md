# Retro Dynamics
Retro Dynamics is a GLFW-based game engine written in C. The primary target of Retro Dynamics is games based on pixel art, and as such certain kinds of optimizations can be made (e.g. packing large amounts of sprites in to a single spritesheet).

Games built with Retro Dynamics are meant to be a single executable file, which means that it relies on certain infrastructure, such as [jot](https://github.com/rdynamics/jot), which is used to pack images into a spritesheet and into, essentially, the executable itself.
