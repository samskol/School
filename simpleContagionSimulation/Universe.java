/* We needa virtual physical space in which to place our Turtle objects. We'll call this space Universe, which means that we have a Universe
 * object and in our Lecture31ObjectSpatialAwareness.java code, we'll create an instance of Universe.
 */

/**
 *
 * @author user
 */
public class Universe  extends ZombieApocalypse {
    //N will be the number of turtle objects in our universe
    private int N;
    private Turtle[] turtle; 
    private int tLocations[][];
    private int width;
    private int height;
    
    /*we want to have objects interact, so radius is how many 
     * squares around the Turtle can the Turtle "see". We are 
     * assuming that space
     * is discrete here, that each Turtle has an (x,y) position 
     * on the grid.
     */
    
    /*the constructor, takes an int and uses this to create N 
     * instances of Turtle and place them in the Universe.*/
    public Universe(int numTurtles, int numZombies, int w, int h)
    {
        StdDraw.setCanvasSize(w, h);
        StdDraw.setXscale(0, w);
        StdDraw.setYscale(0, h);


        /*set the Universe instance variables using arguments to the 
         * Universe constructor*/
        tLocations = new int[w][h];
        N = numTurtles;
        turtle = new Turtle[N];
        
        width = w;
        height = h;
        /*initialize tLocations to have no turtle anywhere on the grid*/
        for(int x = 0; x < width; x++)
            for(int y = 0; y < height; y++)
                tLocations[x][y] = -1;
        
        /*Create the Turtles in the Universe and put 
         * them at random (x,y) locations.
         * Notice that the Turtle class also has 
         * it's own constructor.
         */
        
        for(int i = 0; i < N; i++)
        {
            if (i < numZombies)
                turtle[i] = new Turtle(1);
            else
                turtle[i] = new Turtle(0);
            //generate random integer location for each turtle
            int x = (int)(Math.random()*width);
            int y = (int)(Math.random()*height);
            tLocations[x][y] = i; 
            /*this only lets one at a location, we'll 
             * assume that means a zombie is hiding
             */
            /*setLocation is a method in Turtle. We call 
             * it here to set the turtle location*/
            turtle[i].setLocation(x, y);
        }
    }
    public void moveZombies()
    {
        /*This method is part of the Universe and 
         * determines how each turtle will move given 
         * it's current location.
         * We take it's current location, select a 
         * random integer between min and max and add 
         * that value to the x and y position 
         * of the turtle. The tLocations matrix tracks 
         * each turtle in the Universe.
         */
        int min = -8;
        int max = 8;
        int newX;
        int newY;
        for(int x = 0; x < N; x++)
        {
          int oldX = turtle[x].getXLocation();
          int oldY = turtle[x].getYLocation();

          newX = min + (int)(Math.random() * ((max - min)+1));
          if ((oldX+newX < 0) || (oldX+newX >= width)) 
            newX = 0;
          newY = min + (int)(Math.random() * ((max - min)+1));
          if ((oldY+newY < 0) || (oldY+newY >= width)) 
            newY = 0;

          tLocations[oldX][oldY] = -1;
          tLocations[oldX+newX][oldY+newY] = x;
          turtle[x].setLocation(oldX+newX, oldY+newY);
        }
    }
    public void zombieAttack(int b, int a, int c, int radius)
    {
        /* Spread zombie-ism
         */
        for(int x = 0; x < N; x++)
        {
        	if (turtle[x].isZombie()==2){
                turtle[x].makeChangeS(c);
            	}
        	
        	if(turtle[x].isZombie()==1)
        	{
        		int locX = turtle[x].getXLocation();
                int locY = turtle[x].getYLocation();
                int xLow;
                int xHigh;
                int yLow;
                int yHigh;
                
                turtle[x].makeChangeR(a);
                
                if (locX - radius < 0)
                    xLow = 0;
                else
                    xLow = locX - radius;

                if (locX + radius >= width)
                xHigh = width-1;
                else
                    xHigh = locX + radius;

                if (locY - radius < 0)
                    yLow = 0;
                else
                    yLow = locY - radius;

                if (locY + radius >= height)
                    yHigh = height-1;
                else
                    yHigh = locY + radius;
                
                for(int x2 = xLow; x2 < xHigh; x2++)
                {
                    for(int y2 = yLow; y2 < yHigh; y2++)
                    {
                        if(tLocations[x2][y2] >= 0) //there's a turtle there to infect
                        {
                            	
                            turtle[tLocations[x2][y2]].makeZombiesAttack(b);
                            //System.out.println("zombie attack");
                        }
                    }
                }  
            }
        }
    }
}
    
    
