## ESP32 IDF support

In this folder you can find the ESP32 IDF support. It is based on the ESP-IDF 5 with Cmake support.

The main example presently is the tcmenu_arduino_esp32 example which is based on the hello_world example with a 
menu added.

For you own projects you need to install the library components into the components folder. You do this using the
python script in this folder as follows: `generateCmake.py -l libDirectory -o componentsDirectory`. Where lib directory
is the directory containing the libraries, and componentsDirectory is the directory where the components should be created.
Normally this is in the project/components directory.

I typically run the above script in both the cmakeProject/lib and cmakeProject/mbed_libs folders so I get the core
libraries and the extra libraries intended for outside Arduino projects..

Once you've done this, and run through the regular getting started steps, you should be able to build and run the 
example.