//Reference: https://medium.com/@lee1003094395/adding-a-system-call-which-can-pass-a-userspace-string-b245105bed38
#include <linux/kernel.h>
#include <linux/linkage.h>
#include <linux/syscalls.h>
#include <linux/uaccess.h>

SYSCALL_DEFINE2(upcase, char __user *, str,  int, length)
{
    char a[512] = "";
    int i = 0;
    if (copy_from_user(a, str, length))
    {
        return 1;
    }
        
    while (i<length)
    {
        if ('a' <= a[i] && a[i]<= 'z') a[i] = a[i] - 32;
        ++i;
    }
    
    if (copy_to_user(str, a, length))
    {
        return 1;
    }
    return 0;
}
