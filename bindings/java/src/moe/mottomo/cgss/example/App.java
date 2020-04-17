package moe.mottomo.cgss.example;

import moe.mottomo.cgss.kawashima.HcaDecoder;
import moe.mottomo.cgss.kawashima.HcaDecoderConfig;
import moe.mottomo.cgss.takamori.FileAccess;
import moe.mottomo.cgss.takamori.FileMode;
import moe.mottomo.cgss.takamori.FileStream;

public class App {

    public static void main(String[] args) throws Exception {
        try (FileStream fsIn = new FileStream("C:\\Users\\M\\Desktop\\cv_0001\\_acb_cv_0001.acb\\external\\cue_000127.hca", FileMode.OPEN_EXISTING, FileAccess.READ)) {
            try (FileStream fsOut = new FileStream("C:\\Users\\M\\Desktop\\tmp.wav", FileMode.CREATE, FileAccess.WRITE)) {
                HcaDecoderConfig config = new HcaDecoderConfig();
                config.cipherConfig.setKey1(0xC59E7114);
                config.cipherConfig.setKey2(0x00000000);
                config.cipherConfig.setKeyModifier((short)0);

                try (HcaDecoder decoder = new HcaDecoder(fsIn, config)) {
                    final int bufferSize = 4096;
                    byte[] buffer = new byte[bufferSize];
                    int read;

                    do {
                        read = decoder.read(buffer, 0, bufferSize);

                        if (read > 0) {
                            fsOut.write(buffer, 0, read);
                        }
                    } while (read > 0);
                }
            }
        }

        System.out.println("Complete.");
    }

}
