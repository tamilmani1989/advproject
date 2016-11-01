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

	system("sudo su -c \"echo 3 > /proc/sys/vm/drop_caches\"");

	getrusage(RUSAGE_SELF, &rusage);
        start = rusage.ru_majflt;


    	int fd1 = open (argv[1], O_RDONLY);

    	/* Get the size of the file. */
    	status = fstat (fd1, & s);
    	size1 = s.st_size;

    	f1 = (char *) mmap (0, size1, PROT_READ, MAP_SHARED, fd1, 0);

	if(f1==MAP_FAILED)
	{
		printf("MAP failed:%d\n",errno);
	}

	printf("size:%lu\n",size1);
	//unsigned long len = (size1*3)/4;
	for(k=0;k<1;k++)
	{
    		for (unsigned long i = 0; i < size1/3; i=i+4096) {

        		c+=f1[i];
			//if(c >=0 && c<128)
    		}
	}
#if 0

        for(k=0;k<1;k++)
        {
                for (unsigned long i = 0; i <size1/2; i=i+4096) {

                        c+=f1[i];
			//if(c >=0 && c <128)
                }
        }


        for(k=0;k<1;k++)
        {
                for (unsigned long i = 0; i < 2040109465; i=i+4097) {
                        c+=f1[i];
		}
			//if(c>=0 && c<128)
	}
#endif

#if 0

        for(k=0;k<1;k++)
        {
                for (unsigned long i = 0; i < size1/3; i=i+4097) {
                        char c;

                        c+=f1[i];
                }
        }

#endif

       getrusage(RUSAGE_SELF, &rusage);
        printf("%ld\n",rusage.ru_majflt-start);


	munmap(f1,size1);
	close(fd1);

    	return 0;
}
