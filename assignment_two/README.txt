Below is a list of control bindings that work for my submission. 
Please consult "keyboard.c" for more 
details.

Quit:
    esc
Wave Options:
    n - toggle normals
    t - toggle tangents
    w - turn on wireframe mode
    p - turn on polygone mode (turns off wireframe mode)
    ' - pauses wave animation
    + - increases wave tesselation
    - - decreases wave tesselation
Cannon Controls:
    a - rotate island gun left
    d - rotate island gun right
    g - fire island gun
    b - fire island defense
    w - elevate right boat's gun
    s - depress right boat's gun

Most of the features of this project are missing due to time mismanagement.
The boats have no AI, there's no scoring system, boat's cannot fire and their
cannons are never loaded. Tracking the position of the boats barrel is in
an incomplete form on another branch, but is not present in this submission.
Collision detection between cannonballs and boats is
likewise unimplemented. Terrain is flat, with no texture. There is no
spritework. The OSD relies on text only, with no health bar.
Pausing the wave also doesn't work properly;

Skybox texture source:
931.de/p/free-skybox-texture-set/
