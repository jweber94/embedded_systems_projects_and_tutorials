# Add the my_gpio_driver.o object file to the targets that the linux kernel driver build system want to create
obj-m += my_gpio_driver.o

# Set the variable (only for this Makefile) KDIR to the path where the linux kernel driver build system is located
KDIR = /lib/modules/$(shell uname -r)/build

all:
	# -C /path/to/dir changes the path where we want to build our code, before the actual build process is started
	# M= is NOT a parameter to make, it is a variable that is available during the build process by make - This is mostly (in case of linux driver development with the linux kernel header build system) done to return to the directory where the build process is started from, after the build is finished or IN THIS CASE: Where to store the kernel module binarys after the build process
	# The modules at the end of the make command tells the kernel build system (which builds upon make) which target should be build - The modules target contains the obj-m that is set on top of that makefile
	make -C $(KDIR) M=$(shell pwd) modules # invoke the make linux header build system with its modules target

clean:
	make -C $(KDIR) M=$(shell pwd) clean # invoke the make linux header build system with its clean target
