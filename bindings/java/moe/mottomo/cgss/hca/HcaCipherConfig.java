package moe.mottomo.cgss.hca;

public final class HcaCipherConfig {

    public HcaCipherConfig() {
        this(HcaCipherType.NO_CIPHER);
    }

    public HcaCipherConfig(int key1, int key2) {
        if (key1 == 0 && key2 == 0) {
            cipherType = HcaCipherType.NO_CIPHER;
        } else {
            cipherType = HcaCipherType.WITH_KEY;
        }
        key = (((long)key1) << 32) | key2;
    }

    public HcaCipherConfig(long key) {
        this((int)(key >>> 32), (int)(key & 0xffffffff));
    }

    public HcaCipherConfig(HcaCipherType cipherType) {
        key = 0;
        if (cipherType == HcaCipherType.WITH_KEY) {
            cipherType = HcaCipherType.NO_CIPHER;
        }
        this.cipherType = cipherType;
    }

    public int getKey1() {
        return (int)(key >>> 32);
    }

    public int getKey2() {
        return (int)(key & 0xffffffff);
    }

    @Override
    public HcaCipherConfig clone() {
        try {
            return (HcaCipherConfig)super.clone();
        } catch (CloneNotSupportedException e) {
            return null;
        }
    }

    public HcaCipherType cipherType;

    public long key;

}
