package Week14.Practice1;
import java.util.Scanner;
import java.util.Iterator;

public class Main {
    public static void main(String[] argv){
        Scanner s = new Scanner(System.in);


        System.out.print("Input max number: ");
        int number = s.nextInt();
        Eratos e = new Eratos(number);
        e.setPrime();

        Iterator<Integer> it = e.getPrime().iterator();
        while(it.hasNext()){
            System.out.print(it.next()+" ");
        }
    }
}
