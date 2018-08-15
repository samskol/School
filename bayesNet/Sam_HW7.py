# Sam SKolnekovich
# 11/5/15
# Assignment 7

import getopt, sys

class node():
	def __init__(self,name):
		self.initValue=0
		self.conditions={}
		self.name=name
		self.children=[]
		self.parents=[]
		self.pos=False

cnode=node("c")
snode=node("s")
rnode=node("r")
wnode=node("w")

cnode.initValue=.5
	
rnode.parents.append(cnode)
snode.parents.append(cnode)

wnode.parents.append(snode)
wnode.parents.append(rnode)
	
cnode.children.append(snode)
cnode.children.append(rnode)
rnode.children.append(wnode)
snode.children.append(wnode)
	
	

cnode.setval=.5
#chances of wetgrass given conditions

snode.conditions["c"]=.1
snode.conditions["~c"]=.5 

rnode.conditions["c"]=.8
rnode.conditions["~c"]=.2

wnode.conditions["sr"]=.99
wnode.conditions["s~r"]=.9
wnode.conditions["~sr"]=.9
wnode.conditions["~s~r"]=0
nodeslist=[cnode,snode,rnode,wnode]
listAll=[]
samplelist=[0.82,0.56,0.08,0.81,0.34,0.22,0.37,0.99,0.55,0.61,0.31,0.66,0.28,1.0,0.95,
0.71,0.14,0.1,1.0,0.71,0.1,0.6,0.64,0.73,0.39,0.03,0.99,1.0,0.97,0.54,0.8,0.97,
0.07,0.69,0.43,0.29,0.61,0.03,0.13,0.14,0.13,0.4,0.94,0.19, 0.6,0.68,0.36,0.67,
0.12,0.38,0.42,0.81,0.0,0.2,0.85,0.01,0.55,0.3,0.3,0.11,0.83,0.96,0.41,0.65,
0.29,0.4,0.54,0.23,0.74,0.65,0.38,0.41,0.82,0.08,0.39,0.97,0.95,0.01,0.62,0.32,
0.56,0.68,0.32,0.27,0.77,0.74,0.79,0.11,0.29,0.69,0.99,0.79,0.21,0.2,0.43,0.81,
0.9,0.0,0.91,0.01]
iterateList=[0,4,8,12,16,20,24,28,32,36,40,44,48,52,56,60,64,68,72,76,80,84,88,92,96]

def priorSampling():
	'''
	'Forward sampling take file and implement find the next vals store their bools and then recover them.
	'Do it for entire sample file
	'take sample file store all values
	'''
	print("Prior Sampling Results")
	for x in iterateList:
		counter=0
		cnode.pos=False
		snode.pos=False
		rnode.pos=False
		wnode.pos=False
		if(samplelist[x]<.5): #taking cloudy to be less than .5 not cloudy otherwise, and just less than (tie goes to upper boundary)
			cnode.pos=True
		counter=counter+1
		
		if(cnode.pos==True):
			if(samplelist[x+counter]<.1):
				snode.pos=True
		
		if(cnode.pos==False):
			if(samplelist[x+counter]<.5):
				snode.pos=True
		counter=counter+1
		
		if(cnode.pos==True):
			if(samplelist[x+counter]<.8):
				rnode.pos=True
		
		if(cnode.pos==False):
			if(samplelist[x+counter]<.2):
				rnode.pos=True
		counter=counter+1
		if(rnode.pos==True and snode.pos==True):
			if(samplelist[x+counter]<1):
				wnode.pos=True
		
		if(rnode.pos==True and snode.pos==False):
			if(samplelist[x+counter]<.90):
				wnode.pos=True
		if(rnode.pos==False and snode.pos==True):
			if(samplelist[x+counter]<.90):
				wnode.pos=True
		for node in nodeslist:
			if(node.pos==True):
				listAll.append("+")
			if(node.pos==False):
				listAll.append("-")
			#listAll.append(node.name)
			
	return listAll
	
	
def calcPriorProb(givenList):
	#Traverses the prior table for each calculation
	#there's definitely more efficient ways of doing this.
	#calc p(c)
	a=2
	b=1
	c=0
	x=0
	i=0
	j=0
	
	listc=[]
	while c<100:
		listc.append(givenList[c])
		c=c+4
	for x in listc:
		if(x=="+"):
			i=i+1
	print("p(c)",i/25)
	
	
	#P(c|r)
	listr=[]
	while a<100:
		listr.append(givenList[a])
		a=a+4
	i=0
	j=0
	for x in range(len(listc)):
		if(listr[x]=="+"):
			j=j+1
			if(listc[x]=="+"):
				i=i+1
	
	print("p(c|r):",i/j)
	
	#p(s|w)
	i=0
	j=0
	d=3
	list_s=[]
	listw=[]
	while b<100:
		list_s.append(givenList[b])
		b=b+4
	while d<100:
		listw.append(givenList[d])
		d=d+4
		
	for x in range(len(listw)):
		if(listw[x]=="+"):
			j=j+1
			#print("j",j)
			if(list_s[x]=="+"):
				i=i+1
			#	print("i",i)
	
	print("p(s|w):",i/j)
	
	#p(s|c,w)
	i=0
	j=0
	
	for x in range(len(listw)):
		if(listc[x]=="+"and listw[x]=="+"):
			j=j+1
			#print("j",j)
			if(list_s[x]=="+"):
				i=i+1
			#	print("i",i)
	print("p(s|c,w):",i/j)
	
	#for x in givenList:
	return 1
	
		
def rejectionSampling():
	print("Rejection Sampling")
	counter1=0
	cnode.pos=False
	snode.pos=False
	rnode.pos=False
	wnode.pos=False
	#case p(c)
	x=0
	i=0
	while x <100:
		if(samplelist[x]<.5):
			i=i+1
		x=x+1
	print ("p(c):",i/100)
	#Rejection sampling uses 100 samples, prior used 25 since list was formed with forward sampling.
	#Prior had to go through s,r,w and back to c giving it only 1/4 of the samples
	
	#case p(c|r)
	x=0
	i=0
	j=0
	while x<99:
		cloudy=False
		rainy=False
		if(samplelist[x]<.5):
			cloudy=True
		x=x+1
		
		if(cloudy==True):
			if(samplelist[x]<.8):
				rainy=True
			else:
				rainy=False
		if(cloudy==False):
			if(samplelist[x]<.2):
				rainy=True
		if (rainy==True and cloudy==False):
			j=j+1
		if(rainy==True and cloudy==True):
			i=i+1
		x=x+1
	print("p(c|r):",i/(i+j))
	
	#case p(s|w)
	# since you need the p(w) given the samples, you have to use 4 samples each iteration.
	i=0
	x=0
	j=0
	while(x<97):
		cloudy1=False
		rainy1=False
		wet1=False
		sprinkler1=False
		if(samplelist[x]<.5): #taking cloudy to be less than .5 not cloudy otherwise, and just less than (tie goes to upper boundary)
			cloudy1=True
		x=x+1
		
		if(cloudy1==True):
			if(samplelist[x]<.1):
				sprinkler1=True
				
		if(cloudy1==False):
			if(samplelist[x]<.5):
				sprinkler1=True
		x=x+1
		
		if(cloudy1==True):
			if(samplelist[x]<.8):
				rainy1=True
		
		if(cloudy1==False):
			if(samplelist[x]<.2):
				rainy1=True
		x=x+1
		if(rainy1==True and sprinkler1==True):
			if(samplelist[x]<1):
				wet1=True
		
		if(rainy1==True and sprinkler1==False):
			if(samplelist[x]<.90):
				wet1=True
		if(rainy1==False and sprinkler1==True):
			if(samplelist[x]<.90):
				wet1=True
		if(wet1==True and sprinkler1==True):
			i=i+1
		if(wet1==True):
			j=j+1
		#print(i,j)
		x=x+1
	print("p(s|w):",i/j)
	
	#case p(s|c,w)
	i=0
	x=0
	j=0
	while(x<97):
		cloudy1=False
		rainy1=False
		wet1=False
		sprinkler1=False
		if(samplelist[x]<.5): #taking cloudy to be less than .5 not cloudy otherwise, and just less than (tie goes to upper boundary)
			cloudy1=True
		x=x+1
		
		if(cloudy1==True):
			if(samplelist[x]<.1):
				sprinkler1=True
				
		if(cloudy1==False):
			if(samplelist[x]<.5):
				sprinkler1=True
		x=x+1
		
		if(cloudy1==True):
			if(samplelist[x]<.8):
				rainy1=True
		
		if(cloudy1==False):
			if(samplelist[x]<.2):
				rainy1=True
		x=x+1
		if(rainy1==True and sprinkler1==True):
			if(samplelist[x]<1):
				wet1=True
		
		if(rainy1==True and sprinkler1==False):
			if(samplelist[x]<.90):
				wet1=True
		if(rainy1==False and sprinkler1==True):
			if(samplelist[x]<.90):
				wet1=True
		if(cloudy1==True and wet1==True and sprinkler1==True):
			i=i+1
		if(wet1==True and cloudy1==True):
			j=j+1
		#print(i,j)
		#print(x)
		x=x+1
	print("p(s|c,w):",i/j)
	
	
	
	
	return 0

def main():
	print("RESULTS",'\n')
	thisList=priorSampling()
	calcPriorProb(thisList)
	print(" ")
	rejectionSampling()
	print(thisList)
	
if __name__ == "__main__":
    main()



'''
RESULTS 

Prior Sampling Results
p(c) 0.48
p(c|r): 0.75
p(s|w): 0.4
p(s|c,w): 0.0
 
Rejection Sampling
p(c): 0.49
p(c|r): 0.7037037037037037
p(s|w): 0.4
p(s|c,w): 0.0
['-', '-', '+', '+', '+', '-', '+', '-', '-', '-', '-', '-', '+', '-', '-', '-', '+', '-', '-', '-', '+', '-', '+', '+', '+', '+', '-', '-', '-', '-', '-', '-', '+', '-', '+', '+', '-', '+', '+', '+', '+', '-', '-', '-', '-', '-', '-', '-', '+', '-', '+', '+', '+', '-', '-', '-', '-', '+', '-', '+', '-', '-', '-', '-', '+', '-', '+', '+', '-', '-', '-', '-', '-', '+', '-', '-', '-', '+', '-', '+', '-', '-', '-', '-', '-', '-', '-', '-', '+', '-', '-', '-', '+', '-', '+', '+', '-', '+', '-', '+']


------------------
(program exited with code: 0)
Press return to continue

'''



