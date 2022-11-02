#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>

// __init is a makro defined within linux/init.h that marks this function as a one time execution for the kernel. The kernel will tread the memory for this function in a special way (freeing as fast as possible) to be as efficient as possible
static int __init gpio_driver_init(void) {
  printk("GPIO driver loaded sucessfully\n");
  return 0;
}

// __exit is another makro that is defined within linux/init.h
static void __exit gpio_driver_exit(void) {
  printk("GPIO driver unloaded sucessfully\n");
  return;
}

// linking the __init and __exit funtions to the module events init (invoked by $ insmod or $ modprobe) and $ rmmod
module_init(gpio_driver_init);
module_exit(gpio_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jens Weber");
MODULE_DESCRIPTION("GPIO driver to turn an LED by pin on and off");
MODULE_VERSION("0.0.0");
