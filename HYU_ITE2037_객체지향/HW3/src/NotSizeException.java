public class NotSizeException extends Exception{
    public NotSizeException(){super("Contacts Size is not determined!!");};

    @Override
    public String getMessage(){
        return "[NotSizeException] " + super.getMessage();
    }
}
