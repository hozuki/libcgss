package moe.mottomo.cgss.takamori;

public enum FileAccess {

    NONE(0),
    READ(1),
    WRITE(2),
    READ_WRITE(3);

    private final int n;

    FileAccess(int n) {
        this.n = n;
    }

    public int value() {
        return n;
    }

}
