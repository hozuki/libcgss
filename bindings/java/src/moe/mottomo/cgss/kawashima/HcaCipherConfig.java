package moe.mottomo.cgss.kawashima;

public final class HcaCipherConfig implements Cloneable {

    private HcaCipherType cipherType;

    private long rawKey;
    private short keyModifier;

    public HcaCipherConfig() {
        this(HcaCipherType.NO_CIPHER);
    }

    public HcaCipherConfig(int key1, int key2, short mod) {
        if (key1 == 0 && key2 == 0) {
            cipherType = HcaCipherType.NO_CIPHER;
        } else {
            cipherType = HcaCipherType.WITH_KEY;
        }

        rawKey = makeRawKey(key1, key2);
        this.keyModifier = mod;
    }

    public HcaCipherConfig(long rawKey, short mod) {
        this((int)(rawKey >>> 32), (int)(rawKey & 0xffffffff), mod);
    }

    public HcaCipherConfig(HcaCipherType cipherType) {
        rawKey = 0;

        if (cipherType == HcaCipherType.WITH_KEY) {
            cipherType = HcaCipherType.NO_CIPHER;
        }

        this.cipherType = cipherType;
    }

    public long getKey() {
        long rawKey = getRawKey();

        if (keyModifier == 0) {
            return rawKey;
        }

        long k = (Short.toUnsignedLong(keyModifier) << 16) | ((~Short.toUnsignedInt(keyModifier) + 2) & 0x0000ffff);
        long key = rawKey * k;

        return key;
    }

    public int getKey1() {
        return (int)(rawKey >>> 32);
    }

    public void setKey1(int key1) {
        int key2 = getKey2();
        long key = makeRawKey(key1, key2);

        setRawKey(key);
    }

    public int getKey2() {
        return (int)(rawKey & 0xffffffff);
    }

    public void setKey2(int key2) {
        int key1 = getKey1();
        long key = makeRawKey(key1, key2);

        setRawKey(key);
    }

    public long getRawKey() {
        return rawKey;
    }

    public void setRawKey(long rawKey) {
        this.rawKey = rawKey;
    }

    public short getKeyModifier() {
        return keyModifier;
    }

    public void setKeyModifier(short mod) {
        this.keyModifier = mod;
    }

    public HcaCipherType getCipherType() {
        return cipherType;
    }

    public void setCipherType(HcaCipherType cipherType) {
        this.cipherType = cipherType;
    }

    @Override
    public HcaCipherConfig clone() {
        HcaCipherConfig clone;

        try {
            clone = (HcaCipherConfig)super.clone();
        } catch (CloneNotSupportedException e) {
            clone = null;
        }

        return clone;
    }

    private static long makeRawKey(int key1, int key2) {
        return (((long)key1) << 32) | key2;
    }

}
