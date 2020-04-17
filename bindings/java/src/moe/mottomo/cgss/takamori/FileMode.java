package moe.mottomo.cgss.takamori;

public enum FileMode {

    NONE(0),
    APPEND(1),
    CREATE(2),
    CREATE_NEW(3),
    OPEN_EXISTING(4),
    OPEN_OR_CREATE(5);

    private final int n;

    FileMode(int n) {
        this.n = n;
    }

    public int value() {
        return n;
    }

}
