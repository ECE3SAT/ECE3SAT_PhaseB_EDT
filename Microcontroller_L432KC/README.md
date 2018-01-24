# ECE3SAT_PhaseB_EDT

This code has been wrote for STM32 Nucleo L432KC.

## Prerequisites

You need to have an installed and configured [MbedCLI](https://os.mbed.com/docs/v5.7/tools/arm-mbed-cli.html).


## Installation

Use `mbed new .` in this directory to create a Mbed project with all configurations and dependencies in this repository.

You can now use mbed command to compile these files to get the binary to flash your microcontroller
in <project_dir>/BUILD/<target_name>/<compiler_name>/ :  `mbed compile`

In our case, we used `mbed compile -m NUCLEO_L432KC -t GCC_ARM` to deal with NUCLEO_L432KC with a GCC_ARM compiler
(see [MbedCLI configuration](https://os.mbed.com/docs/v5.7/tools/configuring-mbed-cli.html) page to know which parameters you should use)
