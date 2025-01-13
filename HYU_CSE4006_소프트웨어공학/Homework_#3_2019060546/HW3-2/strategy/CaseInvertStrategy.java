// CaseInvertStrategy.java
public class CaseInvertStrategy implements EncryptionStrategy {
    @Override
    public String encrypt(String data) {
        return invertCase(data);
    }

    @Override
    public String decrypt(String data) {
        return invertCase(data); // 대칭 알고리즘
    }

    private String invertCase(String data) {
        StringBuilder inverted = new StringBuilder();
        for (char c : data.toCharArray()) {
            if (Character.isLowerCase(c)) {
                inverted.append(Character.toUpperCase(c));
            } else if (Character.isUpperCase(c)) {
                inverted.append(Character.toLowerCase(c));
            } else {
                inverted.append(c);
            }
        }
        return inverted.toString();
    }
}