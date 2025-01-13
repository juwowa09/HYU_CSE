package Week9;
import Week6.Exercise1.Person;
import java.util.Date;

public class Patient extends Person{
    // store department of patient object
    private String department;

    public Patient(String name,Date born,Date died, String department) {
        super(name, born, died);
        this.department = department;
    }

    public String getDepartment()
    {return this.department;}

    public void setDepartment(String department)
    {this.department = department;}

    @Override
    public String toString()
    {return "[Patient]"+super.toString()+ ", Department : "+ this.department;}
}
