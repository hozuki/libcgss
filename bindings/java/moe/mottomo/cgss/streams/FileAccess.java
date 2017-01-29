package moe.mottomo.cgss.streams;

/**
 * Created by MIC on 2017-01-28.
 */
public enum FileAccess {

    NONE(0), READ(1), WRITE(2), READ_WRITE(3);

    private int n;

    FileAccess(int n) {
        this.n = n;
    }

    public int value() {
        return n;
    }

}
