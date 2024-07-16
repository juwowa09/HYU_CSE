package Week6.Exercise1;
import java.util.Date;

public class Person {
    private String name;
    private Date born;
    private Date died;

    Person(){}

    public Person(String _name, Date _born, Date _died)
    {
        if(!consistent(_born, _died))
        {
            System.out.println("Inconsistent dates");
            System.exit(1);
        }
        else{
            this.name = _name;
            this.born = new Date(_born.getYear(), _born.getMonth(), _born.getDate());
            if(_died != null)
                this.died = new Date(_died.getYear(), _died.getMonth(), _died.getDate());
            else this.died = null;
        }
    };

    public String toString()
    {
        String result = "Name : " + this.name + ", Born in " + born.getMonth() + "/" + born.getDate() + "/" + born.getYear();
        if(this.died != null)
            result += ", died in " + died.getMonth() + "/" + died.getDate() + "/" + died.getYear();
        return result;
    }
    // 존재 가능한 날짜인가?
    private static boolean consistent(Date birth, Date death)
    {
        if(birth == null) return false;
        else if (death == null) return true;
        else return (birth.compareTo(death)<=0);
    }

    public String getName() //2번째 exercise - static mehtod는 오버로딩이 안됨.
    {return this.name;}

    public Date getBorn()
    {return new Date(born.getYear(), born.getMonth(), born.getDate());}

    public Date getDied()
    {
        if(died == null) return null;
        return new Date(died.getYear(),died.getMonth(),died.getDate());
    }

    public void setName(String _name){this.name = _name;}

    public void setBorn(Date _born) {
        if(_born == null) {
            System.out.println("Invalid date");
            return ;
        }
        this.born = new Date(_born.getYear(), _born.getMonth(), _born.getDate());
    }

    public void setDied(Date _died) {
        if(_died == null)
            this.died = null;
        this.died = new Date(_died.getYear(), _died.getMonth(), _died.getDate());
    }

    public boolean equals(Person other) {
        if(other == null)
            return false;
        else
            return (this.name.equals(other.name)&&
                    this.born.equals(other.born)&&
                    datesMatch(this.died, other.died));
                    // 1번 죽고 2번 살면 false. 2번도 죽으면 true
    }

    private static boolean datesMatch(Date date1, Date date2) {
        if(date1 == null) return (date2 == null);
        else if(date2 == null) return false;
        else return (date1.equals(date2));  // both dates are not null.
    }
}
