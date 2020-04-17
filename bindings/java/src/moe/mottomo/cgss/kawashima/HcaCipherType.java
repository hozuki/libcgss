package moe.mottomo.cgss.kawashima;

public enum HcaCipherType {

    NO_CIPHER(0),
    STATIC(1),
    WITH_KEY(56);

    private final int n;

    HcaCipherType(int n) {
        this.n = n;
    }

    public int value() {
        return n;
    }

    public static HcaCipherType valueOf(int n) {
        switch (n) {
            case 0:
                return NO_CIPHER;
            case 1:
                return STATIC;
            case 56:
                return WITH_KEY;
            default:
                throw new IndexOutOfBoundsException();
        }
    }

}
