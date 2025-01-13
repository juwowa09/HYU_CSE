public abstract class ContactInfo {
    private String name;
    private String number;

    public ContactInfo(){}
    public ContactInfo(String name, String number){
        this.name = name;
        this.number = number;
    }
    public void setName(String n){name = n;}
    public void setNumber(String n){number = n;}
    public String getName(){return this.name;}
    public String getNumber(){return this.number;}

    public abstract String getInfo();

    @Override
    public abstract String toString();
}
