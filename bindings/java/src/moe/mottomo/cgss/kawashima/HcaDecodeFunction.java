package moe.mottomo.cgss.kawashima;

public enum HcaDecodeFunction {

    NULL(0),
    DECODE_8BIT_U(1),
    DECODE_16BIT_S(2),
    DECODE_24BIT_S(3),
    DECODE_32BIT_S(4),
    DECODE_32BIT_F(5);

    private final int n;

    HcaDecodeFunction(int n) {
        this.n = n;
    }

    public int value() {
        return n;
    }

    public static HcaDecodeFunction valueOf(int n) {
        switch (n) {
            case 1:
                return DECODE_8BIT_U;
            case 2:
                return DECODE_16BIT_S;
            case 3:
                return DECODE_24BIT_S;
            case 4:
                return DECODE_32BIT_S;
            case 5:
                return DECODE_32BIT_F;
            default:
                return NULL;
        }
    }

}
