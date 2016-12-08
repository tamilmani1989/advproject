#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/io.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include<stdlib.h>
#include <sys/mman.h>

int main(int argc, char const *argv[])
{
	unsigned char *f1,*f2,*f3;
    	unsigned long size1,k,status;
    	struct stat s;
    	const char * file_name = argv[1];
	struct rusage rusage;
	unsigned long start;
	char c;
	system("sudo su -c \"echo 3 > /proc/sys/vm/drop_caches\"");

	getrusage(RUSAGE_SELF, &rusage);
        start = rusage.ru_majflt;


    	int fd1 = open (argv[1], O_RDONLY);

    	/* Get the size of the file. */
    	status = fstat (fd1, & s);
    	size1 = s.st_size;


//	printf("ret:%d\n",ret);



    	f1 = (char *) mmap (0, size1, PROT_READ, MAP_SHARED, fd1, 0);
        int ret = madvise(f1, size1, MADV_RANDOM);
        printf("Ret:%d\n",ret);

	
//	int ret = madvise(f1, 0, MADV_RANDOM);
//	printf("Ret:%d\n",ret);


//	f1=malloc(size1);
	unsigned long len = (size1*3)/4;


	printf("size:%lu %d\n",size1,sizeof(unsigned long));

	for(k=0;k<1;k++)
	{
    		for (unsigned long i = size1/2; i < len; i=i+4096) {
//		for (unsigned long i = 0; i < 12189; i=i+4096) {

        		c+=f1[i];
    		}
	}
#if 1
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

#endif
       getrusage(RUSAGE_SELF, &rusage);
        printf("%ld\n",rusage.ru_majflt-start);


	//munmap(f1,size1);
//	free(f1);
	close(fd1);

    	return 0;
}
