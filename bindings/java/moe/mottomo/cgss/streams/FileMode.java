package moe.mottomo.cgss.streams;

/**
 * Created by MIC on 2017-01-28.
 */
public enum FileMode {

    NONE(0), APPEND(1), CREATE(2), CREATE_NEW(3), OPEN_EXISTING(4), OPEN_OR_CREATE(5);

    private int n;

    FileMode(int n) {
        this.n = n;
    }

    public int value() {
        return n;
    }

}
