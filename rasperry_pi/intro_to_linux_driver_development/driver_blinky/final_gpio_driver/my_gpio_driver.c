#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>

#include <linux/proc_fs.h>
#include <linux/slab.h>

#include <asm/io.h>

#define BUFFER_SIZE 1024
#define BCM2837_GPIO_ADDRESS 0x3F200000 // here lives the start of the GPIO registers

static struct proc_dir_entry * proc_dir_handle = NULL; // handle to the proc file inteface that needs to get an file handle assigned to it on initialization
static char data_buffer[BUFFER_SIZE + 1] = {0}; // user interaction data buffer

static unsigned int * gpio_registers_addr = NULL; // placeholder for the address that is mapped through the MMU to get the start register of the GPFSEL registers

static void gpio_pin_on(unsigned int pin) {
  
  /* Getting the demanded bits of the GPIO registers */
  unsigned int gpfsel_index_n = pin/10;
  unsigned int fsel_bit_pos = pin%10;
  unsigned int * gpfsel_n = gpio_registers + gpfsel_index_n; // Get the correct GPFSEL register
  
  unsigned int * gpset_register = (unsigned int *)((char *) gpio_registers_addr + 0x1c); // the gpset_n register has the offset 0x1c from the gpfsel registers

  /*  */
  // setting all of the pin function selection bits to zeros
  *gpfsel_n &= ~(7 << (fsel_bit_pos*3)); // 7 is 111 in binary, 111 is left shifted by the bit position and then inverted, so 000; The &= is a bitwise comparison and therefore sets every bit to zero
  // setting the new value to the gpio (001 but with LSB)
  *gpfsel_n |= (1 << (fsel_bit_pos*3));

  /* Set the gpio register gpset_n to on */
  *gpset_register |= (1<<pin);
}

static void gpio_pin_off(unsigned int pin){
  unsigned int * gpio_off_register = (unsigned int*)((char *)gpio_registers_addr +0x28);
  *gpio_off_register |= (1 << pin);
  return;
}

ssize_t proc_read_cb(struct file * file, const char __user *user, size_t size, loff_t off){
  // read syscall callback - This is a demanded function signature by struct proc_ops - it needs to return how many bytes are written to the user
  return copy_to_user(user, "Hello!\n", 7) ? 0 : 7;
}

ssize_t proc_write_cb(struct file* file, const char __user *user, size_t size, loff_t *off) {
  // write syscall callback - This is a demanded function signature by struct proc_ops - it needs to return how many bytes are written to the user
  unsigned int pin = UINT_MAX;
  unsigned int value = UINT_MAX;

  // reset the from-user data buffer
  memset(data_buffer, 0x0, sizeof(data_buffer));

  // cut the user input to the maximum buffer size if it would exceed it
  if (size > BUFFER_SIZE) {
    size = BUFFER_SIZE;
  }

  // If copy_from_user goes wrong (aka it return 0 bytes are readed out), we have read out 0 bytes and return
  if (copy_from_user(data_buffer, user, size)) {
    return 0;
  }

  printk("Databuffer: %s\n", data_buffer);

  // Check if we received a valid data format from the user input - scanf takes data and reformats it - on sucess, it will return the number of items that were sucessfully read (https://www.tutorialspoint.com/c_standard_library/c_function_scanf.htm)
  if (scanf(data_buffer, "%d%d", &pin, &value) != 2) {
    printk("Inproper data format submitted!\n");
    return size;
  }

  // we can only use pins between pin 0 and 21 with our driver
  if (pin > 21 || pin < 0) {
    printk("Invalid pin number %d submitted!\n", pin);
    return size;
  }

  // we can only set the pin on (value=1) or off (value=ß)
  if (value != 0 && value != 1) {
    printk("Invalid value %d for setting the pin (1) on or off (0)", value);
    return size;
  }

  printk("Setting pin %d to value %d", pin, value);
  if (value == 1) {
    gpio_pin_on(pin);
  } else {
    gpio_pin_off(pin);
  }
  return size;
}

static const struct proc_ops proc_fops = {
  .proc_read = proc_read_cb,
  .proc_write = proc_write_cb
};

// __init is a makro defined within linux/init.h that marks this function as a one time execution for the kernel. The kernel will tread the memory for this function in a special way (freeing as fast as possible) to be as efficient as possible
static int __init gpio_driver_init(void) {
  printk("Loading the gpio driver");
  
  // Assign the physical adress through the MMU (Memory Management Unit) to the variable to access it in the write functions
  gpio_registers_addr = (int*)ioremap(BCM2837_GPIO_ADDRESS, PAGE_SIZE); // https://os.inf.tu-dresden.de/l4env/doc/html/dde_linux/group__mod__res.html#gd8fce5b58ae2fa9f4158fe408610ccc5
  if (gpio_registers_addr == NULL) {
    printk("Failed to map GPIO memory to driver");
    return -1;
  }
  printk("Successfully mapped the physical GPIO memory through the MMU");

  // Create the file within the /proc directory with the name gpio_interface and assign it to the global driver object for accessing it during read and write systemcalls
  proc_dir_handle = proc_create("gpio_interface", 0666, NULL, &proc_fops);
  if (proc_dir_handle == NULL) {
    printk("Something went wrong during the proc file creation!\n");
    return -1;
  }

  printk("GPIO driver loaded sucessfully\n");
  return 0;
}

// __exit is another makro that is defined within linux/init.h
static void __exit gpio_driver_exit(void) {
  printk("Remove the gpio driver...");
  iounmap(gpio_registers);
  proc_remove(proc_dir_handle);
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
