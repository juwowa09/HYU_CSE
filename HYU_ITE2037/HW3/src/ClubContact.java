public class ClubContact extends ContactInfo{
    private String clubName;

    public ClubContact(){}
    public ClubContact(String name, String number, String clubName){
        super(name, number);
        this.clubName = clubName;
    }

    public String getClubName(){return this.clubName;}
    public void setClubName(String c){clubName = c;}
    @Override
    public String getInfo()
    {return "name: " + getName() + " / phone number: "+ getNumber()+" / club name: "+getClubName();}

    @Override
    public String toString()
    {return ". "+ getInfo();}
}
