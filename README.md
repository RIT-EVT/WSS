# REV3-WSS

## Introduction
The Wheel Speed Sensor (WSS) is a PCB that senses the RPM of wheels on the bike and calculates the speed of
the bike. It uses sensors in both the front and back wheels. It calculates the RPM of each wheel by using
a hall effect sensor and a magnet in each wheel to get a time interval of how long it takes for a wheel to
make a full rotation. This is then used to calculate RPM, and then RPM is used with the radius of the wheel
to calculate the speed of the bike in miles per hour. The WSS then broadcasts the data on the CAN network.
