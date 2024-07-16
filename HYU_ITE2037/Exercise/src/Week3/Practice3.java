package Week3;
import java.util.Scanner;
public class Practice3 {
    public static void main(String[] argv)
    {
        Item[] item = new Item[3];
        Scanner input = new Scanner(System.in);
        double total=.0;
        for(int i=0;i<3;i++)
        {
            System.out.println("Enter name of item "+(i+1));
            String name = input.nextLine();
            System.out.println("Enter quantity of item "+(i+1));
            int quantity = input.nextInt();
            input.nextLine();
            System.out.println("Enter price of item "+(i+1));
            double price = input.nextDouble();
            input.nextLine();
            item[i] = new Item();
            item[i].name = name;
            item[i].quantity = quantity;
            item[i].price = price;
        }
        System.out.println("Your bill:");
        System.out.println();
        System.out.printf("%-30s%-10s%-10s%-10s\n","ITEM","QUANTITY","PRICE","TOTAL");
        for(int i=0;i<3;i++)
        {
            System.out.printf("%-30s%-10d%-10.2f%-10.2f\n",item[i].name,item[i].quantity,item[i].price,item[i].price*item[i].quantity);
            total += item[i].price*item[i].quantity;
        }System.out.println();
        System.out.printf("%-50s%-10.2f\n","SUBTOTAL",total);
        System.out.printf("%-50s%-10.2f\n","6.25% SALES TAX",(total*0.0625));
        System.out.printf("%-50s%-10.2f\n","TOTAL",total+(total*0.0625));

    }
    public static class Item{
        String name = null;
        int quantity = 0;
        double price = .0;
    }
}
