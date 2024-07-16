package Week10.Practice2;

import java.util.ArrayList;

public class Grid {
    private char[][] cell;
    public Grid()
    {
        cell = new char[10][10];
        for(int i=0;i<10;i++)
        {
            for(int j=0;j<10;j++){
                cell[i][j] = '*';
            }
        }
    }

    public void moveOrganism(ArrayList<Doodlebugs> doo, ArrayList<Ants> ants){
        int n = doo.size();
        for(int i=0; i<n; i++)
        {
            cell[doo.get(i).getX()][doo.get(i).getY()] = '*';
            doo.get(i).incrementStarveCnt();
            doo.get(i).move(doo,ants);
            doo.get(i).incrementLiveTime();
            cell[doo.get(i).getX()][doo.get(i).getY()] = 'X';
            if(doo.get(i).starveTimeCheck()){
                cell[doo.get(i).getX()][doo.get(i).getY()] = '*';
                doo.remove(i);
                n--;
                i--;
            }
            else if(doo.get(i).liveTimeCheck()){
                doo.get(i).initialLiveTime();
                Doodlebugs d = doo.get(i).breed();
                if(this.getCell(d.getX()+1,d.getY()) == '*')
                {d.setX(d.getX()+1); doo.add(d);
                    cell[d.getX()][d.getY()]='X';}
                else if (this.getCell(d.getX()-1,d.getY()) == '*')
                {d.setX(d.getX()-1); doo.add(d);
                    cell[d.getX()][d.getY()]='X';}
                else if (this.getCell(d.getX(),d.getY()+1) == '*')
                {d.setY(d.getY()+1); doo.add(d);
                    cell[d.getX()][d.getY()]='X';}
                else if (this.getCell(d.getX(),d.getY()-1) == '*')
                {d.setY(d.getY()-1); doo.add(d);
                    cell[d.getX()][d.getY()]='X';}
            }

        }
        int m = ants.size();
        for(int i=0;i<m;i++)
        {
            cell[ants.get(i).getX()][ants.get(i).getY()] = '*';
            ants.get(i).move(doo,ants);
            ants.get(i).incrementLiveTime();
            cell[ants.get(i).getX()][ants.get(i).getY()] = 'o';
            if(ants.get(i).liveTimeCheck()){
                ants.get(i).initialLiveTime();
                Ants a = ants.get(i).breed();
                if(this.getCell(a.getX()+1,a.getY()) == '*')
                {a.setX(a.getX()+1); ants.add(a);
                    cell[a.getX()][a.getY()]='o';}
                else if (this.getCell(a.getX()-1,a.getY()) == '*')
                {a.setX(a.getX()-1); ants.add(a);
                    cell[a.getX()][a.getY()]='o';}
                else if (this.getCell(a.getX(),a.getY()+1) == '*')
                {a.setY(a.getY()+1); ants.add(a);
                    cell[a.getX()][a.getY()]='o';}
                else if (this.getCell(a.getX(),a.getY()-1) == '*')
                {a.setY(a.getY()-1); ants.add(a);
                cell[a.getX()][a.getY()]='o';}
            }
        }
    }
    public void printAnt(Ants a)
    {

    }
    public void print()
    {
        for(int i=0;i<10;i++)
        {
            for(int j =0;j<10;j++)
            {
                System.out.print(cell[i][j]+" ");
            }
            System.out.println();
        }
    }
    public char getCell(int x, int y){
        if(x>=0 && x<10 && y>=0 && y<10)
            return cell[x][y];
        else return ' ';
    }

}
