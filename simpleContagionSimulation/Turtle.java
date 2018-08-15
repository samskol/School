import java.awt.Color;

public class Turtle extends ZombieApocalypse {

	/**
	 *
	 * @author user
	 */
		private int firstday=0;
	    private int rdays=0;
		private int x;
	    private int y;
	    private int zombie=0;
	    
	    public Turtle(int z)
	    {
	        this.zombie = z;
	    }
	    public void setLocation(int xIn, int yIn)
	    {
	        //erase old zombie location
	        StdDraw.setPenColor(StdDraw.WHITE);
	        StdDraw.filledCircle((double)this.x, (double)this.y, 6);

	        if (this.zombie == 1){
	            StdDraw.setPenColor(StdDraw.RED);
	           
	        }
	        if (this.zombie==0)
	            StdDraw.setPenColor(Color.BLUE);
	        if(this.zombie==2)
	        	StdDraw.setPenColor(Color.GREEN);
	        this.x = xIn;
	        this.y = yIn;
	        StdDraw.filledCircle((double)this.x, (double)this.y, 5);
	        //StdDraw.show(1);
	    }
	    public int isZombie()
	    {
	        return this.zombie;
	    }
	    public void makeZombie()
	    {
	    	this.zombie = 1;
	    	firstday=days;
	    }
	    public void makeSusceptible(){
	    	this.zombie=0;
	    }
	    public void makeRecovery(){
	    	this.zombie = 2;
	    	rdays=days;
	    }
	    public void makeChangeS(int c){
	    	if (days-rdays>c){
	    		makeSusceptible();
	    	}
	    }
	    public void makeChangeR(int a){
	    	
	    	if (days-firstday>a){
	    		makeRecovery();
	    	}
	    }
	    public void makeZombiesAttack(int b){
	    	double x=Math.random()*100;
	    	if(x<=b && this.zombie!=2){
	    		makeZombie();
	    	}
	    }
	    public int getXLocation()
	    {
	        return this.x;
	    }
	    
	    public int getYLocation()
	    {
	        return this.y;
	    }

}