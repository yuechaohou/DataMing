import os,sys,math,copy,random
import numpy as np
import matplotlib.pyplot as plt

def distance(x,y):
	return math.hypot(x[0]-y[0],x[1]-y[1])


def K(m,n):
	operand = math.hypot(m,n)
	expoent=0.0 - (operand * operand/2.0)
	result = 1.0/2.0/math.pi * math.exp(expoent)
	return result


def FindAttractor(x,D,h,theta):
	t=0
	xcluster = list()
	xcluster.append(x)
	while t <= 0 or (t>=1 and distance(xcluster[t],xcluster[t-1])> theta):
		member_1 = 0.0
		member_2 = 0.0		
		denominator = 0.0
		for j in range(len(D)):
			first = float(xcluster[t][0] - D[j][0])/float(h)
			second = float(xcluster[t][1] - D[j][1])/float(h)
			m=K(first ,second)
			member_1 = member_1 + m*D[j][0]
			member_2 = member_2 + m*D[j][1]
			denominator = float(denominator) + m
		temp = list()
		first = float(member_1 / denominator)
		second = float(member_2 / denominator)
		temp.append(first)
		temp.append(second)
		xcluster.append(temp)		
		t=t+1
	return xcluster[t]


def f(x, D, h):
	value = 0.0
	n = float(len(D))
	for i in range(len(D)):
		first = float(x[0] - D[i][0])/float(h)
		second = float(x[1] - D[i][1])/float(h)
		value = value + K(first, second)
	value = value/n/float(h)/float(h)
	return value
	

def Maximal_density(A,D,h):
	result= list()
	while len(A)>0:
		temp = list()
		temp.append(A[0])
		A.remove(A[0])
		i=0
		while i< len(temp):
			j=0
			while j < len(A):
				if distance(temp[i],A[j]) <=0.0001:
					temp.append(A[j])
					A.remove(A[j])
				j=j+1
			i=i+1
		result.append(temp)
	return result


read_file = open(sys.argv[1], 'r')
content = read_file.readlines()

h= 10
epsilon = 0.000004
theta = 0.0001
A=list()
D= list()
R= dict()

for line in content:
	line=line.strip()
	index_1= line.find(',')
	if index_1>0:
		index_2= line.find(',', index_1+1)
		if index_2 >0:
			attr_1= float(line[0: index_1])
			attr_2= float(line[index_1+1 : index_2])
			temp=list()
			temp.append(attr_1)
			temp.append(attr_2)
			D.append(temp)
read_file.close()


for x in D:
	nextx = FindAttractor(x,D,h,theta)
	if f(nextx, D, h) >= epsilon:
		if nextx not in A:
			A.append(nextx)
			R[(nextx[0],nextx[1])]=list()
		R[(nextx[0],nextx[1])].append(x)

D_new=list()
for node in D:
	if f(node, D, h)>= epsilon:
		D_new.append(node)

C= Maximal_density(A,D_new,h)
C_new= copy.deepcopy(C)


for i in range(len(C)):
	for x in C[i]:
		for nodes in R[(x[0],x[1])]:
			if nodes not in C_new[i]:
				C_new[i].append(nodes)
		

p=list()
for element in C_new:
	first=list()
	second=list()
	for node in element:
		first.append(node[0])
		second.append(node[1])
	p.append(first)
	p.append(second)


for i in range(len(p)/2):
	plt.scatter(p[2*i], p[2*i+1], c=(random.uniform(0,1),random.uniform(0,1),random.uniform(0,1)), s = 30) 

plt.show() 
