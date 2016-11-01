#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/io.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include<stdlib.h>
#include<linux/perf_event.h>
#include<linux/hw_breakpoint.h>
#include<stdint.h>
#include<errno.h>
//#include<perf_event.h>
#include<sys/ioctl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/perf_event.h>
#include <asm/unistd.h>

static long perf_event_open(struct perf_event_attr *hw_event,
                pid_t pid,
                int cpu,
                int group_fd,
                unsigned long flags) {
  int ret = syscall(__NR_perf_event_open, hw_event, pid, cpu,
            group_fd, flags);
  return ret;
}

int main(int argc, char const *argv[])
{
	unsigned char *f1,*f2,*f3;
    	unsigned long size1,k,status;
    	struct stat s;
    	const char * file_name = argv[1];
	struct rusage rusage;
	unsigned long start;
 	struct perf_event_attr pe_attr_page_faults;

        memset(&pe_attr_page_faults, 0, sizeof(pe_attr_page_faults));
  	pe_attr_page_faults.size = sizeof(pe_attr_page_faults);
  	pe_attr_page_faults.type =   PERF_TYPE_SOFTWARE;
  	pe_attr_page_faults.config = PERF_COUNT_SW_PAGE_FAULTS_MAJ;
  	pe_attr_page_faults.disabled = 1;
  	pe_attr_page_faults.exclude_kernel = 1;
  	int page_faults_fd = perf_event_open(&pe_attr_page_faults, 0, 0, -1, 0);
  	if (page_faults_fd == -1) {
    		printf("perf_event_open failed for page faults: %s\n", strerror(errno));
    		return -1;
  	}

	system("sudo su -c \"echo 1 > /proc/sys/vm/drop_caches\"");

	getrusage(RUSAGE_SELF, &rusage);
        start = rusage.ru_majflt;


  	// Start counting
  	ioctl(page_faults_fd, PERF_EVENT_IOC_RESET, 0);
  	ioctl(page_faults_fd, PERF_EVENT_IOC_ENABLE, 0);

    	int fd1 = open (argv[1], O_RDONLY);

    	/* Get the size of the file. */
    	status = fstat (fd1, & s);
    	size1 = s.st_size;

    	f1 = (char *) mmap (0, size1, PROT_READ|PROT_WRITE, MAP_PRIVATE,fd1, 0);
	if(f1==MAP_FAILED)
	{
		printf("Map failed\n");
	}

	unsigned long len = (size1*3)/4;

	printf("size:%lu\n",size1);

	for(k=0;k<3;k++)
	{
    		for (unsigned long i = size1/2; i < len; i=i+4096) {
        		char c;

        		c=f1[i];
    		}
	}

        for(k=0;k<4;k++)
        {
                for (unsigned long i = 0; i < size1/2; i=i+4096) {
                        char c;

                        c=f1[i];
                }
        }

        for(k=0;k<1;k++)
        {
                for (unsigned long i = 0; i < len; i=i+4096) {
                        char c;

                        c=f1[i];
                }
        }


        for(k=0;k<3;k++)
        {
                for (unsigned long i = 0; i < size1/2; i=i+4096) {
                        char c;

                        c=f1[i];
                }
        }


       getrusage(RUSAGE_SELF, &rusage);
        printf("%ld\n",rusage.ru_majflt-start);


       ioctl(page_faults_fd, PERF_EVENT_IOC_DISABLE, 0);
       uint64_t page_faults_count;
       read(page_faults_fd, &page_faults_count, sizeof(page_faults_count));

	printf("perf page fault:%d\n",page_faults_count);
	munmap(f1,size1);
	close(fd1);

    	return 0;
}
