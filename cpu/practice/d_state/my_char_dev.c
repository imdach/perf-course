#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/sched.h>
#include <asm/uaccess.h>

#define DEVICE_NAME "mychardev"

int major_num;
static struct class *my_class_class;

static char msg[] = "hello, world!\n";
char *p;

/**
 * 设备文件打开的回调
 */
static int my_device_open(struct inode *inode, struct file *file) {
    printk("%s\n", __func__);

    // 使用一个静态的局部变量，记录设备文件打开的次数, 每次 cat，这个 counter 加一
    static int counter = 0;
    if (counter == 2) {
        __set_current_state(TASK_UNINTERRUPTIBLE);    //改变进程状态为睡眠
        schedule();
    }

    p = msg;
    ++counter;
    return 0;
}

static int my_device_release(struct inode *inode, struct file *file) {
    printk("%s\n", __func__);
    return 0;
}

/**
 * 处理 cat 等读取该设备文件的逻辑，返回 "hello, world!" 字符串到用户终端输出
 */
static ssize_t my_device_read(struct file *file,
                              char *buffer, size_t length, loff_t *offset) {
    printk("%s %u\n", __func__, length);

    int bytes_read = 0;
    if (*p == 0) return 0;
    while (length && *p) {
        put_user(*(p++), buffer++);
        length--;
        bytes_read++;
    }
    return bytes_read;
}

static ssize_t my_device_write(struct file *file,
                               const char *buffer, size_t length, loff_t *offset) {
    printk("%s %u\n", __func__, length);
    return length;
}

struct file_operations fops = {
        .owner = THIS_MODULE,
        .open = my_device_open,
        .release = my_device_release,
        .read = my_device_read,
        .write = my_device_write,
};

/**
 * 内核模块初始化
 */
int my_module_init(void) {
    printk("my module loaded\n");
    // register_chrdev 函数的 major 参数如果等于 0，则表示采用系统动态分配的主设备号
    major_num = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_num < 0) {
        printk("Registering char device failed with %d\n", major_num);
        return major_num;
    }
    // 接下来使用 class_create 和 device_create 自动创建 /dev/mychardev 设备文件
    my_class_class = class_create(THIS_MODULE, DEVICE_NAME);
    device_create(my_class_class, NULL, MKDEV(major_num, 0), NULL, DEVICE_NAME);
    return 0;
}

/**
 * 内核模块卸载
 */
void my_module_exit(void) {
    device_destroy(my_class_class, MKDEV(major_num, 0));
    class_destroy(my_class_class);
    unregister_chrdev(major_num, DEVICE_NAME);
    printk("my module unloaded\n");
}

module_init(my_module_init);
module_exit(my_module_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Arthur.Zhang");
MODULE_DESCRIPTION("A simple char device driver");