package Week9;
import java.util.Date;
import Week6.Exercise1.Person;

public class Doctor extends Person{
    private String hospital;

    public Doctor(String name, Date born, Date died, String hospital) {
        super(name, born, died);
        this.hospital = hospital;
    }

    public String getHospital()
    {return this.hospital;}

    public void setHospital(String hospital)
    {this.hospital = hospital;}

    @Override
    public String toString()
    {return "[Doctro]" + super.toString() + ", Affiliation : " + this.hospital;}

    public boolean equals(Doctor d)
    {return super.equals(d) && this.hospital.equals(d.hospital);}

    public void examination(Patient p)
    {System.out.println("I haven\'t decided my major yet.");}
}
