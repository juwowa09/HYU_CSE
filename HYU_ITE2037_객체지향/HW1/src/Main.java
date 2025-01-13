import java.util.Scanner;

public class Main {
    public static void main(String[] args)
    {
        LotteryGenerator L = new LotteryGenerator();
        LotteryChecker C = new LotteryChecker();
        Scanner input = new Scanner(System.in);

        // 남은 돈과 티켓 구매수 저장할 배열
        int[] a = new int[2];

        while(true)
        {   // 당첨 로또번호 생성
            int[] placeArr = new int[4];
            int[] winNumber = L.generate(7);
            InputFormat(input, a);

            System.out.print(">>Round Winning number : ");
            for(int i=0; i<6; i++)
            {
                System.out.printf("%d ",winNumber[i]);
            }
            System.out.printf("+ %d\n",winNumber[6]);

            for(int i=0;i<a[1];i++)
            {   // 내가 산 로또 번호 생성, 당첨 check
                int[] number = L.generate(6);
                int place = C.Check(winNumber, number);

                if(place !=0)
                {   // 당첨 된 경우 if statement 실행
                    placeArr[place-1]++;
                    // 몇등으로 몇개 당첨 됐는지 count
                    System.out.print("Lottery number: ");

                    for(int j=0;j<6;j++)
                    {   // 당첨된 로또 번호, 등수 출력
                        System.out.printf("%d ",number[j]);
                    }System.out.println("Winner ("+place+"st place)");
                }
                number = null;
            }System.out.println("Remaining money : "+a[0]);
            // 남은돈 출력
            for(int i=0;i<4;i++)
            {   //몇등으로 몇개 당첨됐는지 출력.
                System.out.println((i+1)+"st place:" +placeArr[i]);
            }System.out.println();

            placeArr = null;
            winNumber = null;
        }
    }
    public static void InputFormat(Scanner i, int[] a)
    {
        int m, t;
        do {
            System.out.println(">>Input your money number of lottery ticket:");
            System.out.print("<<");
            m = i.nextInt();
            t = i.nextInt();
            i.nextLine();

            if(m == 0 || t == 0)
            {   // 프로그램 종료 statement
                System.out.println(">>End of program");
                System.exit(0);
            }
        } while(a[0] + m -(t*10) < 0);  // 가지고있는 돈보다 많은 로또 사려면 다시

        // 남은 돈 지갑에 넣어주기, 티켓 구매수
        a[0] = a[0] + m - (t*10);
        a[1] = t;
    }
}
