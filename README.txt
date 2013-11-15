LPC17xx-Drivers
===============

LPC17xx Driver Files and Template Structure for EPB_1768 Rev1.0 ARM Cortex M3 Board.

Development Platform : Eclipse Kepler
Compiler             : ARM GCC Toolchain
Platform             : Windows and Linux


How to Use:
1. Import LPC17xx Template from given setup

2. Clone Repository

  $ git clone https://github.com/dwijaybane/LPC17xx-Drivers.git

3. Copy the CM3 Core Files

4. Copy following files in your Header Files Section in workspace (Basic Setup)
   lpc_system_init.h
   lpc_types.h
   lpc17xx_clkpwr.h
   lpc17xx_gpio.h
   lpc17xx_nvic.h
   lpc17xx_pinsel.h
   lpc17xx_systick.h
   lpc17xx_wdt.h
   Other Peripherals Header Files as required

5. Copy following files in your Source Files Section in workspace (Basic Setup)
   lpc_global.c
   lpc_system_init.c
   lpc17xx_clkpwr.c
   lpc17xx_gpio.c
   lpc17xx_nvic.c
   lpc17xx_pinsel.c
   lpc17xx_systick.c
   lpc17xx_wdt.c
   Other Peripherals Source Files as required
   Your Main File

