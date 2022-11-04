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

## Create a device file driver
### What is a device file driver?
+ There are three kinds of linux drivers that you can create:
  - char modules: Char drivers are file drivers, which is the most common way to implement a hardware driver!
  - block modules: Different kind of driver
  - network modules: Different kind of driver
+ The most popular kind of driver is the file driver:
  - Since the linux philosophy says "everything is a file", we can interact with the kernel functionality by reading and writing to that driver file.
  - Be careful: Driver files are not _real_ files! Reading and writing to that file will ***NOT*** result in a persistend entry within that _file_! It will be the input/output to the callback function that is linked by an systemcall that is invoked by the user (space program)!

### Creating a file for the interaction
+ The file for the interaction between the user space and the kernel module can be created in different ways and is (in parts) dependend on which interface should be used!
+ There are two main interfaces:
  * A file in the `/proc` filesystem
  * A device file in the `/dev` filesystem
#### Creating files within the `/dev` filesystem
+ There are two ways to create an interaction file within the `/dev` filesystem:
  * Using the terminal: 
    - We can create a file for the interaction by using the `$ mknode` command
    - Alternatively, we can create the file within the `__init` function of the kernel module. For details, see [8].
  * If we use a file within the `/dev` directory, we need to assign it a major and minor device file number. See [9] for details.
  * Tipp: If we create a device file within `/dev`, we could make a GPIO listen if there is a _high_ level on the pin and if that is the case, we create the device file
+ ***We use the `/dev` char driver, if we want to connect to a real, physical device***

#### Creating files within the `/proc` filesystem
+ ***We choose a file within the `/proc` filesystem, if we want to access system information or request functionallities that are done by the chip/processor itself!***
+ There is a defined interface for the `/proc` filesystem that we can lookup with `linux/proc_fs.h` under the name `proc_ops`, see [10].

#### Interacting with the user by copying data between user space and kernel space
+ To read and write data from and to the kernel space, we need to invoke the kernel functions `copy_to_user()` and `copy_from_user()`, which are defined in [11] and [12]
  * Make sure that you allocate a static data buffer that you reset every time we invoke `copy_from_user` by zeroing it out (e.g. with memset [13])
  * Make sure to sanity check user input to fit into the data buffer!
+ After we have implemented the read and write functions accoring to the function signature that is given by `proc_ops`, we need to link them to the instance of the `proc_ops` struct!
  * Make sure that you have proper forward declaration!
+ After the `proc_ops` instance is created, we need to link it to a file within the `/proc` folder. Therefore, we need to create one!
  * This can be done by `proc_create()` function [14] within the `__init` function of the kernel module!
+ The resulting pointer from `proc_create()` needs to be assigned to the static `proc_dir_entry` struct [15] of the kernel module!
  * With this handle, we are able to delete the file within the `/proc` directory if the `__exit` function of the kernel module is called
+ ***See `./proc_fs/my_gpio_driver.c` for more implementation details!***


#### Creating the real driver for the GPIO interface
+ See `./final_gpio_driver/my_gpio_driver.c` for more details
+ The main takeaways are:
  - Look at the kernel header documentation and the given function signatures to hand back the demanded values that needs to be calculated by your driver logic! (E.g. the read and write callback for the `proc_ops` object need to ssize_t the amount of bytes that were written or read out by the request from the user space.)
  - Fancy bit banging and binary masking is a good way to setup register values
  - Define a valid interface and check it within your driver code such that the user space request could not be processed if it is not well formated
  - Physical adresses needs to be remaped to kernel space, because linux uses a MMU, see [16],[17] for details
    * This will get you back the virtual adress that the MMU has mapped to the physical adress  

## References
+ [1] Video 1: https://www.youtube.com/watch?v=lWzFFusYg6g&t=289s
+ [2] Video 2: https://www.youtube.com/watch?v=LKHNHvDedf0
+ [3] Video 3: https://www.youtube.com/watch?v=wB5ionPAFnk6L4&t=4s
+ [4] https://www.youtube.com/watch?v=juGNPLdjLH4
+ [5] https://www.tecmint.com/install-kernel-headers-in-ubuntu-and-debian/ (install linux kernel headers on a non-rasperry-pi system)
+ [6] https://stackoverflow.com/questions/8832114/what-does-init-mean-in-the-linux-kernel-code
+ [7] https://www.cyberciti.biz/faq/linux-how-to-load-a-kernel-module-automatically-at-boot-time/ (Loading kernel modules on default)
+ [8] https://stackoverflow.com/questions/49350553/can-i-call-mknod-from-my-kernel-module
+ [9] https://www.youtube.com/watch?v=h7ybJMYyqDQ
+ [10] https://elixir.bootlin.com/linux/latest/source/include/linux/proc_fs.h
+ [11] https://www.ibm.com/docs/en/i/7.1?topic=functions-memset-set-bytes-valuehttps://www.cs.bham.ac.uk/~exr/lectures/opsys/13_14/docs/kernelAPI/r4037.html
+ [12] https://www.kernel.org/doc/htmldocs/kernel-api/API---copy-to-user.html
+ [13] https://www.ibm.com/docs/en/i/7.1?topic=functions-memset-set-bytes-value
+ [14] https://elixir.bootlin.com/linux/latest/source/fs/proc/generic.c#L582
+ [15] https://elixir.bootlin.com/linux/latest/source/fs/proc/internal.h#L30
+ [16] https://os.inf.tu-dresden.de/l4env/doc/html/dde_linux/group__mod__res.html#gd8fce5b58ae2fa9f4158fe408610ccc5
+ [17] https://www.thegeekdiary.com/what-is-ioremap/