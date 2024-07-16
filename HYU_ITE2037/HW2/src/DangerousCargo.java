public class DangerousCargo extends HeavyCargo{
    public DangerousCargo(int ID, int weight)
    {super();
    this.ID = ID;
    this.weight = weight;}

    @Override
    public int getID()
    {return ID;}

    @Override
    public double getWeight()
    {return weight;}

    @Override
    public double consumption()
    {return getWeight() * 4.0;}
}
