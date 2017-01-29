package moe.mottomo.cgss.wave;

public final class DefaultWaveGenerator {

    public static native int decode8BitU(float data, byte[] buffer, int cursor);

    public static native int decode16BitS(float data, byte[] buffer, int cursor);

    public static native int decode24BitS(float data, byte[] buffer, int cursor);

    public static native int decode32BitS(float data, byte[] buffer, int cursor);

    public static native int decodeFloat(float data, byte[] buffer, int cursor);

    private DefaultWaveGenerator() {
    }

}
