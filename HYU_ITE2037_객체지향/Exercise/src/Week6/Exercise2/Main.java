package Week6.Exercise2;
import java.util.Scanner;

public class Main {
    public static void main(String[] argv)
    {
        Scanner S = new Scanner(System.in);
        int a = S.nextInt();
        int b = S.nextInt();
        System.out.println(lcm(a,b));
        System.out.println(gcd(a,b));
    }
    public static int lcm(int a, int b) {
        return (a * b) / gcd(a, b);
    }

    public static int gcd(int a, int b) {
        int r;
        while (b != 0) {
            r = a % b;
            a = b;
            b = r;
        }
        return a;
    }
}

