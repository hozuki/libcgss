package moe.mottomo.cgss.kawashima;

import moe.mottomo.cgss.takamori.NativeStream;

public final class HcaDecoder extends HcaFormatReader {

    private final HcaDecoderConfig config;

    public HcaDecoder(NativeStream baseStream) {
        this(baseStream, new HcaDecoderConfig());
    }

    public HcaDecoder(NativeStream baseStream, final HcaDecoderConfig config) {
        super(baseStream);
        this.config = config;
        init(baseStream, config);
    }

    @Override
    public native int read(byte[] buffer, int offset, int count);

    @Override
    public native long getPosition();

    @Override
    public native void setPosition(long position);

    @Override
    public native long getLength();

    public HcaDecoderConfig getConfig() {
        return config.clone();
    }

    @Override
    public native void dispose();

    private native void init(NativeStream baseStream, final HcaDecoderConfig config);

}
