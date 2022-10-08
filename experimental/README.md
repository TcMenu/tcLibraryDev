Danger Will Robinson. Danger.

This is an experimental hack of ST provided code to try and get LTDC working with BSP functions within Arduino.

Very heavily based on : https://github.com/STMicroelectronics/STM32CubeF4/

A few notes..

* https://github.com/STMicroelectronics/STM32CubeF4/blob/3d6be4bd406f275728e0a321cc371c62a3100533/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_fmc.c
* https://github.com/stm32duino/Arduino_Core_STM32/
* https://github.com/stm32duino/Arduino_Core_STM32/discussions/1624
* https://github.com/fpistm/Arduino_Core_STM32/commit/62a306275720a48e8833fb1eb83b549e50630e5a#diff-6ba5daa261970b3ca8185ad8825d4e14R93
* https://www.stm32duino.com/viewtopic.php?t=759
* https://github.com/monoclecat/STM32F4-discovery-lcd

Required adjustment to variant_generic.h

    //DJC adjustment - enable HAL_LTDC display
    #define HAL_LTDC_MODULE_ENABLED
    #define HAL_SDRAM_MODULE_ENABLED
    //DJC adjustment - enable HAL_LTDC display

in file
    
    .platformio\packages\framework-arduinoststm32\variants\STM32F4xx\F427Z(G-I)T_F429ZET_F429Z(G-I)(T-Y)_F437Z(G-I)T_F439Z(G-I)(T-Y)\variant_generic.h
