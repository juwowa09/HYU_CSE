package Week10.Practice2;
import java.util.Random;
import java.util.ArrayList;

public class Doodlebugs extends Organism{
    private int starveCnt;
    private String shape;

    public Doodlebugs(){super();}
    public Doodlebugs(int x,int y){
        super(x,y);
        starveCnt = 0;
        shape = "X";
    }

    @Override
    public void move(ArrayList<Doodlebugs>doo, ArrayList<Ants> ants){
        int [] l = n_location();
        int cnt = 0;
        for(int i = 0; i<ants.size();i++)
        {
            if(getX() + 1 == ants.get(i).getX() && getY() == ants.get(i).getY()) {
                ants.remove(i);
                setX(getX()+1);
                initialStarveCnt();
                return;
            }
            else if(getX() - 1 == ants.get(i).getX() && getY() == ants.get(i).getY()) {
                ants.remove(i);
                setX(getX()-1);
                initialStarveCnt();
                return;
            }
            else if(getX() == ants.get(i).getX() && getY() + 1 == ants.get(i).getY()){
                ants.remove(i);
                setY(getY() + 1);
                initialStarveCnt();
                return;
            }
            else if(getX() == ants.get(i).getX() && getY() - 1 == ants.get(i).getY()){
                ants.remove(i);
                setY(getY() - 1);
                initialStarveCnt();
                return;
            }
        }
        for(int j=0;j<doo.size();j++)
        {
            if(l[0] == doo.get(j).getX() && l[1] == doo.get(j).getY())
            {cnt=1;}
        }
        if(cnt==1)return;
        setX(l[0]);
        setY(l[1]);
    }

    public Doodlebugs breed() {
        Doodlebugs newDbugs = new Doodlebugs(getX(), getY());
        return newDbugs;
    }

    public void incrementStarveCnt(){starveCnt++;}
    public int getStarveCnt(){return starveCnt;}
    public void initialStarveCnt(){starveCnt = 0;}

    public boolean starveTimeCheck(){
        if(getStarveCnt() == 3) return true;
        return false;
    }

    @Override
    public boolean liveTimeCheck() {
        if(getLiveTime() == 8) return true;
        return false;
    }
}
