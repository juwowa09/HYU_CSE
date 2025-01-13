import java.io.*;

public class Main {
    public static void main(String[] args) {
        File file = new File("test.txt");

        try {
            // 1. Caesar Cipher 테스트
            System.out.println("Testing Caesar Cipher:");
            EncryptionStrategy caesarCipher = new CaesarCipherStrategy();
            Writer writer = new EncryptWriter(new FileWriter(file), caesarCipher);
            String input = "";
            for(String str : args){
                input += str + " ";
            }
            writer.write(input);
            writer.close();

            // 암호화된 데이터 출력
            BufferedReader encryptedReader = new BufferedReader(new FileReader(file));
            String encryptedContent = encryptedReader.readLine();
            System.out.println("Encrypted (Caesar): " + encryptedContent);
            encryptedReader.close();

            Reader reader = new DecryptReader(new FileReader(file), caesarCipher);
            char[] buffer = new char[1024];
            int len = reader.read(buffer);
            System.out.println("Decrypted (Caesar): " + new String(buffer, 0, len));
            reader.close();

            // 2. Case Invert 테스트
            System.out.println("\nTesting Case Invert:");
            EncryptionStrategy caseInvert = new CaseInvertStrategy();
            writer = new EncryptWriter(new FileWriter(file), caseInvert);
            writer.write(input);
            writer.close();

            // 암호화된 데이터 출력
            encryptedReader = new BufferedReader(new FileReader(file));
            encryptedContent = encryptedReader.readLine();
            System.out.println("Encrypted (Case Invert): " + encryptedContent);
            encryptedReader.close();

            reader = new DecryptReader(new FileReader(file), caseInvert);
            len = reader.read(buffer);
            System.out.println("Decrypted (Case Invert): " + new String(buffer, 0, len));
            reader.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}