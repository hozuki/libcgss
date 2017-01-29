package moe.mottomo.cgss.hca;

import moe.mottomo.cgss.streams.Stream;

public final class HcaDecoder extends HcaFormatReader {

    public HcaDecoder(Stream baseStream) {
        this(baseStream, new HcaDecoderConfig());
    }

    public HcaDecoder(Stream baseStream, final HcaDecoderConfig config) {
        super(baseStream);
        this.config = config.clone();
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

    public final HcaDecoderConfig getConfig() {
        return config.clone();
    }

    @Override
    protected native void dispose();

    private final HcaDecoderConfig config;

    private native void init(Stream baseStream, final HcaDecoderConfig config);

}
