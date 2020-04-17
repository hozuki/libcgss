package moe.mottomo.cgss;

public final class JniLibraryLoader {

    private static final String BASE_NAME = "cgss_jni";

    public static void initialize() {
        System.loadLibrary(BASE_NAME);
    }

}
