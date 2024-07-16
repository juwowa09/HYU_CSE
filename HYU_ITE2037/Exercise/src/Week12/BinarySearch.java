package Week12;

public class BinarySearch {
    public int binarySearch(int[] arr, int start, int end, int x){
        if(start <= end){
            int middle = (start+end)/2;
            if(arr[middle] == x) return middle;
            if(arr[middle] > x) return binarySearch(arr, start, middle-1, x);
            if(arr[middle] < x) return binarySearch(arr, middle+1, end, x);
        }
        return -1;
    }
}
