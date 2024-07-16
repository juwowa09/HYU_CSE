package Week14.Practice1;
import java.util.ArrayList;

public class Eratos {
    private ArrayList<Integer> arr;
    private ArrayList<Integer> prime;

    public Eratos(int number) {
        arr = new ArrayList<>();
        prime = new ArrayList<>();

        for(int i=1;i<number;i++)
        { arr.add(i);}
    }
    public ArrayList<Integer>getPrime(){
        return prime;
    }

    public void setPrime() {
        arr.remove((Integer) 1);

        for (int i = 0; i < arr.size(); i++) {
            int temp = arr.get(i);
            prime.add(temp);
            i--;
            for (int j = 0; j < arr.size(); j++) {
                if (arr.get(j) % temp == 0) {
                    arr.remove(j);
                    j--;
                }
            }
        }
    }
}

