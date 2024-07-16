public class LotteryChecker {
    public int Check(int[] win, int[] numbers)
    {
        int cnt=0;
        int bns=0;
        for(int i=0;i<7;i++)
        {
            for(int j=0;j<6;j++)
            {
                if(i == 6)  // 보너스번호 체크
                {
                    if (numbers[j] == win[i])
                    {
                        bns++;
                        break;
                    }
                }
                else
                {   // 당청번호와 내 로또번호가 맞을 경우 count.
                    if (numbers[j] == win[i])
                    {
                        cnt++;
                        break;
                    }
                }
            }
        }
        if(cnt == 6 || (cnt==5 && bns == 1)) return 1;
        else if(cnt == 5 || (cnt == 4 && bns == 1)) return 2;
        else if (cnt == 4) return 3;
        else if(cnt == 3) return 4;
        else return 0;
    }
}
