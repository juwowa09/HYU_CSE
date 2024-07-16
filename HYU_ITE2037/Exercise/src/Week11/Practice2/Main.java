package Week11.Practice2;
import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Account a = null;
        Scanner s = new Scanner(System.in);
        while (true) {
            try {
                System.out.println("Input Account : ");
                String ID = s.next();
                String account = s.next();
                int b = s.nextInt();
                a = new Account(ID,account,b);
            } catch (MyException e) {
                System.out.println(e.getMessage());
                continue;
            }
            System.out.println(a.toString());
            break;
        }
    }
}
