package Week6.Exercise1.util;
import java.util.Calendar;
import Week6.Exercise1.*;
import java.util.Date;

public class AgeCalculator {
    public static int calAge(Person P) {
        Date pborn = P.getBorn();
        Date pdied;
        Calendar cal = Calendar.getInstance();
        if(P.getDied() != null)
        {
            pdied = P.getDied();
            int age = pdied.getYear() - pborn.getYear();
            if( pborn.getMonth() < pdied.getMonth()) {return age;}
            else if ( pdied.getMonth() == pborn.getMonth())
            {
                if(pborn.getDay() <= pdied.getDay()) {return age;}
                return age-1;
            }
            else return age-1;
        }
        else{
            pdied = P.getDied();
            int age = cal.get(Calendar.YEAR) - pborn.getYear();
            if( pborn.getMonth() > cal.get(Calendar.MONTH) + 1)
            {return age-1;}
            else if ( pborn.getMonth() == cal.get(Calendar.MONTH)+1)
            {
                if(pborn.getDay() <= cal.get(Calendar.DAY_OF_MONTH)) {return age;}
                return age-1;
            }
            else return age;
        }
    }
    public static int isOlder(Person p1, Person p2) {
        if (calAge(p1) > calAge(p2)) return 1;
        else if (calAge(p1) == calAge(p2)) return 0;
        else return -1;
    }
}
