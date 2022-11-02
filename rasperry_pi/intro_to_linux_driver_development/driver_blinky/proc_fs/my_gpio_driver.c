#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>

#include <linux/proc_fs.h>
#include <linux/slab.h>

#define MAX_USER_INPUT_SIZE_GPIO 1024

// global available within the kernel module
static struct proc_dir_entry * gpio_proc_file = NULL;
static char data_buffer[MAX_USER_INPUT_SIZE_GPIO];

/* 
 * Create read and write function based on the proc_ops datastructures method signatures, 
 * see https://elixir.bootlin.com/linux/latest/source/include/linux/proc_fs.h for details 
 * The functions need to be implemented before we initialize the static prof_ops instance
 * because of forward declaration!
 */
ssize_t gpio_read(struct file * file, char __user * user, size_t size, loff_t *off) {
  copy_to_user(user, "Hello for the GPIO driver!\n", 27); // see https://www.kernel.org/doc/htmldocs/kernel-api/API---copy-to-user.html for function signature and details
  return 27;
}

ssize_t gpio_write(struct file * file, const char __user * user, size_t size, loff_t *off) {
  // reset the buffer with everything is zero
  memset(data_buffer, 0x0, sizeof(data_buffer));

  // validate input length from the user space and if it exceeds, cut it back to the buffer size
  if (size > MAX_USER_INPUT_SIZE_GPIO) {
    size = sizeof(MAX_USER_INPUT_SIZE_GPIO);
  }

  // get data from the userspace to the kernel modules data buffer
  copy_from_user(data_buffer, user, size); // see https://www.cs.bham.ac.uk/~exr/lectures/opsys/13_14/docs/kernelAPI/r4037.html for function signature and details 
    
  printk("Received the following message from the user: %s\n", data_buffer);
  return size;
}

/* Create the interface instance for reading and writing from and to user space /proc filesystem */
static const struct proc_ops proc_fops = {
  .proc_read = gpio_read,
  .proc_write = gpio_write
};

// __init is a makro defined within linux/init.h that marks this function as a one time execution for the kernel. The kernel will tread the memory for this function in a special way (freeing as fast as possible) to be as efficient as possible
static int __init gpio_driver_init(void) {
  printk("Initializing the GPIO kernel driver");
  
  // create an entry within the /proc filesystem and link it with the static proc_fops kernel datastructure that defines links the functions to the file datastructure
  gpio_proc_file = proc_create("gpio_fd", 0666, NULL, &proc_fops); // see https://elixir.bootlin.com/linux/latest/source/fs/proc/generic.c#L582 for function signature and details
  // sanity check if we created that file successfully
  if (gpio_proc_file == NULL) {
    printk("Could not create an inteface file. Initialization failed!");
    return 1;
  }

  printk("GPIO driver loaded sucessfully\n");
  return 0;
}

// __exit is another makro that is defined within linux/init.h
static void __exit gpio_driver_exit(void) {
  printk("Uninitializing the GPIO kernel driver");
  proc_remove(gpio_proc_file);
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
