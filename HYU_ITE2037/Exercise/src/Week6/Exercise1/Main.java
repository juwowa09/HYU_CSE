package Week6.Exercise1;
import Week6.Exercise1.*;
import java.util.Date;
import java.util.Scanner;
import java.util.StringTokenizer;
import Week6.Exercise1.util.*;

import Week5.Student;
import Week6.Exercise1.util.AgeCalculator;
public class Main {
    int m;
    public static void main(String [] argv)
    {
        int[] a = new int[1];
        a[0] = 5;

        Person p1 = makePerson();
        Person p2 = makePerson();

        System.out.println(p1.toString());
        System.out.println(p2.toString());
        System.out.println(p1.getName()+"`s age is " + AgeCalculator.calAge(p1));
        System.out.println(p2.getName()+"`s age is " + AgeCalculator.calAge(p2));
        if(AgeCalculator.isOlder(p1,p2) == 1){
            System.out.println(p1.getName()+"`s age "+AgeCalculator.calAge(p1)
                    +" is Older than "+p2.getName()+"`s age "+AgeCalculator.calAge(p2));
        }else if (AgeCalculator.isOlder(p1,p2) == -1)
        {
            System.out.println(p2.getName()+"`s age "+ AgeCalculator.calAge(p2)
                    +" is Older than "+p1.getName()+"`s age "+AgeCalculator.calAge(p1));
        }else{System.out.println(p1.getName()+" and "+p2.getName()+" are same Age " + AgeCalculator.calAge(p2));}
    }

    static Person makePerson(){
        Person p = null;
        int year,month,day,i=0;
        String name = null;
        Scanner input = new Scanner(System.in);
        System.out.println("Input name and born dates");
        String inform = input.nextLine();
        StringTokenizer stk = new StringTokenizer(inform);

        name = stk.nextToken();
        year = Integer.parseInt(stk.nextToken());
        month = Integer.parseInt(stk.nextToken());
        day = Integer.parseInt(stk.nextToken());
        stk = null;
        inform = null;

        Date myDate1 = new Date(year,month,day);
        System.out.println("Input died dates");
        inform = input.nextLine();
        if(inform !="") {
            stk = new StringTokenizer(inform);

            year = Integer.parseInt(stk.nextToken());
            month = Integer.parseInt(stk.nextToken());
            day = Integer.parseInt(stk.nextToken());
            Date myDate2 = new Date(year, month, day);
            stk = null;
            inform = null;
            p = new Person(name,myDate1,myDate2);
        }
        else{
            p = new Person(name,myDate1,null);
        }

        return p;
    }
}
