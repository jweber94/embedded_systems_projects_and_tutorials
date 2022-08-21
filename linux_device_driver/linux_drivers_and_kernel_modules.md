# Basics about linux device driver development
+ Uses: ***Rasperry Pi 4***
+ The following information are mainly based on [this](https://www.youtube.com/playlist?list=PLCGpd0Do5-I3b5TtyqeF1UdyD4C-S-dMa) youtube playlist but has some additional information that I have googled while doing this tutorial

## Restrictions in kernel development
+ If you want to develop a linux device driver, you need to develop in the "kernel space", which is more restricted and you do not have access to all development libraries and capabilities. The main drawbacks are:
    - You have less memory on the stack and heap
    - You do not have access to floating point variables
    - You can not include all std-libraries

+ If you want to develop a valid driver for the kernel, you need to include a few headers to your C-program
    - `linux/module.h`
    - `linux/init.h`
    - You need to set the `MODULE_LICENCE` variable to write a useable linux device driver

+ To build your kernel module, you need to use the kernel makefile (which is located in `/lib/modules/$(uname -r)/build/Makefile`) to compile your module
    - And you need to append the object file of your driver `.c` file to the build variable `obj-m`!
    - See `Makefile` as an example
    - ***The actual kernel module is the `<modname>.ko` file!***

+ After you have build your custom module, you can load it with `$ insmod <modname>` and unload it with `$ rmmod <modname>`
    - You can see the kernel logging with `$ dmesg` or with the log file in `$ tail -f /var/log/dmesg` (or on the rasperry with `$ tail -f /var/log/kernel.log`)
    - `$ dmesg`: _Diagnostic messages_ of the kernel

## Connecting
+ (External) devices in linux have a character type (disk, block, ...), a major number and a minor number
    - major number: describes the used driver
    - minor number: describes the device instance
+ 