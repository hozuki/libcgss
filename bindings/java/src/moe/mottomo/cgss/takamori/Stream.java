package moe.mottomo.cgss.takamori;

public abstract class Stream {

    Stream() {
    }

    public abstract int read(byte[] buffer, int offset, int count);

    public abstract int write(byte[] buffer, int offset, int count);

    public abstract void seek(long offset, StreamSeekOrigin origin);

    public abstract boolean isWritable();

    public abstract boolean isReadable();

    public abstract boolean isSeekable();

    public abstract long getPosition();

    public abstract void setPosition(long position);

    public abstract long getLength();

    public abstract void setLength(long length);

    public abstract void flush();

    public final void copyTo(Stream destination) {
        copyTo(destination, 10240);
    }

    public abstract void copyTo(Stream destination, int bufferSize);

}
