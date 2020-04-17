package moe.mottomo.cgss.kawashima;

import moe.mottomo.cgss.takamori.NativeStream;

public class HcaCipherConverter extends HcaFormatReader {

    public HcaCipherConverter(NativeStream baseStream, final HcaCipherConfig from, final HcaCipherConfig to) {
        super(baseStream);
        cipherFrom = from.clone();
        cipherTo = to.clone();

        init(baseStream, from, to);
    }

    @Override
    public native int read(byte[] buffer, int offset, int count);

    @Override
    public native long getPosition();

    @Override
    public native void setPosition(long position);

    @Override
    public native long getLength();

    public final HcaCipherConfig getCipherFrom() {
        return cipherFrom.clone();
    }

    public final HcaCipherConfig getCipherTo() {
        return cipherTo.clone();
    }

    @Override
    public native void dispose();

    private native void init(NativeStream baseStream, final HcaCipherConfig from, final HcaCipherConfig to);

    private final HcaCipherConfig cipherFrom, cipherTo;

}
