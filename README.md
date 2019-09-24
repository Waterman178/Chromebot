![Screenshot](https://i.imgur.com/xYjeKWN.jpg)
# Chromebot: Shitty color-based triggerbot

## Method

Chromebot uses GetPixel() in conjunction with GetCursorPos() to grab the color in the center of the crosshair.
Once the color is obtained we convert it to HSL color in order to have hue/saturation/brightness tolerance for
our comparator. This way we dont have to worry about the accuracy because we can adjust the tolerance to work
both in bright and dark environments unlike regular RGB color comparison. Afterwards we compare that color with
every color in the player model array and if it matches we simulate a click using mouse_event().

## Improvements

Chromebot was coded in a very short time span and thus it contains some bugs. Lots of improvements could be made
to make it more reliable and optimized. Some of them are...
- Add more accurate player model color scheme
- Further optimize the code to make it run smoother/faster
- Remove the use of mouse_event() by replacing it with a mouse hook or an Arduino
- Remove the use of GetAsyncKeyState() by replacing it with a keyboard hook
- Add a wider scan area for more accurate scans (might slow it down tho)
- Add an external .ini config file for storing your settings

## Features

- Fully customizable player model color scheme
- Accurate color comparison even in bright/dark areas due to HSL
- Doesnt inject in CSGO or open any handles to the game
- Simple adjustable config (key, tolerance, delays)

## How to use

Compile the project and inject Chromebot.dll into any running process besides CSGO.
