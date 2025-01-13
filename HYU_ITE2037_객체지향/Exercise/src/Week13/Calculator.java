package Week13;

public class Calculator implements ComplexNumberCalculator, RealNumberCalculator{
    public static void main(String[] args)
    {
        Calculator calc = new Calculator();
        ComplexNumber c1 = new ComplexNumber(new ComplexNumber.RealNumber(4, 10), new ComplexNumber.RealNumber(3, 2));
        ComplexNumber c2 = new ComplexNumber(new ComplexNumber.RealNumber(3, 10), new ComplexNumber.RealNumber(-4, 2));

        calc.print(calc.mul(c1, c2));
    }

    public ComplexNumber add(ComplexNumber c1, ComplexNumber c2)
    {
        ComplexNumber.RealNumber realPart = add(c1.real, c2.real);
        ComplexNumber.RealNumber imaginaryPart = add(c1.imaginary, c2.imaginary);

        return new ComplexNumber(realPart, imaginaryPart);
    }

    public ComplexNumber sub(ComplexNumber c1, ComplexNumber c2)
    {
        ComplexNumber.RealNumber realPart = sub(c1.real, c2.real);
        ComplexNumber.RealNumber imaginaryPart = sub(c1.imaginary, c2.imaginary);

        return new ComplexNumber(realPart, imaginaryPart);
    }

    public ComplexNumber mul(ComplexNumber c1, ComplexNumber c2)
    {
        ComplexNumber.RealNumber realPart = mul(c1.real, c2.real);
        ComplexNumber.RealNumber imaginaryPart = mul(c1.imaginary, c2.imaginary);

        return new ComplexNumber(realPart, imaginaryPart);
    }

    public ComplexNumber.RealNumber add(ComplexNumber.RealNumber real1, ComplexNumber.RealNumber real2)
    {
        int nDivid = real1.divid * real2.divis + real1.divis * real2.divid;
        int nDivis = real1.divis * real2.divis;

        return new ComplexNumber.RealNumber(nDivid, nDivis);
    }

    public ComplexNumber.RealNumber sub(ComplexNumber.RealNumber real1, ComplexNumber.RealNumber real2)
    {
        int nDivid = real1.divid * real2.divis - real1.divis * real2.divid;
        int nDivis = real1.divis * real2.divis;

        return new ComplexNumber.RealNumber(nDivid, nDivis);
    }

    public ComplexNumber.RealNumber mul(ComplexNumber.RealNumber real1, ComplexNumber.RealNumber real2)
    {
        int nDivid = real1.divid * real2.divid;
        int nDivis = real1.divis * real2.divis;

        return new ComplexNumber.RealNumber(nDivid, nDivis);
    }
}
