package moe.mottomo.cgss;

/**
 * Created by MIC on 2017-01-28.
 */
public abstract class NativeObject {

    protected NativeObject() {
        nativePtr = 0;
    }

    protected long nativePtr;

}
