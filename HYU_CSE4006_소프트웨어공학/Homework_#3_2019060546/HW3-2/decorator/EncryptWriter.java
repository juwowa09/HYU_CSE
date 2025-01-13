import java.io.*;

public class EncryptWriter extends Writer {
    private Writer writer; // Writer 객체

    public EncryptWriter(Writer writer) {
        this.writer = writer;
    }

    @Override
    public void write(char[] cbuf, int off, int len) throws IOException {
        // 입력 데이터를 암호화
        String data = new String(cbuf, off, len);
        String encryptedData = CaesarCipher.encrypt(data);
        // 암호화된 데이터를 원래 Writer로 전달
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