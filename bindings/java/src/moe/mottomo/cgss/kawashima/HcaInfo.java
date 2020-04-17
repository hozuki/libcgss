package moe.mottomo.cgss.kawashima;

public final class HcaInfo implements Cloneable {

    private short versionMajor;
    private short versionMinor;
    private int channelCount;
    private int samplingRate;
    private int blockCount;
    private short blockSize;
    private short athType;
    private boolean loopExists;
    private int loopStart;
    private int loopEnd;
    private HcaCipherType cipherType;
    private float rvaVolume;
    private byte commentLength;
    private String comment;

    private short fmtR01, fmtR02;
    private short compR01, compR02, compR03, compR04, compR05, compR06, compR07, compR08;
    private int compR09;
    private short vbrR01, vbrR02;
    private short loopR01, loopR02;

    private int dataOffset;

    private HcaInfo() {
        cipherType = HcaCipherType.NO_CIPHER;
        comment = "";
    }

    @Override
    public HcaInfo clone() {
        HcaInfo clone;

        try {
            clone = (HcaInfo)super.clone();
        } catch (CloneNotSupportedException e) {
            clone = null;
        }

        return clone;
    }

    public short getVersionMajor() {
        return versionMajor;
    }

    public short getVersionMinor() {
        return versionMinor;
    }

    public int getChannelCount() {
        return channelCount;
    }

    public int getSamplingRate() {
        return samplingRate;
    }

    public int getBlockCount() {
        return blockCount;
    }

    public short getBlockSize() {
        return blockSize;
    }

    public short getAthType() {
        return athType;
    }

    public boolean hasLoop() {
        return loopExists;
    }

    public int getLoopStart() {
        return loopStart;
    }

    public int getLoopEnd() {
        return loopEnd;
    }

    public HcaCipherType getCipherType() {
        return cipherType;
    }

    public float getRvaVolume() {
        return rvaVolume;
    }

    public byte getCommentLength() {
        return commentLength;
    }

    public String getComment() {
        return comment;
    }

    public short getFmtR01() {
        return fmtR01;
    }

    public short getFmtR02() {
        return fmtR02;
    }

    public short getCompR01() {
        return compR01;
    }

    public short getCompR02() {
        return compR02;
    }

    public short getCompR03() {
        return compR03;
    }

    public short getCompR04() {
        return compR04;
    }

    public short getCompR05() {
        return compR05;
    }

    public short getCompR06() {
        return compR06;
    }

    public short getCompR07() {
        return compR07;
    }

    public short getCompR08() {
        return compR08;
    }

    public int getCompR09() {
        return compR09;
    }

    public short getVbrR01() {
        return vbrR01;
    }

    public short getVbrR02() {
        return vbrR02;
    }

    public short getLoopR01() {
        return loopR01;
    }

    public short getLoopR02() {
        return loopR02;
    }

    public int getDataOffset() {
        return dataOffset;
    }
}
