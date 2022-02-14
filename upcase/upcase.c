#include <linux/kernel.h>
#include <linux/syscalls.h>

SYSCALL_DEFINE2(upcase, char *, str, int, n)
{
    printk("Converting string\n");
    int i = 0;
    while (i<n)
    {
        if ('a' <= str[i] && str[i]<= 'z') str[i] = str[i] - 32;
        printk("%s\n",str);
        ++i;
    }
    return 0;
}
