public class NormalContact extends ContactInfo{
    private String relation;

    public NormalContact(){}
    public NormalContact(String name, String number, String relation){
        super(name, number);
        this.relation = relation;
    }

    public String getRelation(){return this.relation;}
    public void setRelation(String r){relation = r;}
    @Override
    public String getInfo()
    {return "name: " + getName() + " / phone number: "+ getNumber()+" / relation: "+getRelation();}

    @Override
    public String toString()
    {return ". "+getInfo();}
}
