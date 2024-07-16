package Week10.Practice1;

public abstract class Taxi {
    private int carNum;
    private double distance;
    private double income;

    // constructor
    public Taxi(){}
    public Taxi(int carNum)
    {
        if(setCarNum(carNum)) {
            distance = 0;
        }
        else System.out.println("This CarNumber can't exist");
    }

    public boolean setCarNum(int carNum){
        if(carNumVaild(carNum))
        {
            this.carNum = carNum;
            return true;
        }
        return false;
    }
    public int getCarNum()
    { return carNum; }

    public void setDistance(double distance) {this.distance += distance;}
    public double getDistance()
    {return distance;}

    public double getIncome(){ return income; }
    public void setIncome(double income){ this.income = income;}

    public String toString()
    {return "This Taxi has CarNumber " + getCarNum()+ " and Total Income is "+getIncome();}

    public abstract double getPaid(double distance);

    // update total income by call getPaid method
    public void doDrive(double dis) {
        setIncome(getPaid(dis));
    }

    public boolean earnMore(Taxi t) {
        if (distance > t.distance)
        { return true; }
        else return false;
    }

    protected boolean carNumVaild(int carNum) {
        if(carNum >= 1000 & carNum < 10000)return true;
        return false;
    }
}
