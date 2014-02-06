
import os
speedup = [open("speedup1.txt",'w'), open("speedup2.txt",'w'), open("speedup3.txt",'w')]
eff = outfile = [open("eff1.txt",'w'), open("eff2.txt",'w'), open("eff3.txt",'w')]

info = []
i = 0
for infile in os.listdir(os.curdir):
    if(infile != "speedup.plt" and infile != "eff.plt" and infile != "old" and infile != "filter.py" and infile != "logs.txt"):
        openfile = open(infile)
        data = openfile.read().split('\n')
        #print data
        openfile.close()
		
        for line in data:
			#print line
			obj = line.split(' ')
			#print obj
			if len(obj) == 4:
				info.append((int(obj[0]),int(obj[1]),float(obj[3])))

			i = i + 1
			
info.sort(key=lambda tup: tup[0])
print info
ts = [1,1,1]
i = 0
x = 0
for row in info:
	#find ts
	if (x == 0):
		ts[i] = row[2]
	p = row[0]	
	tp = row[2]
	S = (ts[i] / tp)
	E = S / p
	
	speedup[i].write(str(p) + " " + str(S) + "\r\n")
	eff[i].write(str(p) + " " + str(E) + "\r\n")
	
	i= i +1
	
	if(i == 3):
		i = 0
		x = 1

for file in speedup:
	file.close()
	
for file in eff:	
	file.close()
    
