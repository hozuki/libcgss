package moe.mottomo.cgss;

public final class LibraryLoader {

    private static final String BASE_NAME = "cgss_jni";
    public static final String VERSION = "0.2.0";

    static {
        initialize();
    }

    public static void initialize() {
        System.loadLibrary(BASE_NAME + "-" + VERSION);
    }

}
