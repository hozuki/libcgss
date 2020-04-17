package moe.mottomo.cgss.takamori;

public enum StreamSeekOrigin {

    Begin(0),
    Current(1),
    End(2);

    private final int n;

    StreamSeekOrigin(int n) {
        this.n = n;
    }

    public int value() {
        return n;
    }

}
