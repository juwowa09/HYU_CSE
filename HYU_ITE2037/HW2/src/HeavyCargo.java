public class HeavyCargo extends Cargo{
    public HeavyCargo(){}
    public HeavyCargo(int ID, int weight) {
        super();
        if(weight > 1000){
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
    {return getWeight() * 2.5;}
}
