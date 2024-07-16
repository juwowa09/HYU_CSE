package Week13;

public interface ComplexNumberCalculator {
    public ComplexNumber add(ComplexNumber com1, ComplexNumber com2);
    public ComplexNumber sub(ComplexNumber com1, ComplexNumber com2);
    public ComplexNumber mul(ComplexNumber com1, ComplexNumber com2);
    default void print(ComplexNumber com){
        System.out.println("Real : " + com.real + ", Imaginary : " + com.imaginary);
    }
}
