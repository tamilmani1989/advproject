for k in `seq 1 20`
do
	sudo sysctl kernel.sample=1
	sudo su -c "echo 3 >> /proc/sys/vm/drop_caches"
	start=$(date +"%s")
	echo $start
	./freq2 3g.txt
	sleep 15
	./freq2 3g.txt
	sleep 15
	for i in `seq 1 4`;
	do
		./freq 3g.txt
		sleep 1
	done
	sleep 1
	./freq2 3g.txt

	./temp 3g.txt >> results-lru.txt
	./freq 3g.txt >> results-lru.txt
	end=$(date +"%s")
	echo "LRU $end  $start"
	time=`expr $end - $start`
	echo $time >> results-lru.txt
	echo "------" >> results-lru.txt
	
	sudo sysctl kernel.sample=2
	sudo sysctl kernel.lruk=10
	sudo su -c "echo 3 >> /proc/sys/vm/drop_caches"
	start=$(date +"%s")
        ./freq2 3g.txt
        sleep 15
        ./freq2 3g.txt
        sleep 15
        for i in `seq 1 4`;
        do
                ./freq 3g.txt
                sleep 1
        done
        sleep 1
        ./freq2 3g.txt

        ./temp 3g.txt >> results-lruk.txt
        ./freq 3g.txt >> results-lruk.txt
	end=$(date +"%s")
        time=`expr $end - $start`
        echo $time >> results-lruk.txt
	echo "LRU_K $end $start"
	echo "-------" >> results-lruk.txt

done


