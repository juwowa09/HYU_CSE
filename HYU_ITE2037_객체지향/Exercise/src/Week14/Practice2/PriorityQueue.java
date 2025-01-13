package Week14.Practice2;

import java.util.ArrayList;

public class PriorityQueue<T> {
    ArrayList<Integer> p;
    ArrayList<T> heap;

    public PriorityQueue() {
        p = new ArrayList<>();
        heap = new ArrayList<>();
    }

    public void add(T s,int priority){
        p.add(priority);
        heap.add(s);
        for(int i=0;i<p.size();i++)
        {
            if(priority>=p.get(i))
            {
                for(int j = p.size()-1;j>i;j--)
                {
                    p.set(j,p.get(j-1));
                    heap.set(j,heap.get(j-1));
                }
                p.set(i,priority);
                heap.set(i,s);
                break;
            }
        }
    }
    public T remove(){
        if(heap.size()==0) return null;
        T temp = heap.get(0);
        p.remove(heap.get(0));
        heap.remove(0);
        return temp;
    }
}
