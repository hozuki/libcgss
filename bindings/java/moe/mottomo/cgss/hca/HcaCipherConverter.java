package moe.mottomo.cgss.hca;

import moe.mottomo.cgss.streams.Stream;

/**
 * Created by MIC on 2017-01-28.
 */
public class HcaCipherConverter extends HcaFormatReader {

    public HcaCipherConverter(Stream baseStream, final HcaCipherConfig from, final HcaCipherConfig to) {
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
    protected native void dispose();

    private native void init(Stream baseStream, final HcaCipherConfig from, final HcaCipherConfig to);

    private final HcaCipherConfig cipherFrom, cipherTo;

}
