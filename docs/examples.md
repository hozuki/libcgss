# Examples

- [C](#c)
- [C++](#c-plus-plus)
- [Java](#java)

## C

[`utftable.c`](../src/apps/utftable/utftable.c)

```c
int main(int argc, const char *argv[]) {
    if (argc == 1) {
        print_help();
        return 0;
    }

    const char *file_name = argv[1];

    if (!cgssHelperFileExists(file_name)) {
        fprintf(stderr, "File '%s' does not exist or cannot be opened.\n", file_name);
        return -1;
    }

#define ERROR_BUFFER_SIZE (100)
    char error_buffer[ERROR_BUFFER_SIZE] = {'\0'};

    CGSS_HANDLE stream;

    CGSS_OP_RESULT r = cgssCreateFileStream3(file_name, CGSS_FILE_MODE_OPEN_EXISTING, CGSS_FILE_ACCESS_READ, &stream);

    if (!CGSS_OP_SUCCEEDED(r)) {
        cgssGetOpResultString(r, error_buffer, ERROR_BUFFER_SIZE);
        fprintf(stderr, "Error when creating stream, code = %d (%s)\nDetail: %s\n", r, error_buffer, cgssGetLastErrorMessage());
        return r;
    }

    UTF_TABLE *table;

    r = cgssUtfReadTable(stream, 0, &table);

    if (CGSS_OP_SUCCEEDED(r)) {
        print_table(file_name, table);

        cgssUtfFreeTable(table);
        table = NULL;
    } else {
        cgssGetOpResultString(r, error_buffer, ERROR_BUFFER_SIZE);
        fprintf(stderr, "Error when reading UTF table, code = %d (%s)\nDetail: %s\n", r, error_buffer, cgssGetLastErrorMessage());
    }

    cgssCloseHandle(stream);

    return 0;
}
```

## <span id="c-plus-plus">C++</span>

[`hca2wav.cpp`](../src/apps/hca2wav/hca2wav.cpp)

```cpp
#include "cgss_api.h"

int main() {
    cgss::CHcaDecoderConfig decoderConfig;
    decoderConfig.decodeFunc = cgss::CDefaultWaveGenerator::Decode16BitS;
    decoderConfig.waveHeaderEnabled = TRUE;
    decoderConfig.cipherConfig.keyParts.key1 = 0x12345678;
    decoderConfig.cipherConfig.keyParts.key2 = 0x90abcdef;
    
    try {
        cgss::CFileStream fileIn(HCA_FILE_NAME, cgss::FileMode::OpenExisting, cgss::FileAccess::Read),
            fileOut(WAV_FILE_NAME, cgss::FileMode::Create, cgss::FileAccess::Write);
        cgss::CHcaDecoder hcaDecoder(&fileIn, decoderConfig);
    
        uint32_t read = 1;
        static const uint32_t bufferSize = 1024;
        uint8_t buffer[bufferSize];
        while (read > 0) {
            read = hcaDecoder.Read(buffer, bufferSize, 0, bufferSize);
            if (read > 0) {
                fileOut.Write(buffer, bufferSize, 0, read);
            }
        }
    } catch (const cgss::CException &ex) {
        cerr << "Exception: " << ex.GetExceptionMessage() << ", code=" << ex.GetOpResult() << endl;
        return ex.GetOpResult();
    }
    return 0;
}
```

## Java

JNI required.

[`App.java`](../bindings/java/src/moe/mottomo/cgss/example/App.java)

```java
import moe.mottomo.cgss.kawashima.HcaDecoder;
import moe.mottomo.cgss.kawashima.HcaDecoderConfig;
import moe.mottomo.cgss.takamori.FileAccess;
import moe.mottomo.cgss.takamori.FileMode;
import moe.mottomo.cgss.takamori.FileStream;

public class App {

    public static void main(String[] args) throws Exception {
        try (FileStream fsIn = new FileStream("input.hca", FileMode.OPEN_EXISTING, FileAccess.READ)) {
            try (FileStream fsOut = new FileStream("output.wav", FileMode.CREATE, FileAccess.WRITE)) {
                HcaDecoderConfig config = new HcaDecoderConfig();
                config.cipherConfig.setKey1(0x12345678);
                config.cipherConfig.setKey2(0x90abcdef);
                config.cipherConfig.setKeyModifier((short)0xab12);

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
```
