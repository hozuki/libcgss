package moe.mottomo.cgss.hca;

import moe.mottomo.cgss.streams.Stream;

public abstract class HcaFormatReader extends Stream {

    @Override
    public final int write(byte[] buffer, int offset, int count) {
        throw new UnsupportedOperationException("HcaFormatReader.write");
    }

    @Override
    public native boolean isWritable();

    @Override
    public native boolean isReadable();

    @Override
    public native boolean isSeekable();

    @Override
    public final void setLength(long length) {
        throw new UnsupportedOperationException("HcaFormatReader.setLength");
    }

    @Override
    public final void flush() {
        throw new UnsupportedOperationException("HcaFormatReader.flush");
    }

    public final native HcaInfo getHcaInfo();

    HcaFormatReader(Stream baseStream) {
    }

    protected Stream baseStream;

}
