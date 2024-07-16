package Week11.Practice2;

public class Account {
    private String ID;
    private int account_num;
    private int balance;

    public Account(){}
    public Account(String ID, String account_num, int balance)throws MyException{
        if(balance<1000) throw new MyException("balance less than $1000");
        if(account_num.length() != 5) throw new MyException("account number length error");
        if(Character.isDigit(ID.charAt(0))) throw new MyException("Start Character");
        if(!(Character.isDigit(ID.charAt(ID.length()-1)) && Character.isDigit(ID.charAt(ID.length()-2)) && Character.isDigit(ID.charAt(ID.length()-3)))) throw new MyException("ID finish three digits");
        this.ID = ID;
        this.account_num = Integer.parseInt(account_num);
        this.balance = balance;
    }

    public String toString()
    {return "User ID : " + this.ID + ", account number : "+this.account_num+", balance : "+this.balance;}
}
