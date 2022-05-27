/* ����������� ������������ ������. ? */
#include <linux/module.h> /* �������� ������� � ����������� ��� ������������ �������� ������� ���� */
#include <linux/init.h>  /* ��������� �� ������� ������������� � ������� */
#include <linux/fs.h>    /* �������� ������� ����������� � �������� �������� */
#include <linux/cdev.h>  /* �������� ����������� ������� ��� ����������� �������� */
#include <linux/slab.h>  /* �������� ������� ���� ��� ���������� ������� */
#include <asm/uaccess.h> /* ������������� ������ � ������������ ������������ */

/* ������� ���������� �� ����������. ? */

int scull_major = 0;	/*������� �����*/
int scull_minor = 0;	/*������� �����*/
int scull_nr_devs = 1;	/**/
int scull_quantum = 4000;	/*���-�� ������������ �������*/
int scull_qset = 1000;	
/*_________________________________________________________________________________________________________________________________________*/

struct scull_qset {   /* ���������� ����� ������������ ������� ������ ����������, ������ �� ������� ��������� �� ��������� scull_qset.*/
	void **data;			
	struct scull_qset *next; 	
};

struct scull_dev {    /* ������� ������������ ������ ���������� ���������� ���������� scull_dev, � ����� ������������� ��������� cdev � ����.*/
	struct scull_qset *data;  
	int quantum;		 
	int qset;		  
	unsigned long size;	  
	unsigned int access_key;  
	struct semaphore sem;    
	struct cdev cdev;	 
};

struct scull_dev *scull_device;

int scull_trim(struct scull_dev *dev)	/*�������� �� ������ � ���������� ������ ����*/
{
	struct scull_qset *next, *dptr;
	int qset = dev->qset; 
	int i;

	for (dptr = dev->data; dptr; dptr = next) { 
		if (dptr->data) {
			for (i = 0; i < qset; i++)
				kfree(dptr->data[i]);

			kfree(dptr->data);
			dptr->data = NULL;
		}

		next = dptr->next;
		kfree(dptr);
	}

	dev->size = 0;
	dev->quantum = scull_quantum;
	dev->qset = scull_qset;
	dev->data = NULL;

	return 0;
}

struct file_operations scull_fops = {/*������������� ����� ����� ��������: MAJOR, MINOR � ���������� ��������. ��� ����� ������� ��������� ��������� file_operations.*/
	.owner = THIS_MODULE,			
	//.read = scull_read,
	.write = scull_write,
	.open = scull_open,
	.release = scull_release,
};

static void scull_setup_cdev(struct scull_dev *dev, int index)	/*�������� ��������� � ����*/
{
	int err, devno = MKDEV(scull_major, scull_minor + index);	

	cdev_init(&dev->cdev, &scull_fops);

	dev->cdev.owner = THIS_MODULE;
	dev->cdev.ops = &scull_fops;

	err = cdev_add(&dev->cdev, devno, 1);

	if (err)
		printk(KERN_NOTICE "Error %d adding scull  %d", err, index);
}

void scull_cleanup_module(void) /*������� scull_cleanup_module ���������� ��� �������� ������ ���������� �� ����. ?*/
{
	int i;
	dev_t devno = MKDEV(scull_major, scull_minor);

	if (scull_device) {
		for (i = 0; i < scull_nr_devs; i++) {
			scull_trim(scull_device + i);		
			cdev_del(&scull_device[i].cdev);	
		}
		
		kfree(scull_device);
	}
    printk(KERN_INFO �Work is Over�);
	unregister_chrdev_region(devno, scull_nr_devs); 
}

/*������� ������������ ���������� ?*/
static int scull_init_module(void)
{
	int rv, i;
	dev_t dev;

		
	rv = alloc_chrdev_region(&dev, scull_minor, scull_nr_devs, "scull");	/*������� alloc_chrdev_region �� ������������ �������� ���������� ������� ��������� � ��������� ��� ����������*/

	if (rv) {
		printk(KERN_WARNING "error, can't get major %d\n", scull_major); /*���� ����������� �������� �� �������� ����� ������, ���������� ��������� ������������.*/
		return rv;
	}

        scull_major = MAJOR(dev);       /*������� MAJOR(dev) �� �������� ������� �����*/

	scull_device = kmalloc(scull_nr_devs * sizeof(struct scull_dev), GFP_KERNEL);	
	
	if (!scull_device) {
		rv = -ENOMEM;
		goto fail;
	}

	memset(scull_device, 0, scull_nr_devs * sizeof(struct scull_dev));		

	for (i = 0; i < scull_nr_devs; i++) {	/*������� scull_setup_cdev ��������� ������������� � ���������� ��������� � �������.*/					
		scull_device[i].quantum = scull_quantum;
		scull_device[i].qset = scull_qset;
		sema_init(&scull_device[i].sem, 1);
		scull_setup_cdev(&scull_device[i], i);					
	}

	dev = MKDEV(scull_major, scull_minor + scull_nr_devs);	/*MKDEV ������ ��� �������� ������� � ������� ������� ���������.*/
	
	printk(KERN_INFO "major = %d minor = %d\n", scull_major, scull_minor);

	return 0;

fail:
	scull_cleanup_module();
	return rv;
}
#define DEVICE_NAME �my_dummy_device�
MODULE_AUTHOR("Andrey Ilin");
MODULE_LICENSE("GPL");

module_init(scull_init_module);		
module_exit(scull_cleanup_module);