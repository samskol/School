
public class ZombieApocalypse {

	/**
	 * @param args
	 */
	public static int days=0;
	// TODO Auto-generated method stub
    // set some number to be zombies
    private static int z = 1;
    //this is number of total ZombieTurtles, only z of which are initially zombies
    private static int zTurtles = 300;
    private static int N = 250;
    /*need a Universe object that we can put turtles into. turtles will be circles and change colors when they 
     * come in contact with other turtles that are inflicted with zombie-ism
     * 
     */
    public static void startIt(int b, int a, int c, int radius){
    	 Universe un = new Universe(zTurtles,z,600, 600);
    	 /*our simulation will run for N iterations */
    	 for(int i = 0; i < N; i++)
         {
             /*choose new random location for all Turtles in the Universe */
             un.moveZombies();
             /*Convert new zombies from Turtle population */
             un.zombieAttack(b,a,c,radius);
             days=days+1;
             System.out.println(days);
         }
             
    }
	public static void main(String[] args){
	int b = Integer.parseInt(args[0]);	
	int a= Integer.parseInt(args[1]);
	int c = Integer.parseInt(args[2]);
	int radius = Integer.parseInt(args[3]);
	startIt(b,a,c,radius);
	}

}

