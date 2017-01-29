package moe.mottomo.cgss.streams;

import moe.mottomo.cgss.NativeObject;

import java.io.Closeable;

public abstract class Stream extends NativeObject implements Closeable {

    public abstract int read(byte[] buffer, int offset, int count);

    public abstract int write(byte[] buffer, int offset, int count);

    public final void seek(long offset, StreamSeekOrigin origin) {
        seek(offset, origin.value());
    }

    public abstract boolean isWritable();

    public abstract boolean isReadable();

    public abstract boolean isSeekable();

    public abstract long getPosition();

    public abstract void setPosition(long position);

    public abstract long getLength();

    public abstract void setLength(long length);

    public final native byte readByte();

    public final native int writeByte(byte b);

    public abstract void flush();

    public final void copyTo(Stream destination) {
        copyTo(destination, 10240);
    }

    public final native void copyTo(Stream destination, int bufferSize);

    @Override
    public final void close() {
        dispose();
    }

    protected Stream() {
    }

    private native void seek(long offset, int origin);

    protected abstract void dispose();

}
