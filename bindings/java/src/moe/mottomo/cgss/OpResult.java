package moe.mottomo.cgss;

public enum OpResult {

    OK(0),
    GENERIC_FAULT(-1),
    BUFFER_TOO_SMALL(-2),
    NOT_IMPLEMENTED(-3),
    INVALID_OPERATION(-4),
    INVALID_ARGUMENT(-5),
    FORMAT_ERROR(-6),
    CHECKSUM_ERROR(-7),
    DECODE_FAILED(-8),
    INVALID_HANDLE(-9);

    private final int n;

    OpResult(int n) {
        this.n = n;
    }

    public int value() {
        return n;
    }

    public static OpResult valueOf(int n) {
        if (n > 0) {
            n = 0;
        }

        switch (n) {
            case 0:
                return OK;
            case -2:
                return BUFFER_TOO_SMALL;
            case -3:
                return NOT_IMPLEMENTED;
            case -4:
                return INVALID_OPERATION;
            case -5:
                return INVALID_ARGUMENT;
            case -6:
                return FORMAT_ERROR;
            case -7:
                return CHECKSUM_ERROR;
            case -8:
                return DECODE_FAILED;
            case -9:
                return INVALID_HANDLE;
            default:
                return GENERIC_FAULT;
        }
    }

}
