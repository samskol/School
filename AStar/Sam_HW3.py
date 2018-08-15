#Sam SKolnekovich
#Assignment3


import sys
import math
from sys import argv

script, filename, Heuristic = argv

txt = open(filename)
rows=0
cols=0
i=0
j=-1
print(txt.read())

adj_nodes=[]	
nodepaths=[]
allnodes=[]

class node():
	def __init__(self,node,locationx,locationy,distance,heur1,cost,parentnode,value, adj,totalcost,visited, iscalculated):
		self.node=node
		self.locationx=locationx
		self.locationy=locationy
		self.distance=distance    #initialize objects, distance not used
		self.heur1=heur1			#heuristic
		self.cost=cost				#cost from previous node+cost of current adjacency
		self.parentnode=parentnode
		self.value=value			#0,1=mountain,2=wall
		self.adj=adj				#list of adj vertices, tuple not a node list
		self.totalcost=totalcost	#cost+heur1
		self.visited=False
		self.iscalculated=False   #initialized, not used
		
def addHeur1(current_node,goal_node,heuristic):  #Heuristic, cmdl needs one of two words
	x=current_node.locationx
	y=current_node.locationy
	a=goal_node.locationx
	b=goal_node.locationy
	if(heuristic=='manhattan'):
		heur=10*(math.fabs(a-x)+math.fabs(y-b))
		return heur
	if(heuristic=="pythagorean"):
		return (10 * math.sqrt(((a-x)**2)+((y-b)**2)))
	else:
		return ("Did not enter correct Heuristic") 

	
def addNode(x,y,value):      #addnodes in loop from matrix, matrix from text file
	value=int (value)
	newnode=node(None,x,y,None,None,None,None,value,None,None,False,False)
	allnodes.append(newnode)
	
	print(allnodes[-1].locationx,allnodes[-1].locationy, allnodes[-1].value)

def findAdjacent():      #associate a list with the adjacent vertices
	for nodes in allnodes:
		
		nodes.adj=[(nodes.locationx-1,nodes.locationy-1),(nodes.locationx,nodes.locationy-1),(nodes.locationx+1,nodes.locationy-1),
		(nodes.locationx-1,nodes.locationy), (nodes.locationx+1,nodes.locationy),
		(nodes.locationx-1,nodes.locationy+1),(nodes.locationx,nodes.locationy+1),(nodes.locationx+1,nodes.locationy+1)]
		for i in range(0,2):
			for int1,int2 in nodes.adj:
				if(int1>9 or int1<0):
					nodes.adj.remove((int1,int2))
		for i in range(0,2):
			for int1,int2 in nodes.adj:
				if(int2>7 or int2<0):
					nodes.adj.remove((int1,int2))
		
		
	
def AStar(rows,cols, current_node,parent_node):
	goalFound=False	
	goalnode=allnodes[cols-1]
	goalnode.locationx=allnodes[cols-1].locationx
	goalnode.locationy=allnodes[cols-1].locationy
	if (len(nodepaths)==0):
		nodepaths.append(current_node)
		nodepaths[0].cost=0
		nodepaths[0].heur1=addHeur1(nodepaths[0],goalnode,Heuristic)
		nodepaths[0].parentnode=parent_node
		nodepaths[0].parentnode.cost=0
	temp_list=[]
	current_node.parentnode=parent_node
	startnode=allnodes[rows*cols-cols]
	j=0
	for item1,item2 in current_node.adj:
		for item in range(len(allnodes)): 				#assign list of adj vertices to actual nodes
			if(item1==allnodes[item].locationx and item2==allnodes[item].locationy and allnodes[item].value!=2 and allnodes[item].iscalculated==False and allnodes[item].visited==False):
				#print(allnodes[item].locationx,allnodes[item].locationy) 
				allnodes[item].heur1=addHeur1(allnodes[item],goalnode,Heuristic)
				if(allnodes[item].value==0):
					if((allnodes[item].locationx-current_node.locationx)**2+(allnodes[item].locationy-current_node.locationy)**2==2):
						allnodes[item].cost=14
					if((allnodes[item].locationx-current_node.locationx)**2+(allnodes[item].locationy-current_node.locationy)**2==1):
						allnodes[item].cost=10
				if(allnodes[item].value==1):
					if((allnodes[item].locationx-current_node.locationx)**2+(allnodes[item].locationy-current_node.locationy)**2==2):
						allnodes[item].cost=24
					if((allnodes[item].locationx-current_node.locationx)**2+(allnodes[item].locationy-current_node.locationy)**2==1):
						allnodes[item].cost=20
												#Above just takes adj nodes and assigns them a cost for a given value
				allnodes[item].parentnode=current_node
				a=int(allnodes[item].cost)
				
				y=int(allnodes[item].parentnode.cost)
				
				a=a+y
				
				i=int(allnodes[item].heur1+a)
				
				allnodes[item].cost=a
				allnodes[item].totalcost=i
				if(len(adj_nodes)==0):
					adj_nodes.append(allnodes[item])
					
				else:
					if(allnodes[item] not in adj_nodes):
						adj_nodes.append(allnodes[item])
						
				allnodes[item].iscalculated=True
				adj_nodes[-1].parentnode=current_node
				
				
		
	for item in range(len(adj_nodes)-1):
		if(adj_nodes[item+1].totalcost>adj_nodes[item].totalcost):
			temp_list.append(adj_nodes[item])
			adj_nodes[item]=adj_nodes[item+1]
			adj_nodes[item+1]=temp_list[-1]
			temp_list.pop()
			
	
	
	if(len(adj_nodes)>=1):
		nodepaths.append(adj_nodes[-1])
		adj_nodes[-1].visited=True
		nodepaths[-1].parentnode=adj_nodes[-1].parentnode
		adj_nodes.pop()
	nodepaths[-1].visited=True
		
	
	d=0
	isdone=False
	finalcount=0
	if(goalnode in nodepaths):
		goalFound=True
		for items in range(len(nodepaths)):
			finalcount=finalcount+1
		#if(nodepaths[-1].locationx==goalnode.locationx and nodepaths[-1].locationy==goalnode.locationy):
		print(nodepaths[-1].totalcost)
		while (isdone==False):
			if(nodepaths[-1].locationx==0 and nodepaths[-1].locationy==rows-1):
				isdone=True
			print(nodepaths[-1].locationx, nodepaths[-1].locationy)#nodepaths[-1].totalcost,nodepaths[-1].heur1)
			nodepaths[-1]=nodepaths[-1].parentnode
		print(finalcount)
		'''for item in range(len(nodepaths)):
			for item1,item2 in nodepaths[item].adj:
				if(len(nodepaths)>1):
					if(item1==nodepaths[item].locationx and item2==allnodes[item].locationy):
						print(nodepaths[item].locationx,nodepaths[item].locationy)
						nodepaths[-1]=nodepaths[item]
					else:
						nodepaths.pop()
				if (len(nodepaths)>=0):
					break'''
				
	
	
	if(len(adj_nodes)>0 and goalFound==False):							#Recurse with new current_node
		AStar(rows,cols,nodepaths[-1],nodepaths[-1].parentnode)			#keep track of its parent 
		
		
	

with open(filename) as txt:
	for line in txt:
		line=line.strip()
		items=line.split(' ')
		#print(line)
		#print(items)
		for value in items:
			cols=cols+1
		rows=rows+1
rows=rows-1
cols=cols-1
cols=round(cols/rows)
print(rows,cols)
matrix=[[0 for x in range(rows)] for y in range(cols)]

with open(filename) as txt:
	for line in txt:
		line=line.strip()
		items=line.split(' ')
		i=0
		j=j+1
		for value in items:
			if(j<=rows-1 and i<=cols-1):
				matrix[i][j]=value
				addNode(i,j,value)
			i=i+1
# so i=cols and j=rows and i comes before j, even though range is opposite,

findAdjacent()
startnode=allnodes[rows*cols-cols]
dummy=node(None,None,None,None,None,None,None,None,None,None,False,False)
AStar(rows,cols,startnode,dummy)
