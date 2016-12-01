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
#include <sys/mman.h>

int main(int argc, char const *argv[])
{
	unsigned char *f1,*f2,*f3;
    	unsigned long size1,k,status;
    	struct stat s;
    	const char * file_name = argv[1];
	struct rusage rusage;
	unsigned long start;
       	char c=0;
	int i=0;
//	system("sudo su -c \"echo 3 > /proc/sys/vm/drop_caches\"");

	getrusage(RUSAGE_SELF, &rusage);
        start = rusage.ru_majflt;


    	int fd1 = open (argv[1], O_RDONLY);

    	/* Get the size of the file. */
    	status = fstat (fd1, & s);
    	size1 = s.st_size;
//        int ret = posix_fadvise(fd1, 0, 0, POSIX_FADV_RANDOM );
//        printf("Ret :%d\n",ret);

    	f1 = (char *) mmap (0, size1, PROT_READ, MAP_SHARED, fd1, 0);
	int ret = madvise(f1, size1, MADV_RANDOM);


	if(f1==MAP_FAILED)
	{
		printf("MAP failed:%d\n",errno);
	}

	//unsigned long len = (size1*3)/4;

/*	for(unsigned long i=0;i<size1/4;i++)
	{
		c+=f1[i];
	}
*/
	for(k=0;k<1;k++)
	{
    		for (unsigned long i = size1/3; i < size1/2; i=i+4096) {

        		c+=f1[i];
			//if(c >=0 && c<128)
    		}
//		sleep(10);
	}

/*        for(k=0;k<1;k++)
        {
                for (unsigned long i = 0; i <size1/2; i=i+4096) {

                        c+=f1[i];
			//if(c >=0 && c <128)
                }
        }
*/
#if 0

        for(k=0;k<2;k++)
        {
                for (unsigned long i = 0; i < 2147483648 ; i=i+4096) {
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
