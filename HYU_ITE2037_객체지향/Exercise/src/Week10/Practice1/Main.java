package Week10.Practice1;

public class Main {
    public static void main(String[] argv)
    {
        Taxi t1 = new GeneralTaxi(1234,2.1);
        Taxi t2 = new DeluxeTaxi(2345, 6.1);

        System.out.println(t1.toString());
        System.out.println(t2.toString());
        t1.doDrive(4);
        t2.doDrive(4);

//        t2.doDrive(5);

        System.out.println(t1.toString());
        System.out.println(t2.toString());
        t1.doDrive(10);
        t2.doDrive(3);
        System.out.println(t1.toString());
        System.out.println(t2.toString());
        if(t1.earnMore(t2)) System.out.println("t1 earn more than t2");
        else System.out.println("t2 earn more than t1");
    }
}
