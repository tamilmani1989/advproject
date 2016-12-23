for k in `seq 1 30`
do
	sudo sysctl kernel.sample=1
	sudo su -c "echo 3 >> /proc/sys/vm/drop_caches"
	start=$(date +"%s")
	./freq $1
	sleep 15
	./freq $1
	sleep 15
	for i in `seq 1 4`;
	do
		./freq2 $1
		sleep 1
	done
	sleep 1
	./freq $1

	./temp $1 
	./freq $1

	end=$(date +"%s")
	time=`expr $end - $start`
	echo $time >> results-lru.txt
	
	sudo sysctl kernel.sample=2
	sudo sysctl kernel.lruk=10
	sudo su -c "echo 3 >> /proc/sys/vm/drop_caches"
	start=$(date +"%s")
        ./freq $1
        sleep 15
        ./freq $1
        sleep 15
        for i in `seq 1 4`;
        do
                ./freq2 $1
                sleep 1
        done
        sleep 1
        ./freq $1

        ./temp $1 
        ./freq $1
	end=$(date +"%s")
        time=`expr $end - $start`
        echo $time >> results-lruk.txt

done


