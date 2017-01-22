# libcgss

**libcgss** is a helper library for [Idolmaster Cinderella Girls Starlight Stage](http://cinderella.idolmaster.jp/sl-stage/)
(CGSS). It provides both C and C++ API for HCA audio manipulation. ACB archive support
is in progress.

The project consists of a library (*libcgss*) and several other applications.

- Library (*libcgss*)
  - [*kawashima*](http://www.project-imas.com/wiki/Mizuki_Kawashima): HCA decoding library, migrated from [kawashima](https://github.com/hozuki/kawashima).
  - [*takamori*](http://www.project-imas.com/wiki/Aiko_Takamori): Base structure: streams and exceptions.
- Applications
  - *hcaenc*: HCA encoder. Requires `hcaenc_lite.dll`.
  - *hcacc*: HCA cipher converter.
  - *hca2wav*: HCA decoder.
  - *hcainfo*: HCA info viewer.

This project has a sister project [**DereTore**](https://github.com/OpenCGSS/DereTore).
However the strength of C API is better interoperability with other libraries.
The pure C# version of *hcaenc* and *hcacc* can be found in *DereTore* as applications.

## Example 

A quick example, from [hca2wav](src/apps/hca2wav/hca2wav.cpp):

```cpp
#include "cgss_api.h"

int main() {
    cgss::CHcaDecoderConfig decoderConfig;
    // Currently only signed 16-bit is available.
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

## Building

Requirements:

- CMake (>=3.6)
- MSVC, or Cygwin/gcc, or MinGW/gcc (on Windows)
- gcc (on macOS and Linux, untested)

I personally recommend you to use Cygwin rather than MinGW. The latter seems to lack
support of properly handling `try...catch` statements.

## API & Manual

API definition can be found at [docs/api/cgss-api.md](docs/api/cgss-api.md).

User manual can be found at [docs/manual/user-manual.md](docs/manual/user-manual.md).

## License

MIT License. See [LICENSE.md](LICENSE.md).

## Disclaimer

See [DISCLAIMER.md](DISCLAIMER.md).

## Notes

More tech details can be found in [Behind the Scenes](docs/behind-the-scenes.md).
