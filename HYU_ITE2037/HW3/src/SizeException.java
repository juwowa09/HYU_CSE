public class SizeException extends Exception{
    public SizeException(){super("Contacts Size is illegal!!");};

    @Override
    public String getMessage(){
        return "[SizeException] " + super.getMessage();
    }
}
