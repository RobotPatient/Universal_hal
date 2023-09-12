# Introduction

This is the documentation for the Universal hal framework. The Universal hal framework provides a generic cross platform interface to hardware peripherals/functions of microcontrollers.



There are multiple already existent software frameworks which do abstract away the hardware to a generic interface. The most well-known among these are: Arduino and Zephyr os. 



Both have their upsides and downsides. **This framework strives to achieve a middleground between these two frameworks**. 



## Existent frameworks

### Arduino

Arduino is one of the most well-known embedded projects in existence. It is very beginner friendly and works very well for quick prototyping.

One of the downsides with Arduino was the reliance on the BSP of the manufacturer and the messy codebase that comes with it. Also it has its own language based on c++, this of course requires a c++ compiler. Not every microcontroller manufacturer uses an architecture that supports c++.



### Zephyr os

Zephyr is an interesting project. As it provides a very lightweight (memory footprint) and efficient RTOS and hal with generic interface. 

To achieve this efficiency it uses overlay files (with custom build-system) for the hardware peripherals and connected devices. Although this approach provides good cross-platform compatibility because all hardware details are abstracted away behind a generic interface. It certainly has a steeper learning curve. Another disadvantage of zephyr is that it is targeted towards microcontrollers using a 32-bit architecture and comes with a RTOS along with it.



## Why?

The making of this framework started because of the struggles I experienced when using the START/ASF/Arduino framework on the Microchip SAMD series of microcontrollers. 

The START/ASF framework had all the functions needed for the project but had a very messy codebase and requires some tweaking to use it under Windows without the Microchip studio/MPLAB ide (only make, no CMake support).  

To mitigate this a CMake wrapper was built which used all sources and headers that were listed in the make file of the example projects. 

But then a question came from the project client: "can this library also be used with platformio?". The answer to that question would be no, use arduino. However the problem is that Arduino abstracts away the SAMD hal to a level that is not compatible/usable with the original ASF HAL. And in turn the Arduino framework doesn't implement all the features needed by this project. Zephyr OS was not even considered because of the steep learningcurve and the unconventional build-system.

If I would wrap ASF as a platform io library, I would have problems with symbols being redefined and the whole thing not compiling without a small rebuilt of the ASF HAL. Then I would end up with what [adafruit]([GitHub - adafruit/Adafruit_ASFcore: ASF core files for Zero](https://github.com/adafruit/Adafruit_ASFcore)) has done, taking the core files out of the ASF library and wrapping it into my own. However taking this route has risks, because if the manufacturer releases a new version of the ASF library I would have to update  the complete library again, risking API breaks and having a very messy code base. 

Instead I chose to do the thing every insane person would do :) Create my own HAL and abstractions. But try to make the drivers available to as many microcontroller architectures as possible with minimal performance overhead and compatibility for any toolchain/ide.

 
