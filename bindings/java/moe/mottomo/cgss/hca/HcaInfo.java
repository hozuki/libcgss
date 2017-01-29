package moe.mottomo.cgss.hca;

public final class HcaInfo implements Cloneable {

    public HcaInfo() {
    }

    public short versionMajor;
    public short versionMinor;
    public int channelCount;
    public int samplingRate;
    public int blockCount;
    public short blockSize;
    public short athType;
    public boolean loopExists;
    public int loopStart;
    public int loopEnd;
    public HcaCipherType cipherType;
    public float rvaVolume;
    public byte commentLength;
    public String comment;

    public short fmtR01, fmtR02;
    public short compR01, compR02, compR03, compR04, compR05, compR06, compR07, compR08;
    public int compR09;
    public short vbrR01, vbrR02;
    public short loopR01, loopR02;

    public int dataOffset;

    @Override
    public HcaInfo clone() {
        try {
            return (HcaInfo)super.clone();
        } catch (CloneNotSupportedException e) {
            return null;
        }
    }

}
