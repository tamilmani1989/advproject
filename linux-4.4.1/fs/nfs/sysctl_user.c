#include<linux/sysctl.h>
//#include<linux/sysctl_pagerep.h>
static int global_var=1;
static int min_val = 0;
static int max_val = 5;
static int min_lruk = 2;
static int max_lruk = 86400;
static int threshold = 180; 

void setPageRepAlgo(int);
void setLruThreshold(int);


int set_lruk_threshold(struct ctl_table *table, int write, 
                  void __user *buffer, size_t *lenp, loff_t *ppos)
{                                                                      
        int ret;                                                       
        ret = proc_dointvec_minmax(table,write,buffer,lenp,ppos);      
        if(ret == 0) {                                                 
        
		setLruThreshold(*((int *)table->data));                                                               
        }
	
	printk("Threshold level:%d\n", *((int *)table->data));                                                            
        return ret;                                                    
}


int switch_pagerep_algo(struct ctl_table *table, int write,
                  void __user *buffer, size_t *lenp, loff_t *ppos)
{
        int ret;
        ret = proc_dointvec_minmax(table,write,buffer,lenp,ppos);
        if(ret == 0) {

                setPageRepAlgo(*((int *)table->data));
        }

        printk("+++++Algo number:%d+++++++\n", *((int *)table->data));
        return ret;
}


static struct ctl_table sample_child_table[] = {
        {
//                .ctl_name = KERN_SAMPLE,
                .procname = "sample",
                .maxlen = sizeof(int),
                .mode = 0644,
                .data = &global_var,
                //.proc_handler = &proc_dointvec_minmax,
		.proc_handler = &switch_pagerep_algo,
                .extra1 = &min_val,
                .extra2 = &max_val,
        },
        {}
};

static struct ctl_table lruk_child_table[] = {
        {
//                .ctl_name = KERN_SAMPLE,
                .procname = "lruk",
                .maxlen = sizeof(int),
                .mode = 0644,
                .data = &threshold,
                //.proc_handler = &proc_dointvec_minmax,
                .proc_handler = &set_lruk_threshold,
                .extra1 = &min_lruk,
                .extra2 = &max_lruk,
        },
        {}
};
static struct ctl_table sample_parent_table[] = {
        {
//               .ctl_name = CTL_KERN,
                .procname = "kernel",
                .mode = 0555,
                .child = sample_child_table,
        },
        {}
};

static struct ctl_table lruk_parent_table[] = {
        {
//               .ctl_name = CTL_KERN,
                .procname = "kernel",
                .mode = 0555,
                .child = lruk_child_table,
        },
        {}
};
void register_user_sysctl(void) {
        /* register the above sysctl */
        if (!register_sysctl_table(sample_parent_table)) {
                printk(KERN_ALERT "Error: Failed to register sample_parent_table\n");
                return;
        }
	if (!register_sysctl_table(lruk_parent_table)) {
                printk(KERN_ALERT "Error: Failed to register sample_parent_table\n");
                return;
        }

	printk("****USER SYSCTL REGISTERED*******\n");
}

EXPORT_SYMBOL(register_user_sysctl);

