package Week3;
import java.util.Scanner;
public class Practice2 {
    public static void main(String[] argv)
    {
        Scanner input = new Scanner(System.in);
        int per = input.nextInt();
        System.out.printf("(%d/100)*4=%d\n",per,per*4/100);
    }
}
