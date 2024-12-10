#!/bin/bash



# install dependencies

sudo apt-get update

sudo apt-get install libglfw3-dev libgl1-mesa-dev libglu1-mesa-dev libfl-dev libglew-dev freeglut3-dev -y



# compile

make clean

make



# run

./final



# remove installed libraries

sudo apt-get remove --purge libglfw3-dev libgl1-mesa-dev libglu1-mesa-dev libfl-dev libglew-dev freeglut3-dev -y