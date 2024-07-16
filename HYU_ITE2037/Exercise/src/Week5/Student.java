package Week5;
import java.util.Calendar;
import java.util.Random;

public class Student{
    Student(){}
    Student(String name, int year, int month, int day)
    {
        this.name = name;
        this.year = year;
        this.month = month;
        this.day = day;
    }
    Student(String name, int year)
    {
        Random R = new Random();
        int m = R.nextInt(12) + 1;
        int ed = R.nextInt(30)+1;
        int od = R.nextInt(31)+1;
        this.month = m;
        this.name = name;
        this.year = year;

        if(m % 2 == 1 && month<=7 ) {day = od;}
        else if(m%2 == 0 && month<=7)
        {
            if(m == 2)
            {
                ed = R.nextInt(28) + 1;
                day = ed;
            }
            else day = ed;
        }
        else if (m%2 == 0 && month>7) {day = od;}
        else day = ed;
    }

    private String name = null;
    private int year,month,day;
    public String getName() {return this.name;}
    public int getYear() {return this.year;}
    public int getMonth() {return this.month;}
    public int getDay() {return this.day;}
    public boolean checkDate(){
        if(this.month>12){
            return false;
        }
        else
        {
            if(this.month <= 7 && this.month > 0)
            {
                if(this.month == 2)
                {
                    if(this.day > 28 || this.day<=0) {return false;}
                    else return true;
                }
                else if (this.month % 2==1)
                {
                    if(this.day >31 || this.day<=0){return false;}
                    else return true;
                }
                else{
                    if(this.day >30 || this.day<=0){return false;}
                    else return true;
                }
            }
            else if (this.month <= 0) return false;
            else {
                if (this.month % 2==1)
                {
                    if(this.day >30|| this.day<=0){return false;}
                    else return true;
                }
                else{
                    if(this.day >31|| this.day<=0){return false;}
                    else return true;
                }
            }
        }
    }
    public int calAge(int year, int month, int day)
    {
        Calendar cal = Calendar.getInstance();
        int age = cal.get(Calendar.YEAR) - year;
        if( this.month < (cal.get(Calendar.MONTH)+1)) {return age;}
        else if (this.month == (cal.get(Calendar.MONTH)+1))
        {
            if(this.day <= cal.get(Calendar.DAY_OF_MONTH)) {return age;}
            return age-1;
        }
        else return age-1;
    }
    public boolean isOlder(Student stu){
        if(this.month < stu.getMonth()){return true;}
        else if (this.month == stu.getMonth())
        {
            if(this.day < stu.getDay()) return true;
            else return false;
        }
        else return false;
    }
    public void printInform()
    {
        System.out.println(this.getName()+ " "+this.getYear()+"/"+this.getMonth()+"/"+this.getDay()+" age: "+this.calAge(this.year,this.month,this.day));
    }
}

