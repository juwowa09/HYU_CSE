import java.io.*;

public class DecryptReader extends Reader {
    private Reader reader; // 감싸는 Reader 객체

    public DecryptReader(Reader reader) {
        this.reader = reader;
    }

    @Override
    public int read(char[] cbuf, int off, int len) throws IOException {
        int numChars = reader.read(cbuf, off, len);
        if (numChars == -1) return -1; // EOF 처리

        // 암호화된 데이터를 복호화
        String data = new String(cbuf, off, numChars);
        String decryptedData = CaesarCipher.decrypt(data);

        char[] decryptedChars = decryptedData.toCharArray();
        System.arraycopy(decryptedChars, 0, cbuf, off, decryptedChars.length);
        return decryptedChars.length;
    }

    @Override
    public void close() throws IOException {
        reader.close();
    }
}