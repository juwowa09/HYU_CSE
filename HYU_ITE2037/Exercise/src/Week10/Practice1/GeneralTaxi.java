package Week10.Practice1;

public class GeneralTaxi extends Taxi{
    private double farePerKilometer;
    private double baseDistance;
    private double baseFee;

    public GeneralTaxi(){super();}
    public GeneralTaxi(int carNum, double farePerKilometer) {
        super(carNum);
        if (carNumVaild(carNum)) {
            baseDistance = 3.0;
            baseFee = 3.0;
            if(farePerKilometerVaild(farePerKilometer, (baseFee/baseDistance)))
                this.farePerKilometer = farePerKilometer;
        }
    }

    public double getBaseFee(){return baseFee;}
    public double getBaseDistance(){return baseDistance;}
    public double getFarePerKilometer(){return farePerKilometer;}

    @Override
    public String toString() {
        return "This General Taxi has CarNumber " + getCarNum() + ", has fare per kilometer " + getFarePerKilometer()
                +", Total Income is "+getIncome();
    }

    @Override
    public double getPaid(double dis) {
        setDistance(dis);
        if(getDistance() <= getBaseDistance()) return getBaseFee();
        else {
            return getBaseFee() + (getDistance() - getBaseDistance()) * getFarePerKilometer();
        }
    }

    private boolean farePerKilometerVaild(double fare, double base) {
        if(fare > base) return true;
        else return false;
    }
}
