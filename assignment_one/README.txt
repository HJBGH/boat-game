I found the control specs outlined on the assignment page to be inconsistent
and in some cases incomplete or nonsensical. Below is a list of control 
bindings that work for my submission. Please consult "keyboard.c" for more 
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
Boat Movement:
    a - move left boat left
    d - move left boat right
    l - move right boat right
    j - move right boat left
Cannon Controls:
    f - rotate island gun left
    h - rotate island gun right
    g - fire island gun
    b - fire island defense
    O - elevate right boat's gun
    o - depress right boat's gun
    i - fire right boat's gun
    8 - fire right boat's defense
    Q - depress left boat's gun
    q - elevate left boat's gun
    e - fire left boat's gun
    3 - fire left boat's defense

I should also explain some incomplete/poorly implemented features. 
The first is wave pausing; it doesn't resume animation properly, instead the 
waves jump to where they would have been had they not been paused. 
Cannon balls are not drawn as filled circles, just circular lines. 
Additionally; Most of the code surrounding the cannons and their features could
be refactored, perhaps into a cannon struct, with associated functions.
