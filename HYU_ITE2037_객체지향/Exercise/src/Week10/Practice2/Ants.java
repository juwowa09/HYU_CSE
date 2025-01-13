package Week10.Practice2;
import java.util.Random;
import java.util.ArrayList;

public class Ants extends Organism{
    private String shape;
    public Ants(){super();}
    public Ants(int x, int y){
        super(x,y);
        shape = "o";
    }

    @Override
    public void move(ArrayList<Doodlebugs>doo, ArrayList<Ants> ants){
        int[] l = n_location();
        int cnt = 0;
        for(int i=0;i<doo.size();i++)
        {
            if(l[0] == doo.get(i).getX() && l[1] == doo.get(i).getY())
            {cnt=1;break;}
        }
        for(int i=0;i<ants.size();i++)
        {
            if (cnt == 0) {
                if (l[0] == ants.get(i).getX() && l[1] == ants.get(i).getY()) {
                    cnt = 1;
                    break;
                }
            }
            else break;
        }
        if(cnt==1){return;}
        setX(l[0]);
        setY(l[1]);
    }

    public Ants breed(){
        Ants newAnts = new Ants(getX(),getY());
        return newAnts;
    }

    @Override
    public boolean liveTimeCheck(){
        if(getLiveTime() == 3)return true;
        return false;
    }
}
