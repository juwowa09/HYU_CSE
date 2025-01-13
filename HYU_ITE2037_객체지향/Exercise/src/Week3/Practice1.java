package Week3;

import java.util.Scanner;

public class Practice1 {
    public static void main(String[] argv) {
        Scanner input = new Scanner(System.in);
        String line = input.nextLine();
        String[] intArr = line.split(",");
        Integer a = Integer.valueOf(intArr[0]);
        Integer b = Integer.valueOf(intArr[1]);

        System.out.println("dividend: "+a);
        System.out.println("divisor: "+b);
        System.out.println("quotient: "+a/b);
        System.out.println("remainer: "+a%b);
    }
}
