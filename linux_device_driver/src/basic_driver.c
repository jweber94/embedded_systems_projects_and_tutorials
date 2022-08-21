#include <linux/module.c>
#include <linux/init.h>

/* Meta information */
MODULE_LICENCE("GPL") // Only mandatory module property that needs to be set
MODULE_AUTHOR("Jens Weber")
MODULE_DESCRIPTION("Basic driver for the light sensor")

// This is the default function signatur that is invoked whenever the driver (aka kernel module) is loaded into the kernel ($ modprobe basic_driver)
static int __init OnModuleInit(void) {
    printk("Hello Kernel!\n");
    // Doing the initialization of the kernel module

    // return 1; // loading kernel module was not successful
    return 0; // Successfully loaded
}

// This function is invoked by the kernel if we unload it from the kernel ($ rmmod basic_driver)
static void __exit OnModuleExit(void){
    printk("Goodbey Kernel!\n");
}

// register the module init and module exit functions by the kernel
module_init(OnModuleInit);
module_exit(OnModuleExit);