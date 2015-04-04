Final Project
CPE 471
Thomas Steinke

Overview:
---------
My final project is a Roller Coaster simulator that allows you to experience a roller coaster, in both the first- and third-person perspectives.

Build/Run:
----------
The code is built and tested using the Xcode native compiler on 64-bit Mac OS X. It's all built off of the provided Lab 6 project, but I made a lot of changes so some project reconfiguration will be required.
The object files for libnoise, which is used to generate camera shake, are located in libnoise/, and all required headers are included in include/.
A built version of the project is available at Build/, but I cannot guarantee that it will work anywhere other than on Max OS X.

Implementation:
---------------
My implementation draws on a lot of work we did with hierarchical modelling. To construct the track, I build a very long, almost hierarchical model-like structure of track pieces, each one connecting to its neighbor and transforming it accordingly.
To twist the track (the entire course is made with one straight track model), I used a variety of vertex transforms in my shader, using a "bend" vector to specify how to modify it. Every piece of track can be transformed in three ways (corresponding to the axis of rotation):
    x - Twist the track upwards (hill)
    y - Turn the track
    z - Twist the track laterally (corkscrew)
In addition, I use libnoise, in particular its Perlin Noise library, to generate realistic shake on the camera.

Controls:
---------
W/S - Move forward and backwards
A/D - Move left and right
E - Move up
Q - Move down

P - Load next track (3 total)
SPACE - Change perspective
   Either third-person, first car, or last car

Mouse - Look around