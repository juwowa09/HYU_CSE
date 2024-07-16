public class FormatException extends Exception{
    public FormatException(){super("Input Format is incorrect!!");};

    @Override
    public String getMessage(){
        return "[FormatException] " + super.getMessage();
    }
}
