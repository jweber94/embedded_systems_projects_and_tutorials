# Intro to linux driver programming

## Why do we need drivers?
+ Hardware has dedicated memory adresses that can only accessed by the processor in _service_ (in ARM terminology) or _kernel_ mode.
  - If we have an operating system like linux, we are disprevileged such that we can not access the hardware specific adresses of the processor directly!
  - Only the kernel can access these adresses!
+ A driver defines the interface between the user space and the hardware by delivering functionallities that are linked to system calls!
  - Therefore, the driver lives in kernel space and can access the hardware adresses, that are normaly blocked if we are programming in userspace
+ What the driver implementation does is to implement overloaded callbacks that are invoked by systemcalls
+ Advantages:
  - Since the systemcall defines the interface and the driver defines what should be done, if the callback is invoked, we have a restricted set of edge cases that need to be intercepted by the driver implementation. Therefore we can realize a valid and secure access to the hardware that is connected to our device.
+ What does a driver do?
  - Define the interface for interacting with the hardware
  - Keep track of the current state of the connected hardware. (Which might be needed to ensure a valid access to it)

## Setup kernel module development environment
+ First, we need to install the linux kernel headers.
  - On the rasperry pi, this is done by: `sudo apt install rasperrypi-kernel-headers`
  - On every _normal_ system, we can install it by typing `sudo apt install linux-headers-$(uname -r)`
  - This will install the build system for creating kernel modules that we need to use in order to create our kernel module. Also, it will install the source code that is needed to build and overload the kernel functions that we need to overload to program our driver
+ Then we need to create a makefile that uses our source code and compiles it with the kernel-header build system. See `./Makefile` for details

## Creating a basic kernel module
+ We need to include the `linux/init.h` that corresponds to the kernel for that we want to develop.
   - ***Tipp***: Have a look at the comments of these linux headers! These explain for example, why we need to add some keywords to the code!
+ It is important to note, that we need to add some keywords like `__init`, `__exit` or (in this tutorial not used) `__initconst`, `__initdata`, and so on.
  - *These keywords are makros that are defined within the linux kernel headers to place the code in specific memory areas within the kernel, to make the most efficient use of them!*
  - E.g.: `__init` is used to define the a function as a only one time executed init function. Therefore the kernel frees its memory after the initialization is sucessfully done with the corresponding function.
    * Under the hood is a linux kernel build system linker script that places code in specified memory areas!
+ A minimal kernel module needs to implement the `__init` and `__exit`, aka `module_init(init_callback)` and `module_exit(exit_callback)` functions and has the documentation functions:
  - `MODULE_LICENCE("GPL");`
  - `MODULE_AUTHOR("Jens Weber");`
  - `MODULE_DESCRIPTION("My driver");`
  - `MODULE_VERSION("0.0.0");`
+ See the example within `./minimal_driver/` 

## Loading a kernel module
+ To get an overview over all currently loaded/running drivers on your system, just type `$ lsmod` or `$ lsmod | grep <search_for_driver>`
+ To load a driver, we need to load the `<name>.ko` file (`.ko` stands for _kernel object_, which is a relocatable for the linux driver)
  - Loading can be done by:
	* `$ insmod <drivername>.ko`, which just loads the kernel module
	* `$ modprobe <drivername>.ko`, which loads the kernel module and all its dependend kernel modules
+ To see the kernel logs (where our `printk` statements are printed), we need to type `$ dmesg`
+ To unload a kernel module, we type `$ rmmod <drivername>`
### Permanently install a driver
+ By default, kernel modules are stored within the path `/usr/lib/modules/$(uname -r)/kernel/drivers/<drivername>`. You should place your driver here.
+ To load a module by default on every startup, we need to make an entry within `/etc/modules/load.d`
	- We are able to blacklist some modules or module versions within the modules.conf file or the `load.d` directory to avoid loading deprecated drivers that are still on the system or to avoid loading modules that might conflict with each other
	- This can also be done by `/etc/modprobe.d/blacklist.conf`
+ ***REMARK***: It is best practice to leave a comment why a kernel module is blacklisted in the line before the acutal blacklisting
+ To get info about a kernel module (author, version, ... everything that is written to the meta data), you can use `$ modinfo <drivername>` or `$ modinfo /path/to/driver.ko`

## References
+ Video 1: https://www.youtube.com/watch?v=lWzFFusYg6g&t=289s
+ Video 2: https://www.youtube.com/watch?v=LKHNHvDedf0
+ Video 3: https://www.youtube.com/watch?v=wB5ionPAFnk6L4&t=4s
+ https://www.tecmint.com/install-kernel-headers-in-ubuntu-and-debian/ (install linux kernel headers on a non-rasperry-pi system)
+ https://stackoverflow.com/questions/8832114/what-does-init-mean-in-the-linux-kernel-code
+ https://www.cyberciti.biz/faq/linux-how-to-load-a-kernel-module-automatically-at-boot-time/ (Loading kernel modules on default)

