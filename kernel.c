#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

static int __init custom_kernel_init(void) {
    printk(KERN_INFO "Custom Kernel Module: Initialization\n");
    return 0;  // Success
}

static void __exit custom_kernel_exit(void) {
    printk(KERN_INFO "Custom Kernel Module: Cleanup\n");
}

module_init(custom_kernel_init);
module_exit(custom_kernel_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Custom Kernel Module");
MODULE_VERSION("1.0");
