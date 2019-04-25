A lamp with a remote controller that can change its color and brightness.

A remote controller has two buttons. One button toggles the light, the other button increases the brightness. Pushing both buttons together changes the color.

A distant controller blinks the LEDs on the board when it send the packet, and the lamp blinks the LEDs when it recieves a packet from the remote controller.

A distant controller(zr) sends commands to a lamp(zc) by zigbee packets. Packets contain a char symbol. 0-toggle, 1-step up the brightness, 2-change the color. 

The lamp uses a RGB LED with common cathode! The pins are R-A8, G-A9, B-A10
