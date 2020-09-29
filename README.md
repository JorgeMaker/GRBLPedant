![GRBLPedant](doc/cncpendant.jpg?raw=true)
# - GRBLPedant - 

I have built my [GRBL controlled CNC milling machine](https://github.com/JorgeMaker/LlopisCNC) that runs on an Arduino board that I use to route and do my DYI projects.

I am quite satisfied with the outcome of my project, but one thing it lacked compared to more professional machines was a proper pendant so that the XYZ axes could be moved and zeroed without using a computer.

So, I decided to build one. I choose an ESP32 microcontroller to develop due to the large number of GPIO pins available. After some coding, board prototyping, PCB design, some soldering, and 3D printing, I was done with the design shown below.

The developed pedant allows fine movement in each of the three axes allowing the adjustment of the displaced distance each time a position of the rotary encoder is advanced and has a joystick that allows continuous movement in the XY plane

If you find it interesting, feel free to use my development to make your own pendant or incorporate elements of my design when designing your own pedant.

In this repository you can find the files to build your own pedant:

- PCB schematic in KiCad
- Code used to program the ESP32 microcontroller
- STEP files to print the database
