#include <linux/perf_event.h>
#include <linux/hw_breakpoint.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include <asm/unistd.h>
#include <fcntl.h>
#include <signal.h>
#include<stdint.h>
#include <sys/time.h>
#include <sys/resource.h>
//struct perf_event_attr pe_attr_page_faults;

/*  pe_attr_page_faults.size = sizeof(pe_attr_page_faults);
  pe_attr_page_faults.type =   PERF_TYPE_SOFTWARE;
  pe_attr_page_faults.config = PERF_COUNT_SW_PAGE_FAULTS;
  pe_attr_page_faults.disabled = 1;
  pe_attr_page_faults.exclude_kernel = 1;
*/

#define N 1610612736
#define SMALLN 536870912
/*static long perf_event_open(struct perf_event_attr *hw_event,
                pid_t pid,
                int cpu,
                int group_fd,
                unsigned long flags) {
  int ret = syscall(__NR_perf_event_open, hw_event, pid, cpu,
            group_fd, flags);
  return ret;
}
*/
static void accessArray(char *arr, uint64_t size, char val)
{
	uint64_t i;
	for(i=0;i<size;i++)
		arr[i]=val;
}

int main()
{
	int page_faults_fd;
        uint64_t page_faults_count;
	int j=0;
	char *big,*small;
	struct rusage rusage;
	long start;

/*	pe_attr_page_faults.size = sizeof(pe_attr_page_faults);
  	pe_attr_page_faults.type =   PERF_TYPE_SOFTWARE;
  	pe_attr_page_faults.config = PERF_COUNT_SW_PAGE_FAULTS;
	pe_attr_page_faults.disabled = 1;
  	pe_attr_page_faults.exclude_kernel = 1;

	page_faults_fd = perf_event_open(&pe_attr_page_faults, 0, 0, -1, 0);
  	if (page_faults_fd == -1) {
   		printf("perf_event_open failed for page faults:%d \n",errno);
    		return -1;
  	}
*/
	getrusage(RUSAGE_SELF, &rusage);
	start = rusage.ru_majflt;

	big = malloc(sizeof(char) * N);
	if(big==NULL) {
		printf("Not enough memory\n");
		return;
	}
	sleep(60);
	small = malloc(sizeof(char) * SMALLN);
	if(small == NULL)
	{
		printf("Not enuf memory\n");
		return;
	}	
	accessArray(small, SMALLN, 6);	
	
	accessArray(big, N,5);
	
	while(j < 2) {
		accessArray(small, SMALLN, '7');
		sleep(20);
		accessArray(small, SMALLN ,'b');
		sleep(20);
		accessArray(small, SMALLN, 'v');
		j++;
	}

	accessArray(big, N ,'z');
	sleep(2);
	accessArray(small, SMALLN ,'f');

 	free(big); 
	free(small);

	getrusage(RUSAGE_SELF, &rusage);
	printf("%ld\n",rusage.ru_majflt-start);
	return 0;	
}

