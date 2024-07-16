package Week11.Practice2;

public class MyException extends Exception{
    public MyException(){super("");}
    public MyException(String m){super(m);}

    @Override
    public String getMessage(){
        return "MyException : " + super.getMessage();
    }
}
