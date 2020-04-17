package moe.mottomo.cgss.kawashima;

public final class HcaDecoderConfig implements Cloneable {

    public HcaDecoderConfig() {
        cipherConfig = new HcaCipherConfig();
        waveHeaderEnabled = true;
        decodeFunc = HcaDecodeFunction.DECODE_16BIT_S;
    }

    public HcaCipherConfig cipherConfig;
    public boolean waveHeaderEnabled;
    public boolean loopEnabled;
    public int loopCount;
    public HcaDecodeFunction decodeFunc;

    @Override
    public HcaDecoderConfig clone() {
        try {
            HcaDecoderConfig config = (HcaDecoderConfig)super.clone();
            config.cipherConfig = cipherConfig.clone();
            return config;
        } catch (CloneNotSupportedException e) {
            return null;
        }
    }

}
