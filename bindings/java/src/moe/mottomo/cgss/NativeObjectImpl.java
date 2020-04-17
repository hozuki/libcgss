package moe.mottomo.cgss;

abstract class NativeObjectImpl implements NativeObject {

    private boolean isDisposed;
    private long nativePtr;

    protected NativeObjectImpl() {
        nativePtr = 0;
        isDisposed = false;
    }

    public void markDisposed() {
        isDisposed = true;
    }

    public abstract void dispose();

    public boolean isDisposed() {
        return this.isDisposed;
    }

    public long getNativePtr() {
        return nativePtr;
    }

    public void setNativePtr(long ptr) {
        nativePtr = ptr;
    }

}
