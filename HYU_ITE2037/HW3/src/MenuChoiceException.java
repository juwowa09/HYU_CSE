public class MenuChoiceException extends Exception {
    private int e;
    public MenuChoiceException(){super("Exception!");};
    public MenuChoiceException(int e){
        super("Out of MenuChoice Range!!");
        this.e = e;
    }

    @Override
    public String getMessage(){
        return "[MenuChoiceException]" + e+ " is " + super.getMessage();
    }
}
