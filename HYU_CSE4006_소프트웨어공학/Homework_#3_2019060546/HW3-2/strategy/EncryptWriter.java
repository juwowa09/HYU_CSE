// EncryptWriter.java
import java.io.*;

public class EncryptWriter extends Writer {
    private Writer writer;
    private EncryptionStrategy strategy;

    public EncryptWriter(Writer writer, EncryptionStrategy strategy) {
        this.writer = writer;
        this.strategy = strategy;
    }

    @Override
    public void write(char[] cbuf, int off, int len) throws IOException {
        String data = new String(cbuf, off, len);
        String encryptedData = strategy.encrypt(data); // 암호화
        writer.write(encryptedData);
    }

    @Override
    public void flush() throws IOException {
        writer.flush();
    }

    @Override
    public void close() throws IOException {
        writer.close();
    }
}