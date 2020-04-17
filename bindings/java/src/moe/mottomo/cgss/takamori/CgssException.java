package moe.mottomo.cgss.takamori;

import moe.mottomo.cgss.OpResult;

public class CgssException extends RuntimeException {

    private final OpResult opResult;

    public CgssException(String message, int opResult) {
        this(message, OpResult.valueOf(opResult));
    }

    public CgssException(String message) {
        this(message, OpResult.GENERIC_FAULT);
    }

    public CgssException(String message, OpResult opResult) {
        super(message);
        this.opResult = opResult;
    }

    public OpResult getOpResult() {
        return opResult;
    }

}
