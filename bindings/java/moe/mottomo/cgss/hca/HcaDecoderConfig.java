package moe.mottomo.cgss.hca;

public final class HcaDecoderConfig implements Cloneable {

    public HcaDecoderConfig() {
        this.cipherConfig = new HcaCipherConfig();
    }

    public HcaCipherConfig cipherConfig;
    public boolean waveHeaderEnabled;
    public boolean loopEnabled;
    public int loopCount;
    public HcaDecodeFunc decodeFunc;

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
