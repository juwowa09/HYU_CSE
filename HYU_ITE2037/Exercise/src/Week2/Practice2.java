package Week2;

public class Practice2 {
    public static void main(String[] argv){
        String input = "Walt Savitch Asdfasdf";
        String output = null;

        String[] s = input.split(" ");
        for(String var : s)
        {
            output += var.substring(1,2).toUpperCase();
            output +=var.substring(2);
            output += var.substring(0,1).toLowerCase() + "ay ";
        }
        output = output.substring(4);
        System.out.println(output);
    }
}
