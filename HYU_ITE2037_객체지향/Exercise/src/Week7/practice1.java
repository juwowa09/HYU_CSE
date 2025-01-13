package Week7;
import java.util.Scanner;

public class practice1 {
    public static void main(String[] argv)
    {
        Scanner input = new Scanner(System.in);
        System.out.print("Input your snail scale : ");
        int n = input.nextInt();
        int [][] snail = drawSnail(n);
        for(int i=0;i<n;i++)
        {
            for(int j=0;j<n;j++)
            {
                System.out.printf("%4d",snail[i][j]);
            }
            System.out.println();
        }
    }
    public static int[][] drawSnail(int n)
    {
        int direct=1, k=0, i=0, j=-1,v=n;
        int[][] snail = new int[n][n];
        while(true)
        {
            for(int l=0;l<v;l++)
            {
                k = k+1;
                j = j+direct;
                snail[i][j] = k;
            }
            v = v - 1;
            if(v<=0)break;
            for(int m=0;m<v;m++)
            {
                k=k+1;
                i=i+direct;
                snail[i][j] = k;
            }
            direct *= -1;
        }

        int[][] s = new int[n][n];
        for( i=0;i<n;i++)
        {
            for( j=0;j<n;j++)
            {
                s[i][j] = snail[i][j];
            }
        }
        return s;
    }
}
