'''
Created on 2016.6.22

@author: Hou Yuechao
'''
import os,sys,time


def Spade(minsup):
    print 'what is your name',minsup

start = time.clock()

direction = os.getcwd()
ifilepath = os.path.join(direction,"sequencedb.txt")
ofilepath = os.path.join(direction,"output.txt")
iFile = open(ifilepath,"r")
oFile = open(ofilepath,"w")
lines = iFile.readlines()
data = list()
result = list()

try:
    minsup = int(sys.argv[1])
except:
    minsup = "wrong"

if minsup == "wrong":
    print 'Minsup is not an integer'
    exit()
    
for line in lines:
    if line.startswith(">"):
        continue
    else:
        data.append(line)
        
Spade(minsup)

finish = time.clock()
print (finish - start), 'ms'

