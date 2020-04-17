package moe.mottomo.cgss;

public interface NativeObject extends AutoCloseable {

    boolean isDisposed();

    void markDisposed();

    /**
     * FOR INTERNAL USE. DO NOT CALL.
     *
     * @return Native pointer
     */
    long getNativePtr();

    /**
     * FOR INTERNAL USE. DO NOT CALL.
     *
     * @param ptr Native pointer
     */
    void setNativePtr(long ptr);

    void dispose();

    default void close() {
        if (isDisposed()) {
            return;
        }

        dispose();
        markDisposed();
    }

}
