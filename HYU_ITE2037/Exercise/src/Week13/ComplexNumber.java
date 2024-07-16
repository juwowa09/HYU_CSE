package Week13;

public class ComplexNumber {
    protected RealNumber real;
    protected RealNumber imaginary;

    public ComplexNumber(RealNumber real, RealNumber imaginary)
    {
        this.real = real;
        this.imaginary = imaginary;
    }
    public static class RealNumber
    {
        protected int divid;
        protected int divis;

        public RealNumber(int divid, int divis)
        {
            this.divid = divid;
            this.divis = divis;
        }

        public String toString(){
            return String.valueOf(divid) + "/" + String.valueOf(divis);
        }
    }
}
