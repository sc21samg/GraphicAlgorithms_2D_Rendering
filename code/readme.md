COMP3811 Coursework 1
=====================

Base project for Coursework 1 in COMP3811. Do not re-distribute the code
outside of Minerva.

## Updates

There are a few updates to the base project. In particular
- Command line flags to define window / framebuffer resolution
- Window is resizable

Command line flags:
--help          : print help and exit
--fbshift=N     : scale framebuffer resolution by 1/2^N relative to the window size
--geometry=WxH  : create window with width W and height H (default is 1280x720)

Note: the shift is unsigned. The application will not run if the shift is large
enough to reduce the framebuffer size below 1.

Examples:

$ bin/main-debug-x64-gcc.exe --help
(prints help and exits)

$ bin/main-debug-x64-gcc.exe --fbshift=1 
Creates a window with the default size 1280x720. Surface/framebuffer resolution
is 1/2 of the window's renderable area (so around 1280/2 x 720/2, minus
whatever space the OS's default window decorations take up). The surface's 
contents are scaled up to the window's size using nearest filtering.

$ bin/main-debug-x64-gcc.exe --geometry=1920x1080
Creates a window with size 1920x1080. 

$ bin/main-debug-x64-gcc.exe --geometry=1920x1080 --fbshift=1
Creates a window with the default size 1920x1080. Surface/framebuffer resolution
is 1/2 of the window's renderable area (so around 1920/2 x 1080/2, minus
whatever space the OS's default window decorations take up). The surface's 
contents are scaled up to the window's size using nearest filtering.


## Notes on tests

The included tests are a small subset of possible tests. They are not meant to
be exhaustive, but rather demonstrate some possible things that one can check
for the. The assignment asks you to come up with additional tests.

The included tests are also written in a way that leaves some ambiguity with
respect to implementations. The assignment doesn't specify all things (one
example is rounding modes), which are left up to the implementer. 

Once you have determined a certain way of drawing, you can perform more
selective testing. (E.g., is a specific pixel set to the expected color?)
