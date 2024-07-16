package Week12;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.File;
import java.util.Scanner;

public class Main {
    public static void main(String[] args){

        try{
            BinarySearch b = new BinarySearch();
            Scanner s = new Scanner(new File(args[0]));
            int[] target = new int[100];
            int[] number = new int[100000];

            for(int i=0;i<target.length;i++)
            {target[i] = Integer.parseInt(s.nextLine());}
            for(int i=0;i<number.length;i++)
            {number[i] = Integer.parseInt(s.nextLine());}

            for(int i : target)
            {
                int index = b.binarySearch(number, 0,number.length, i);
                if(index == -1)
                {
                    System.out.println("target is not founded");
                }
                else {
                    System.out.print("target: " + i + "  index: "+ index);
                    System.out.println();
                }
            }
        }
        catch(FileNotFoundException e){
            System.out.println("input.txt was not found");
            System.exit(0);
        }catch(IOException e){
            System.out.println("Error reading from input.txt");
        }
    }
}
