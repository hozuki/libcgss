package moe.mottomo.cgss.takamori;

public class FileStream extends NativeStream {

    public FileStream(String fileName) {
        this(fileName, FileMode.OPEN_OR_CREATE);
    }

    public FileStream(String fileName, FileMode mode) {
        this(fileName, mode, FileAccess.READ_WRITE);
    }

    public FileStream(String fileName, FileMode mode, FileAccess access) {
        init(fileName, mode.value(), access.value());
    }

    private native void init(String fileName, int mode, int access);

    @Override
    public native int read(byte[] buffer, int offset, int count);

    @Override
    public native int write(byte[] buffer, int offset, int count);

    @Override
    public native boolean isWritable();

    @Override
    public native boolean isReadable();

    @Override
    public native boolean isSeekable();

    @Override
    public native long getPosition();

    @Override
    public native void setPosition(long position);

    @Override
    public native long getLength();

    @Override
    public native void setLength(long length);

    @Override
    public native void flush();

    @Override
    public native void dispose();

}
