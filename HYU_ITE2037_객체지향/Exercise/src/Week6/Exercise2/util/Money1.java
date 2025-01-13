package Week6.Exercise2.util;

public class Money1 {
    private int dollars;
    private int cents;

    Money1(){};
    public Money1(int d, int c){
        this.dollars = d;
        this.cents = c;
    };

    public Money1(int d) {
        this.dollars = d;
        this.cents = 0;
    }

    public static Money1 add(Money1 m1, Money1 m2) {
        Money1 m = new Money1(m1.getDollars() + m2.getDollars()
                        , m1.getCents() + m2.getCents());
        return m;
    }
    public static Money1 add(Money1 m1){
        Money1 m=null;
        return m;
    }

    public static Money1 minus(Money1 m1, Money1 m2){
        Money1 m = null;
        int d = 0;
        int c = 0;
        d = m1.getDollars() - m2.getDollars();
        if( d > 0 )
        {
            c = m1.getCents() - m2.getCents();
            if(c < 0) {
                d--;
                c += 100;
            }
        }
        else if( d < 0 )
        {
            d = d* -1;
            c = m2.getCents() - m1.getCents();
            if(c < 0) {
                d--;
                c += 100;
            }
        }
        else {
            c = m1.getCents() - m2.getCents();
            if(c < 0) {c *=-1;}
        }
        m = new Money1(d,c);
        return m;
    }

    public void setMoney(int d, int c){
        this.dollars = d;
        this.cents = c;
    }

    public String toString()
    {
        return dollars + "dollars, " + cents + "cents";
    }

    public boolean equals(Money1 m){
        if(m.dollars == this.dollars && m.cents == this.cents)
            return true;
        else return false;
    }
    public int getDollars()
    {return this.dollars;}
    public int getCents()
    {return this.cents;}
}
