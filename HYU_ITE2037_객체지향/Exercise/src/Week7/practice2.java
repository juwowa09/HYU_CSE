package Week7;
import java.util.Scanner;
import java.util.Random; // 1~8까지

public class practice2 {
    public static void main(String[] argv) {
        Scanner input = new Scanner(System.in);
        String[][] head = new String[4][4];
        int [][] tail = new int [4][4];
        int[] arr = new int[16];
        int[] arr1 = new int[16];

        // 랜덤으로 1~16 까지의 index를 element로 가질 arr 생성
        for(int i=0;i<16;i++)
        {
            int cnt = i+1;
            Random rd = new Random();   // 난수 생성
            int number = rd.nextInt(16)+1;

            // 숫자 중복 방지하는 for문
            for(int j=0;j<=i;j++)
            {
                if(number != arr[j])
                    cnt--;
            }
            // 중복 없을 시 선택
            if(cnt == 0)
                arr[i] = number ;
            else
            {   // 숫자 중복이면, 다시 선택
                i-=1;
                continue;
            }
        }
        // 랜덤한 index에 1~8까지 숫자 두개씩 가지는 arr1 생성
        for(int i=1;i<17;i++)
        {
            arr1[arr[i-1]-1] = (i+1)/2;
        }
        // 실제로 사용 할 카드 뒷면 tail 생성, 카드 앞면 head 생성
        for(int i=0;i<4;i++)
        {
            for(int j=0; j<4; j++)
            {
                tail[i][j] = arr1[i*4+j];
                head[i][j] = "*";
            }
        }
        arr1 = null;
        arr = null;
        System.out.printf("\t");
        for(int i=0;i<4;i++)
        {System.out.print((i+1)+"\t");}
        System.out.println();

        for(int i=0;i<4;i++)
        {
            System.out.print((i+1)+"\t");
            for(int j=0; j<4; j++)
            {
                System.out.print(head[i][j] +"\t");
            }
            System.out.println();
        }

        int cnt = 8;
        while(cnt>0)
        {
            int f1=0,f2=0, s1=0,s2=0;
            int first =0;
            int second =0;
            System.out.print("Enter Coordinate: ");

            f1 = input.nextInt();
            f2 = input.nextInt();
            first = tail[f1-1][f2-1];
            s1 = input.nextInt();
            s2 = input.nextInt();
            second = tail[s1-1][s2-1];
            if(f1 == s1 && f2 == s2) continue;
            if(first == second)
            {
                System.out.println(first +" == "+second);
                cnt--;
                head[f1-1][f2-1] = String.valueOf(tail[f1-1][f2-1]);
                head[s1-1][s2-1] = String.valueOf(tail[f1-1][f2-1]);
            }
            else System.out.println(first +" != "+second);

            System.out.printf("\t");
            for(int i=0;i<4;i++)
            {System.out.print((i+1)+"\t");}
            System.out.println();

            for(int i=0;i<4;i++)
            {
                System.out.print((i+1)+"\t");
                for(int j=0; j<4; j++)
                {
                    System.out.print(head[i][j] +"\t");
                }
                System.out.println();
            }
        }
    }
}