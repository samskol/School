fall 2015
usage python3 world1.txt world2.txt

#broken casting after calculation


# ai3202
#Sam Skolnekovich

The second heuristic-
 a. Uses the pythagorean theorem to check the straight distance across weighted with *10.
	heurisic=10*(math.sqrt((goalnode.x-current_location.x)**2+(goalnode.x-current_location.x)**2

 b. I knew this would underestimate the solution. It is a straight line distance. It also is weighted less than 14 which is the diagonal distance, even in the best case grid this heuristic will underestimate. Because of an initial problem in my code I need to underestimate by about 30 units.

 c.My solution sets the initial start node f.cost 28 higher than it should be. This is a problem I have been trying to figure out, but for now I need to make sure the heuristic is low. In manhattan, the optimal solution is not always found with my code implementation. With my “pythagorean” it is nearly found but the cost is an overestimate, due to problems during the first node traversal. Because the initial node has an uncharacteristic cost it causes the second node of the solution to often times be wrong. It then corrects itself. There are also comments in the code to try and help explain.


world1.txt
Pythagorean
-31 squares traversed
-missed 2nd node
Descending order

9 0
8 0
7 1
7 2
6 3
5 3
4 4
4 5
3 6
2 7
1 6*
0 7


World1.txt
Manhattan
18 squares
suboptimal

9 0
8 0
7 0
6 0
5 0
4 1
3 2
2 2
1 3
1 4
1 5
1 6
0 7

'''
World2.txt
Py
-25 squares
-*missed 2nd node for optimal

9 0
8 1
7 2
6 3
5 3
4 4
3 5
3 6
2 7
1 6
0 7


World2.txt
Manhattan
-14 squares
missed 2nd node

9 0
8 1
7 2
6 3
5 3
4 4
3 5
3 6
2 7
1 6 *
0 7

'''
