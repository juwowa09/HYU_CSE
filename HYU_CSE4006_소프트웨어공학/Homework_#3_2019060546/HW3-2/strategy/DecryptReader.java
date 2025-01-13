// DecryptReader.java
import java.io.*;

public class DecryptReader extends Reader {
    private Reader reader;
    private EncryptionStrategy strategy;

    public DecryptReader(Reader reader, EncryptionStrategy strategy) {
        this.reader = reader;
        this.strategy = strategy;
    }

    @Override
    public int read(char[] cbuf, int off, int len) throws IOException {
        int numChars = reader.read(cbuf, off, len);
        if (numChars == -1) return -1;

        String data = new String(cbuf, off, numChars);
        String decryptedData = strategy.decrypt(data); // 복호화

        char[] decryptedChars = decryptedData.toCharArray();
        System.arraycopy(decryptedChars, 0, cbuf, off, decryptedChars.length);
        return decryptedChars.length;
    }

    @Override
    public void close() throws IOException {
        reader.close();
    }
}