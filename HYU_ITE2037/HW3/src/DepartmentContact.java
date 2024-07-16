public class DepartmentContact extends ContactInfo{
    private String department;

    public DepartmentContact(){}
    public DepartmentContact(String name, String number, String department){
        super(name,number);
        this.department = department;
    }
    public void setDepartment(String d){department = d;}
    public String getDepartment(){return this.department;}

    @Override
    public String getInfo()
    {return "name: " + getName() + " / phone number: "+ getNumber()+" / department: "+getDepartment();}

    @Override
    public String toString()
    {return ". "+getInfo();}
}
