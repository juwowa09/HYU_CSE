package Week5;
import java.util.Scanner;
import java.util.StringTokenizer;

public class Practice {
    public static void main(String[] argv){
        int year,month,day,i=0;
        Scanner input = new Scanner(System.in);
        String first = input.nextLine();
        String second = input.nextLine();

        String[] arr = first.split(" ");
        StringTokenizer stk = new StringTokenizer(arr[1],"/");

        year = Integer.parseInt(stk.nextToken());
        month = Integer.parseInt(stk.nextToken());
        day = Integer.parseInt(stk.nextToken());

        Student a = new Student(arr[0],year,month,day);
        Student b = new Student(second,year);

        if(a.checkDate()){
            a.printInform();
            b.printInform();

            if(a.isOlder(b))
            {
                if(a.getDay() == b.getDay())
                {System.out.println("Same Age");}
                else
                {System.out.println(a.getName()+" is older than "+b.getName());}
            }
            else System.out.println(b.getName()+" is older than "+a.getName());
        }
        else System.out.println("ivnalid input");

    }
}
