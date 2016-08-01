# LearnCinderGL
A series of tutorials designed to get folks up and running with Cinder (glNext/0.9.0)

## UNDER CONSTRUCTION
![Header Image](https://lh3.googleusercontent.com/-j50yAJ2gew0/VXH_nF5fkNI/AAAAAAAAB48/NPwQ8ivbY-o/s800/cindertuts.png)

###Setup for XCode Users:
Place these in your cinder folder inside another directory - such as: `Cinder_Root_Directory/samples`

So the path to the XCode file for L01_Camera would be: `Cinder_Root_Directory/samples/LearnCinderGL/L01_Camera/xcode/L01_Camera.xcodeproj`

If you want to put the apps in another location, you just need to change this in your project Build settings - all the way at the bottom of the "Build Settings" tab on the project settings, there is a variable called `CINDER_PATH` it is currently set to `../../../..	` indicating that the path to Cinder's libraries are 4 directories away - if you want to nest these deeper or more shallow, just change this...