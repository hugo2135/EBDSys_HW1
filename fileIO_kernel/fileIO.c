#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>  //filp_open inside
#include <linux/buffer_head.h>
#include <linux/string.h>
#include <asm/segment.h>
#include <asm/uaccess.h>

MODULE_DESCRIPTION("fileIO");
MODULE_LICENSE("GPL");

#define LEN 260

static int fileIO_init(void){
    char buf[LEN];
    struct  file *fp;
    struct  file *fp_out;
    int ret;
    
    printk("--------------");
    printk(KERN_INFO "File accessing\n");
    fp = filp_open("input.txt", O_RDONLY, 0);
    fp_out = filp_open("output.txt", O_CREAT|O_RDWR, S_IROTH);

    if(fp != NULL){
        memset(buf, 0, LEN);
        ret = kernel_read(fp, buf, LEN, &(fp->f_pos));
        int count = 1;

        while(count <= LEN){
            kernel_write(fp_out, buf + (LEN - count), 1, &(fp_out->f_pos));
            count++;
        }

        filp_close(fp, NULL);
        filp_close(fp_out, NULL);
    }

    return 0;
}

static void fileIO_exit(void){
    printk(KERN_INFO "New file is written.\n");
}

module_init(fileIO_init);
module_exit(fileIO_exit);