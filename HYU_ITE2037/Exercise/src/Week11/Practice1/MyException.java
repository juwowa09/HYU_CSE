package Week11.Practice1;

public class MyException extends Exception {
    public MyException(){
        super("My exception happens!");
    }
    public MyException(String message){
        super(message);
    }

    @Override
    public String getMessage() {
        return "[MyException] " + super.getMessage();
    }
}
