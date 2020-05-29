# libcgss

**libcgss** is a helper library originally for [THE iDOLM@STER Cinderella Girls Starlight Stage](http://cinderella.idolmaster.jp/sl-stage/)
(CGSS). It provides both C and C++ API for HCA audio and ACB archive manipulation.

[![AppVeyor](https://img.shields.io/appveyor/ci/hozuki/libcgss.svg)](https://ci.appveyor.com/project/hozuki/libcgss)
[![GitHub contributors](https://img.shields.io/github/contributors/hozuki/libcgss.svg)](https://github.com/hozuki/libcgss/graphs/contributors)
[![Libraries.io for GitHub](https://img.shields.io/librariesio/github/hozuki/libcgss.svg)](https://github.com/hozuki/libcgss)
[![GitHub (pre-)release](https://img.shields.io/github/release/hozuki/libcgss/all.svg)](https://github.com/hozuki/libcgss/releases)
[![Github All Releases](https://img.shields.io/github/downloads/hozuki/libcgss/total.svg)](https://github.com/hozuki/libcgss/releases)

The project consists of a library (*libcgss*) and several other applications.

- Library (*libcgss*)
  - [*takamori*](http://www.project-imas.com/wiki/Aiko_Takamori): Base structure: streams and exceptions.
  - [*ichinose*](http://www.project-imas.com/wiki/Shiki_Ichinose): CRI ACB/AWB archive reader.
  - [*kawashima*](http://www.project-imas.com/wiki/Mizuki_Kawashima): HCA decoder, migrated from [kawashima](https://github.com/hozuki/kawashima).
- Applications
  - *hcaenc (obsolete)*: HCA encoder. Requires `hcaenc_lite.dll`<sup>\[1\]</sup>. [C# equivalent](https://github.com/OpenCGSS/DereTore/tree/master/Apps/Hcaenc)
  - *hcacc*: HCA cipher converter. [C# equivalent](https://github.com/OpenCGSS/DereTore/tree/master/Apps/Hcacc)
  - *hca2wav*: HCA decoder. [C# equivalent](https://github.com/OpenCGSS/DereTore/tree/master/Apps/Hca2Wav)
  - *hcainfo*: HCA info viewer.
  - *utftable*: UTF table schema explorer, similar to [utf_tab](https://github.com/hcs64/vgm_ripping/tree/master/multi/utf_tab).
  - *acbunpack*: ACB archive unpacker. [C# equivalent](https://github.com/OpenCGSS/DereTore/tree/master/Apps/AcbUnzip)
  - *acb2wavs*: Batch converter for HCAs in ACB. [C# equivalent](https://github.com/OpenCGSS/DereTore/tree/master/Apps/Acb2Wavs)
  - *acb2hcas*: Batch exporter for HCAs in ACB. Generates companion `.hcakey` files for [vgmstream](https://github.com/losnoco/vgmstream).
  - *hcaenc*: HCA encoder based on [VGAudio](https://github.com/Thealexbarney/VGAudio)/[VGAudio-CPP](https://github.com/hozuki/vgaudio-cpp). Does not rely on `hcaenc_lite.dll`.
  - *hcadec*: HCA decoder based on VGAudio/VGAudio-CPP.


Related projects:

- This project has a sister project [**DereTore**](https://github.com/OpenCGSS/DereTore).
However the strength of C API is better interoperability with other libraries.
The pure C# version of *hcaenc* and *hcacc* can be found in *DereTore* as applications.
- If you are looking for a faster HCA decoder with higher definition for \*NIX and you are a DIY person, have a look at
[this](https://github.com/Ishotihadus/hca).
- If you are looking for a small native application for HCA encoding/decoding, have a look at
[this](https://github.com/hozuki/vgaudio-cpp).

> <sup>\[1\]</sup> Download ADX2LE from [here](http://www.adx2le.com/download/index.html), and use
the DLL located at `tools\hcaenc_lite.dll`.

## Downloads

- [GitHub Releases](https://github.com/hozuki/libcgss/releases)
- [AppVeyor (CI auto build, may not be stable)](https://ci.appveyor.com/project/hozuki/libcgss)

## Examples

Have a look at some examples [here](docs/examples.md).

C# users can write a P/Invoke wrapper to utilize the C interface.

## Building

Requirements:

- General:
  - CMake (>=3.2)
- Windows:
  - MSVC (>=14.0, VC++ 2015)
- macOS and Linux:
  - GCC (>=6.0)

Steps:

1. Run PowerShell script: `scripts/configure-cmake.ps1`;
2. Run PowerShell script: `scripts/build-project.ps1`;

Artifacts can be found in `bin` directory.

More information:

- [JNI library building instructions](docs/jni-build-instructions.md)
- [PHP extension](https://github.com/towa0131/php-cgss) by [@towa0131](https://github.com/towa0131)

## API & Manual

API definition can be found at [docs/api/cgss-api.md](docs/api/cgss-api.md).

User manual can be found at [docs/manual/user-manual.md](docs/manual/user-manual.md).

## License

MIT License. See [LICENSE.md](LICENSE.md).

## Disclaimer

See [DISCLAIMER.md](DISCLAIMER.md).

## Notes

More tech details can be found in [Behind the Scenes](docs/behind-the-scenes.md).
