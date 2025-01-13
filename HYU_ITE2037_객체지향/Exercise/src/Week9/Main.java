package Week9;
import java.util.Date;

public class Main {
    public static void main(String [] argv)
    {
        Date doctorBorn1 = new Date("1990/02/25");
        Date doctorBorn2 = new Date("1984/03/22");
        Date doctorBorn3 = new Date("1995/07/15");
        Date patient1Born = new Date("2000/03/07");
        Date patient2Born = new Date("2002/05/07");
        Doctor doctor1 = new Physician("Peter", doctorBorn1, null, "Asan");
        Doctor doctor2 = new Physician("Tom", doctorBorn2, null, "Busan");
        Doctor doctor3 = new Doctor("Jenny", doctorBorn3, null, "Seoul");
        System.out.println(doctor3); //System.out.println()은 .toString()을 자동으로 호출해줌
        System.out.println(doctor1);
        System.out.println(doctor2);

        if(doctor1.equals(doctor2)){ //두 의사가 같은 사람인지 확인
            System.out.println("Two doctors are same person");
        }

        Patient patient1 = new Patient("John", patient1Born, null, "internal");
        Patient patient2 = new Patient("Brandon", patient2Born, null, "dental");

        System.out.println(patient1);
        System.out.println(patient2);

        doctor1.examination(patient1);
        doctor2.examination(patient2);
        doctor3.examination(patient1);
    }
}
