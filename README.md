# END OF LIFECYCLE - END OF LIFECYCLE - END OF LIFECYLCE => visit http://www.remoteswitch.de

# Firmware for the UltraSwitch Project of http://www.remoteQTH.com
This is the firmware for the remoteQth.com Ultraswitch 
If you need help, feel free to contact DM5XX@gmx.de
Sketch is developed with PlatformIO

The idea:
Inspired from the megawebswitch project, which is a really powerful way to switch 63 sources (with on/off-Switches, grouped switches, latched switching with configurable time, switching restirctions and configurable excludes - using SD-Card for configuration), the ultraswitch was born.
The learning from the megawebswitch project was, there is too much overload with configuration and possibilities, a normal user does not need.

So, what are the main features of ultraswitch?
- use simple, chinese hardware (16x relay and mcp23017 pcbs) for setting up a modular system
- all is controlled by wifi using a cheap ESP8266-12F (aka wemos D1 mini)
- up to 8 moduls can be connected together (= 128 Relays :P)
- using i2c for connection, only 2 wires will be needed to expand the system
- Setup the System for first-time-use by Web-UI (connect to the wifi-router)
- Setup all IPs, Gateways, NumberOfBoardsToControl, DynDns-Adresses via a configuration-Page = After an initial firmware upload, no further updates on the firmware must be made for configuration. So if you get an set up ESP-12F from us, you configure the System by WebPages... No need to use Arduino IDE for configuration (for sure you can :P)

Following the most important design principle called seperation-of-concerns, UI is seperated from the hardware. Using most flexible web-based concepts, you can adjust the look and feel - or add more features whenever you want. All calls to the Hardware are made in a Level-2-RESTful way. While using web technologies, its easy to change (or easy to find someone who can help with the changes). From using on your local desktop to controlling all via your smartphone - its just on you and the way you set the configuration...

1.0:
As this is the official release, all basic functions are implemented. But the manual is still missing. All this will be added in the next weeks as the project will be part of the workshop for RRDXA at DM9EEs famouse "DX Hotel" in Holzminden, 27/28.01.2018



This is free software. You can use it under the terms of the Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)License if at least the version information says credits to remoteQTH.com :P

To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/
or send a letter to Creative Commons, 171 Second Street, Suite 300, San Francisco, California, 94105, USA.

Tribute to OL7M!
LLAP!
