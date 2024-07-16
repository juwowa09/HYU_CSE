package Week11.Practice1;
import java.util.Scanner;
import java.util.InputMismatchException;

public class Main {
    public static void main(String[] args) {
        Scanner s = new Scanner(System.in);
        while (true) {
            try {
                System.out.println("Input a and b :");
                int a = s.nextInt();
                int b = s.nextInt();
                if (isCoprime(a, b)) {
                    System.out.println("Two number are Coprime");
                } else {
                    System.out.println("Two number are not Coprime");
                }
            } catch (MyException e) {
                System.out.println(e.getMessage());
                continue;
            } catch (InputMismatchException e) {
                System.out.println("[InputMismatchException] input type is not integer");
                break;
            }
        }
    }

    public static boolean isCoprime(int a, int b)throws MyException{
        if(a == b) throw new MyException();
        if(a<=1 || b<=1) throw new MyException("One of a and b is less or equal to 1.");
        if(a>10000 && b>10000) throw new MyException("Both a and b are larger than 10000 One of them less than 10000");

        int great = 1; // 최대 공약수

        for(int i = 1; i <= a; i++)
        {
            if(a % i == 0 && b % i == 0)
            {great = i;}
        }
        if(great == 1){return true;}
        else{return false;}
    }
}

