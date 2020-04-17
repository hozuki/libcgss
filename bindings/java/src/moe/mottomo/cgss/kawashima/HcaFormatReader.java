package moe.mottomo.cgss.kawashima;

import moe.mottomo.cgss.takamori.NativeStream;

public abstract class HcaFormatReader extends NativeStream {

    protected HcaFormatReader(NativeStream baseStream) {
    }

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

    public static native boolean isPossibleHcaStream(NativeStream stream);

}
