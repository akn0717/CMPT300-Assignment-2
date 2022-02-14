#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>

 
int main(int argc, char *argv[])
{
    char str[] ="Hekki";
    int n = 5;
    printf("Converting string\n");
    for (int i=0;i<n;++i)
    {
        if ('a' <= str[i] && str[i]<= 'z') str[i] = str[i] - 32;
        printf("%s\n",str);
    }
    return 0;
}