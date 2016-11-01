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
#include<time.h>
int main(int argc, char const *argv[])
{
	unsigned char *f1,*f2,*f3;
    	unsigned long size1,k,status;
    	struct stat s;
    	const char * file_name = argv[1];
	struct rusage rusage;
	unsigned long start;
        char c;

	system("sudo su -c \"echo 1 > /proc/sys/vm/drop_caches\"");

	getrusage(RUSAGE_SELF, &rusage);
        start = rusage.ru_majflt;


    	int fd1 = open (argv[1], O_RDONLY);

    	/* Get the size of the file. */
    	status = fstat (fd1, & s);
    	size1 = s.st_size;

    	//f1 = (char *) mmap (0, size1, PROT_READ, MAP_PRIVATE, fd1, 0);

	f1=malloc(size1);
	if(f1==NULL)
	{
		printf("MAP failed:%d\n",errno);
	}

	printf("size:%lu\n",size1);
	unsigned long len = (size1*3)/4;

	clock_t begin = clock();
	for(k=0;k<3;k++)
	{
    		for (unsigned long i = size1/2,j=len-1; j>i ; i=i+4096,j=j-4096) {

        		f1[i]='j';
			c+=f1[j];
    		}
	}

        for(k=0;k<4;k++)
        {
                for (unsigned long i = 0,j=(size1/2)-1; j>i; i=i+4096,j=j-4096) {

                        f1[i]='n';
			c+=f1[j];
                }
        }

        for(k=0;k<1;k++)
        {
                for (unsigned long i = size1/2,j=len-1; j > i; i=i+4096,j=j-4096) {

                        f1[i]='v';
			c+=f1[j];
                }
        }


        for(k=0;k<3;k++)
        {
                for (unsigned long i = 0, j=(size1/2)-1; j>i ; i=i+4096,j=j-4096) {

                        f1[i]='z';
			c+=f1[j];
                }
        }

	printf("v:%c\n",c);
	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

       getrusage(RUSAGE_SELF, &rusage);
        printf("%ld \t%f",rusage.ru_majflt-start,time_spent);


	munmap(f1,size1);
	close(fd1);

    	return 0;
}
