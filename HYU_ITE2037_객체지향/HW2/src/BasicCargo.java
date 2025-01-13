public class BasicCargo extends Cargo{
    public BasicCargo(){}
    public BasicCargo(int ID, int weight){
        super();
        if(weight <= 1000 && weight > 0){
            this.ID = ID;
            this.weight = weight;
        }
    }
    @Override
    public int getID()
    {return ID;}

    @Override
    public double getWeight()
    {return weight;}

    @Override
    public double consumption()
    {return getWeight() * 1.5;}

}
