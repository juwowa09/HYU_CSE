// Main.java
import java.io.*;

public class Main {
    public static void main(String[] args) {
        // 테스트 파일 경로
        File file = new File("test.txt");

        try {
            // 1. EncryptWriter로 데이터 암호화 후 파일에 쓰기
            Writer writer = new EncryptWriter(new FileWriter(file));
            String input = "";
            for(String arg : args){
                input += arg+" ";
            }
            writer.write(input);
            writer.close();

            // 2. DecryptReader로 파일에서 읽고 복호화
            Reader reader = new DecryptReader(new FileReader(file));
            char[] buffer = new char[1024];
            int len = reader.read(buffer);
            System.out.println("Decrypted content: " + new String(buffer, 0, len));
            reader.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}