#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/io.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include<stdlib.h>
#include<errno.h>

int main(int argc, char const *argv[])
{
	unsigned char *f1,*f2,*f3;
    	unsigned long size1,k,status;
    	struct stat s;
    	const char * file_name = argv[1];
	struct rusage rusage;
	unsigned long start;
       	char c=0;


	getrusage(RUSAGE_CHILDREN, &rusage);
        start = rusage.ru_majflt;
	
	system("free -m");
	system("sudo su -c \"echo 3 > /proc/sys/vm/drop_caches\"");
	system("free -m");

    	int fd1 = open (argv[1], O_RDONLY);

    	/* Get the size of the file. */
    	status = fstat (fd1, & s);
    	size1 = s.st_size;

	int ret = posix_fadvise(fd1, 0, 0, POSIX_FADV_DONTNEED );
	printf("Ret :%d\n",ret);
    	f1 = (char *) mmap (0, size1, PROT_READ, MAP_SHARED, fd1, 0);

	if(f1==MAP_FAILED)
	{
		printf("MAP failed:%d\n",errno);
	}

	printf("size:%lu\n",size1);

	for(int k=0;k<1;k++)
	{
		for(unsigned long i=0;i<size1/4;i=i+4096)
		{
			c+=f1[i];
		}
	}
/*	for(unsigned long i=0,j=1073741824; j>i; i=i+4096,j=j-4096)
	{
		//printf("hello\n");
		c+=f1[i];
		c+=f1[j];
	}
*/
      system("sudo su -c \"echo 3 > /proc/sys/vm/drop_caches\"");

	printf("v:%c\n",c);
	//unsigned long len = (size1*3)/4;

	munmap(f1,size1);
	close(fd1);
        getrusage(RUSAGE_SELF, &rusage);
        printf("%ld\n",rusage.ru_majflt-start);
	system("free -m");

    	return 0;
}
