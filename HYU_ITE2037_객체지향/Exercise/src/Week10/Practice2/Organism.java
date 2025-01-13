package Week10.Practice2;
import java.util.Random;
import java.util.ArrayList;

public abstract class Organism {
    private int x,y,liveTime;

    public Organism(){}
    public Organism(int x, int y){
        if(vaildCell(x,y)){
            this.x = x;
            this.y = y;
            liveTime = 0;
        }
    }
    public abstract void move(ArrayList<Doodlebugs>doo, ArrayList<Ants> ants);
    public abstract boolean liveTimeCheck();

    public void setX(int x){this.x = x;}
    public void setY(int y){this.y = y;}
    public int getX(){return x;}
    public int getY(){return y;}

    public void initialLiveTime(){liveTime=0;}
    public void incrementLiveTime()
    {liveTime++;}
    public int getLiveTime(){return liveTime;}

    public int[] n_location()
    {
        int nx = getX(), ny = getY();
        Random r = new Random();
        int ri = r.nextInt(4);
        switch (ri)
        {
            case 0:
                nx += 1;
                if(nx>9) nx -= 1;
                break;
            case 1:
                nx -= 1;
                if(nx<0) nx += 1;
                break;
            case 2:
                ny += 1;
                if(ny>9) ny -= 1;
                break;
            case 3:
                ny -= 1;
                if(ny<0) ny += 1;
                break;
        }
        int[] nLocation = new int[2];
        nLocation[0] = nx;
        nLocation[1] = ny;
        return nLocation;
    }

    private boolean vaildCell(int x, int y){
        if(x>=0 && x<10 && y>=0 && y<10) return true;
        return false;
    }
}
