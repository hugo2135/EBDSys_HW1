#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>  //filp_open inside
#include <linux/buffer_head.h>
#include <linux/string.h>
#include <asm/segment.h>
#include <asm/uaccess.h>

MODULE_DESCRIPTION("file_IO_try");
MODULE_LICENSE("GPL");

mm_segment_t oldfs;

struct file *openfile(char *path,int flag,int mode) 
{ 
    struct file *fp; 
    
    fp=filp_open(path, flag, mode); 
    if (fp){
        printk("File is opened");
        return fp;
    }
    else{
        printk("File not open");
        return NULL;
    } ; 
}

int file_pos_write(struct file *fp, loff_t *pos, ssize_t rt){
    struct kiocb kiocb;
    init_sync_kiocb(&kiocb, fp);
    kiocb.ki_pos = pos ? *pos : 0;
    printk("pos : %ld",&pos);
    if(rt>0)
        if (pos){
            *pos = kiocb.ki_pos;
            return 0;
        }
    return -1;
}
 
int closefile(struct file *fp) 
{ 
    filp_close(fp,NULL); 
    return 0; 
} 

static int fileIO_init(void){
    int readlen = 512;
    char __user buf[readlen];
    struct  file *fp;
    struct  file *fp_out;
    ssize_t rt;
    int ret;
    printk("--------------");
    printk(KERN_INFO "File accessing\n");
    fp = openfile("input.txt", O_RDONLY, 0);
    fp_out = openfile("output.txt", O_WRONLY, 0);
    if(fp != NULL){
        memset(buf, 0, readlen);
        ret = kernel_read(fp, buf, readlen, fp->f_pos);
        char a;
        // char tmp[ret];
        int count = 0;
        // memset(tmp, 0, ret);
        
        printk("f_pos:%lld", fp->f_pos);
        printk("buf:%s", buf);
        printk("buf addr 1:%p", buf[0]);
        printk("buf addr 2:%p", buf[1]);
        
        printk("ret:%d", ret);
        while(count<=ret){
            a = buf[ret-count];
            rt = kernel_write(fp_out, &a, 1, fp_out->f_pos);
            file_pos_write(fp_out, fp_out->f_pos, rt);
            count++;
        }
        // printk("tmp:%s", tmp);
        closefile(fp);
        closefile(fp_out);
    }

    return 0;
}

static void fileIO_exit(void){
    printk(KERN_INFO "New file is written.\n");
}

module_init(fileIO_init);
module_exit(fileIO_exit);