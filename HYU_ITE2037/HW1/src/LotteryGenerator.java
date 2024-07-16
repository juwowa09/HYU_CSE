import java.util.Random;
public class LotteryGenerator {
    public int[] generate(int a){
        int[] lottery = new int[a];
        for(int i=0;i<a;i++)
        {
            int cnt = i+1;  // 숫자 중복 방지 카운트변수
            Random rd = new Random();   // 난수 생성
            int number = rd.nextInt(20)+1;

            // 숫자 중복 방지하는 for문
            for(int j=0;j<=i;j++)
            {
                if(number != lottery[j])
                    cnt--;
            }
            // 중복 없을 시 선택
            if(cnt == 0)
                lottery[i] = number ;
            else
            {   // 숫자 중복이면, 다시 선택
                i-=1;
                continue;
            }
        }
        // 로또 당첨 번호 보기 좋게 정렬
        if(a==7){
            for(int i=0; i<5; i++)
            {
                for(int j = 0; j<5-i; j++)
                {
                    if(lottery[j]>lottery[j+1]){
                        int temp = lottery[j];
                        lottery[j] = lottery[j+1];
                        lottery[j+1] = temp;
                    }
                }
            }
        }
        return lottery;
    }
}
