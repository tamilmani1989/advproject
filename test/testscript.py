#!/usr/bin/python
import os
os.system("sudo rm lru")
os.system("sudo rm lruk")
for x in range(0,3):
	for y in range(0,100):
		os.system(" sudo sysctl kernel.sample=1")
		os.system("./perf >> lru")
		os.system("sudo sysctl kernel.sample=2")	
		for z in range(50,230,20):
			cmd = "sudo sysctl kernel.lruk=" + str(z)
			cmd2 = "./perf >> lruk"+ str(z) 
			os.system(cmd)
			os.system(cmd2)

		os.system("sudo sysctl kernel.sample=1")			
		os.system("./perf >> lru")
		os.system("./perf >> lru")

