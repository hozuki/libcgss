package moe.mottomo.cgss.hca;

@FunctionalInterface
public interface HcaDecodeFunc {

    int apply(float data, byte[] buffer, int cursor);

}
