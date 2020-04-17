package moe.mottomo.cgss.takamori;

import moe.mottomo.cgss.JniLibraryLoader;
import moe.mottomo.cgss.NativeObject;

public abstract class NativeStream extends Stream implements NativeObject {

    static {
        JniLibraryLoader.initialize();
    }

    private boolean isDisposed;
    private long nativePtr;

    protected NativeStream() {
        nativePtr = 0;
    }

    public final long getNativePtr() {
        return nativePtr;
    }

    public final void setNativePtr(long ptr) {
        nativePtr = ptr;
    }

    public final boolean isDisposed() {
        return isDisposed;
    }

    public final void markDisposed() {
        isDisposed = true;
    }

    public final native int readByte();

    public final native int writeByte(byte b);

    @Override
    public final void copyTo(Stream destination, int bufferSize) {
        if (destination instanceof NativeStream) {
            copyTo((NativeStream)destination, bufferSize);
        }
    }

    public final native void copyTo(NativeStream destination, int bufferSize);

    @Override
    public void seek(long offset, StreamSeekOrigin origin) {
        seek(offset, origin.value());
    }

    public abstract void dispose();

    private native void seek(long offset, int origin);

}
