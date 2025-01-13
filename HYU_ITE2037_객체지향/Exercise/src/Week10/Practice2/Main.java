package Week10.Practice2;
import java.util.Scanner;
import java.util.ArrayList;

public class Main {
    public static void main(String[] argv)
    {
        Scanner input = new Scanner(System.in);
        Grid g = new Grid();
        g.print();
        String s = null;
        ArrayList<Ants>ants = new ArrayList<>();
        ArrayList<Doodlebugs>doo = new ArrayList<>();

        System.out.println("Press d Enter");
        s = input.nextLine();
        for(int i=0;i<20;i++)
        {
            Ants a;
            int y = i;
            if(y>9) {
                y-=10;
                a = new Ants(9,y);
            }
            else a = new Ants(0,y);
            ants.add(a);
        }
        for(int i=0;i<2;i++)
        {
            Doodlebugs d = new Doodlebugs(2,i);
            doo.add(d);
        }
        while(s.equals("d"))
        {
            g.moveOrganism(doo,ants);
            g.print();
            System.out.println("You want to next Press d Enter");
            s = input.nextLine();
        }
    }
}
