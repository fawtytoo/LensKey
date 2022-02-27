# LensKey
Lenslok cracker

This is based on Lenskey by Simon Owen (https://simonowen.com/spectrum/lenskey/)

Created to be OS independent using the SDL2 library.

## Usage

lenskey GAME IMAGE

GAME is one of 9:

1. ACE (Air Combat Emulator)
1. Art Studio
1. Elite
1. Graphic Adventure Creator
1. Jewels of Darkness
1. Mooncresta
1. Price of Magik
1. Tomahawk
1. TT Racer

Use the index number of the game, not the name.

IMAGE is the saved screenshot you have of the Lenslok code.

1. Move the image within the window using the mouse and place the Lenslok code at the top-left corner.
2. Reduce the size of the window to enclose just the right hand side of the Lenslok code.
3. Press ENTER to reveal the code.
4. If the code is not right, adjust the window size right or left. Then repeat step 3.

LensKey will save a configuration file for the games to remember the last window size and image position to ease future loading.

## Disclaimer
This has only been tested with Elite and Art Studio, due to lack of screenshot examples.
