# libcgss

**libcgss** is a helper library for [THE iDOLM@STER Cinderella Girls Starlight Stage](http://cinderella.idolmaster.jp/sl-stage/)
(CGSS). It provides both C and C++ API for HCA audio manipulation. ACB archive support
is in progress.

The project consists of a library (*libcgss*) and several other applications.

- Library (*libcgss*)
  - [*takamori*](http://www.project-imas.com/wiki/Aiko_Takamori): Base structure: streams and exceptions.
  - [*ichinose*](http://www.project-imas.com/wiki/Shiki_Ichinose): CRI ACB/AWB archive reader.
  - [*kawashima*](http://www.project-imas.com/wiki/Mizuki_Kawashima): HCA decoder, migrated from [kawashima](https://github.com/hozuki/kawashima).
- Applications
  - *hcaenc*: HCA encoder. Requires `hcaenc_lite.dll`<sup>\[1\]</sup>. [C# equivalent](https://github.com/OpenCGSS/DereTore/tree/master/Apps/Hcaenc)
  - *hcacc*: HCA cipher converter. [C# equivalent](https://github.com/OpenCGSS/DereTore/tree/master/Apps/Hcacc)
  - *hca2wav*: HCA decoder. [C# equivalent](https://github.com/OpenCGSS/DereTore/tree/master/Apps/Hca2Wav)
  - *hcainfo*: HCA info viewer.
  - *utftable*: UTF table schema explorer, similar to [utf_tab](https://github.com/hcs64/vgm_ripping/tree/master/multi/utf_tab).
  - *acbunpack*: ACB archive unpacker. [C# equivalent](https://github.com/OpenCGSS/DereTore/tree/master/Apps/AcbUnzip)
  - *acb2wavs*: Batch converter for HCAs in ACB. [C# equivalent](https://github.com/OpenCGSS/DereTore/tree/master/Apps/Acb2Wavs)

This project has a sister project [**DereTore**](https://github.com/OpenCGSS/DereTore).
However the strength of C API is better interoperability with other libraries.
The pure C# version of *hcaenc* and *hcacc* can be found in *DereTore* as applications.

> \[1\] Download ADX2LE from [here](http://www.adx2le.com/download/index.html), and use
the DLL located at `tools\hcaenc_lite.dll`.

## Example 

A quick example, from [hca2wav](src/apps/hca2wav/hca2wav.cpp):

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

## Building

Requirements:

- General:
  - CMake (>=3.2)
- Windows:
  - MSVC (>=19.0, VC++ 2015) or Cygwin/GCC (>=5.0) or MinGW/GCC (>=5.0)
- macOS and Linux:
  - GCC (>=5.0)

For Windows users, please use CMake GUI (supplied with CMake) to generate a Visual Studio solution.

For macOS and Linux users, you can simply invoke the script `build.sh`.

> **Remarks:**
>
> 1. I personally recommend you to use Cygwin rather than MinGW. The latter seems to lack
> support of properly handling `try...catch` statements.
> 2. Linux build is tested on Windows Subsystem for Linux (WSL) using GCC 6.2.0.

## API & Manual

API definition can be found at [docs/api/cgss-api.md](docs/api/cgss-api.md).

User manual can be found at [docs/manual/user-manual.md](docs/manual/user-manual.md).

## License

MIT License. See [LICENSE.md](LICENSE.md).

## Disclaimer

See [DISCLAIMER.md](DISCLAIMER.md).

## Notes

More tech details can be found in [Behind the Scenes](docs/behind-the-scenes.md).
