package Week14.Practice2;
import java.util.ArrayList;

public class Main {
    public static void main(String[] argv)
    {
        PriorityQueue<String> q = new PriorityQueue();
        q.add("X",10);
        q.add("Y",1);
        q.add("Z",3);
        q.add("V",2);
        q.add("B",4);
        q.add("A",5);
        q.add("D",6);
        q.add("S",7);
        q.add("E",8);

        System.out.println(q.remove());
        System.out.println(q.remove());
        System.out.println(q.remove());
        System.out.println(q.remove());
        System.out.println(q.remove());
        System.out.println(q.remove());
        System.out.println(q.remove());
        System.out.println(q.remove());
        System.out.println(q.remove());
    }
}
