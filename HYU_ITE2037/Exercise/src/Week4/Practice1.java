package Week4;
import java.util.Scanner;
public class Practice1 {

    public static void main(String[] argv) {
        Scanner input = new Scanner(System.in);
        System.out.println("Input string S:");
        String str = input.nextLine();
        System.out.println("Longerst Palindrome: "+LongestPalindrome(str));
    }
    public static String LongestPalindrome(String str) {
        // palindrome으로 사용할 string 생성.
        String longestPalindrome = "";

        for (int i = 0; i < str.length(); i++) {
            // 모든 substring의 palindrome 검사하기 위함
            for (int j = i + 1; j <= str.length(); j++) {
                String substr = str.substring(i, j);
                if (isPalindrome(substr) && substr.length() > longestPalindrome.length()) {
                    longestPalindrome = substr;
                }
            }
        }
        return longestPalindrome;
    }

    public static boolean isPalindrome(String str) {
        for(int i=0;i<str.length()/2;i++)
        {
            if(str.charAt(i) != str.charAt(str.length()-1-i))
                return false;
        }
        return true;
    }
}
