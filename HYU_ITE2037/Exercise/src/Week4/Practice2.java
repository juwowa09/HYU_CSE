package Week4;
import java.util.Scanner;
import java.util.Random;
public class Practice2 {
    public static void main(String[] argv){
        int score =0,roll=0,Cscore =0,sum=0;
        Random r = new Random();
        Scanner input = new Scanner(System.in);
        String str = null;

        while(score < 100 && Cscore < 100)
        {
            do {
                System.out.print("(You) insert action r(roll): ");
                str = input.next();

                if(str.equals("r"))
                {
                    roll = r.nextInt(6)+1;
                    sum += roll;
                    System.out.println("(You) dice rolled : " + roll);

                    if(roll == 1){
                        System.out.println("(You) No score added");
                        System.out.println("(You) Your score "+score+", Computer score "+Cscore);
                        System.out.println();
                        sum=0;
                        break;
                    }
                }else if(str.equals("h")){
                    score += sum;
                    System.out.println("(You) "+ sum +" added to You");
                    System.out.println("(You) Your score "+score+", Computer score "+Cscore);
                    System.out.println();
                }
            } while(!str.equals("h"));

            sum = 0;
            do{
                System.out.println("(Computer) insert action r(roll): r");
                roll = r.nextInt(6)+1;
                sum += roll;
                System.out.println("(Computer) dice rolled : " + roll);

                if(roll == 1){
                    System.out.println("(Computer) No score added");
                    System.out.println("(Computer) Your score "+score+", Computer score "+Cscore);
                    System.out.println();
                    sum = 0;
                    break;
                }
                if(sum>=20){
                    Cscore += sum;
                    System.out.println("(Computer) "+ sum+" added to Computer");
                    System.out.println("(Computer) Your score "+score+", Computer score "+Cscore);
                    System.out.println();
                }
            } while(sum<20);
            sum =0;
        }

        if(score>Cscore)
        {
            System.out.println("(Game) Your score "+score+", Computer score "+Cscore);
            System.out.println("Congratulations! You win!");
        }
        else{
            System.out.println("(Game) Your score "+score+", Computer score "+Cscore);
            System.out.println("Sorry, Computer wins.");
        }
    }
}
