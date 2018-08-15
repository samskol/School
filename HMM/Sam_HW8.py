# Sam Skolnekovich
# 10/30/15
# Assigment 8
#HMM part 2

'''
'To run the code you need the following command: python3 assignment8.py typos20.data typos20Test.data
'or to run the output into a txt file run the following: python3 assignment8.py typos20.data typos20Test.data > testfile2.txt
'testfile is included in the zip, can use this for testing or add your own file.
'The output starts with header then state sequence then Error rate: x=(value of error)
'The error rate is too high, I think this can be attributed to initial probabilities being set up wrong,
'or using log properties improperly. 
'''
import sys
from collections import defaultdict
import math

text=open(sys.argv[1])

inputlist=[]
outputlist=[]
dictMatrix={}
dict1Matrix={} #num of a,b,c, and so on in input
dict2Matrix={} #tstateMatrix
dict3={}	   #a/total, b/total and so forth 
pEmissions={}
pTranState={}

#PART TWO
Voutputlist=[]
Vinputlist=[]
typos20text=open(sys.argv[2])
viterbiValues=[{}]
checklist=[]
finallist=[]
tempVit={}
thispath=[]
counter=0
countered=-1
laterlist=[]
alphabet=['a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','_']
alphabet2=['a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z']

for line in text:
	rows=line.strip("\n")
	rows = line.split( )
	inputlist.append(rows[0])
	outputlist.append(rows[1])
	#print(inputlist,outputlist,"\n")
firstline=True
for line in typos20text:
	rows=line.strip("\n")
	rows = line.split( )
	if(firstline==True):
		firstline=False
	else:
		Vinputlist.append(rows[0])
		checklist.append(rows[1])
		if(rows[1]!='_'):
			Voutputlist.append(rows[1])
		if(rows[1]=='_'):
			laterlist.append(countered)
			#countered-=1
	countered+=1
	
		
	
	

for x in range(len(alphabet)):
	for y in range(len(alphabet)):
		dictMatrix[alphabet[x],alphabet[y]]=1 # Laplace smoothing
for x in range(len(alphabet)):
	for y in range(len(alphabet)):
		dict2Matrix[alphabet[x],alphabet[y]]=1 
#print(dictMatrix)
def emissionMatrix():
	for x in range(len(inputlist)):
		dictMatrix[inputlist[x],outputlist[x]]+=1
		if not inputlist[x] in dict1Matrix:
			dict1Matrix[inputlist[x]]=1
		else:
			dict1Matrix[inputlist[x]]+=1
		
#print(dict1Matrix)
#dict1 contains number of A's B's and so on in list
#dict contains number of e[a]|x[a], e[b]|x[a]... e[z]|x[b]

def nextTransMatrix():
	for x in range(len(inputlist)-1):
		dict2Matrix[inputlist[x],inputlist[x+1]]+=1
		
		

def calcPEmissions():
	counter=0
	x=0
	for inputchar in range(len(alphabet)-1):
		for outputchar in range(len(alphabet)-1):
			#if not inputchar
			x=dictMatrix[alphabet[inputchar],alphabet[outputchar]]/(dict1Matrix[alphabet[inputchar]]+26) #+26 Laplace
			#print(alphabet[inputchar],alphabet[outputchar],dictMatrix[alphabet[inputchar],alphabet[outputchar]]/(dict1Matrix[alphabet[inputchar]]+26)) #+26 Laplace
			pEmissions[alphabet[inputchar],alphabet[outputchar]]=x
			

		
def calcTransPStates():
	counter=0
	x=0
	for inputchar in range(len(alphabet)):
		for outputchar in range(len(alphabet)):
			x=dict2Matrix[alphabet[inputchar],alphabet[outputchar]]/(dict1Matrix[alphabet[inputchar]]+26) #+26 Laplace
			#print(alphabet[inputchar],alphabet[outputchar],dictMatrix[alphabet[inputchar],alphabet[outputchar]]/(dict1Matrix[alphabet[inputchar]]+26)) #+26 Laplace
			pTranState[alphabet[inputchar],alphabet[outputchar]]=x
			
def calcInitialProbs():
	dict3["**initial"]=.2
	a=len(inputlist)
	c=0
	for x in range(len(alphabet)):
		dict3[alphabet[x]]=dict1Matrix[alphabet[x]]/a
	'''for b in range(len(alphabet)):
		c=c+dict1Matrix[alphabet[b]]/a
	print (c)'''
#pEmissions holds all vals for alphabet without spaces

def printEmissions():
	print("Emission Probabilites")
	for element in sorted(pEmissions):
		print("P("+str(element[1])+"|"+str(element[0])+") =",pEmissions[element])
	print("Transition Probabilities:")
def printTransitions():
	for element in sorted(pTranState):
		print("P("+str(element[1])+"|"+str(element[0])+") =",pTranState[element])
	print("Initial Probabilities:")
def printInitials():
	for element in sorted(dict3):
		print("P("+str(element)+") =",dict3[element])
		
		
		
	#PART TWO	
	
def getProb(kind,firstletter,secondletter='None'):
	kind=str(kind)
	firstletter=str(firstletter)
	secondletter=str(secondletter)
	findEmission=False
	findTransition=False
	findInitial=False
	
	if(kind=='e'):
		findEmission=True
	if(kind=='t'):
		findTransition=True
	if(kind=='i'):
		findInitial=True
	
	if findEmission==True:
		#print(secondletter,firstletter)
		if(firstletter=='_'):
			return 1
		if(secondletter=='_'):
			return .00000000000001
		else:
			return pEmissions[secondletter,firstletter]
	if findTransition==True:
		if(secondletter=='_' and firstletter=='_'):
			return 0.000000000000001
		else:
			return pTranState[secondletter, firstletter]
	if findInitial==True:
		return dict3[firstletter]

def viterbi():
	path={}
	thispath=[]
	tempPath={}
	x=''
	y=''
	#maxval=0
	i=0
	total=0
	j=0
	k=0
	counter=0
	#initialize viterbi vals
	for letters in alphabet:
		viterbiValues[0][letters]=(math.log1p(dict3[letters]))
		path[letters]=letters
	
	for element in range(1,len(Voutputlist)-1): #element =vit depth
		maxval=0
		largeval=-1000000000
		viterbiValues.append({})
		tempPath={}
		#if(Voutputlist[element]!='_'):
		for letter in alphabet2:	
			#for secondletter in alphabet2:
			(temp,state)=max((math.log10(getProb('e',letter,Voutputlist[element]))+math.log10(getProb('t',secondletter,letter))+viterbiValues[element-1][secondletter],secondletter) for secondletter in alphabet2)
			'''if(temp>largeval):
					largeval=temp
					probPath=secondletter'''
					
			viterbiValues[element][letter]=temp
			tempPath[letter]=path[state]+letter
		
		path=tempPath
		
	n=len(Voutputlist)-2
	(prob,state)=max((viterbiValues[n][y],y)for y in alphabet2)
	
	thispath=list(path[state])
	for element in range(len(laterlist)):
		thispath.insert(laterlist[element],'_')
	#for testing error rate
	print("State Sequence:")
	for element in range(len(thispath)):
		if(thispath[element]==Vinputlist[element]):
			i=i+1
		else:
			j=j+1
		k=i/(i+j)
	for element in range(len(thispath)):
		print(thispath[element])
	return "Error rate:", 1-k


def main():
	emissionMatrix()
	calcPEmissions()
	#printEmissions()
	nextTransMatrix()
	calcTransPStates()
	calcInitialProbs()
	print(viterbi())
if __name__ == "__main__":
	main()





'''
'  Resources (to help after failed attempt)
'  https://en.wikipedia.org/wiki/Viterbi_algorithm
'  "Viterbi." Wikipedia. Wikimedia Foundation, n.d. Web. 30 Nov. 2015.
'
'
'''
