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

## Building

Requirements:

- CMake (>=3.6)
- MSVC, or Cygwin/gcc, or MinGW/gcc (on Windows)
- gcc (on macOS and Linux, untested)

I personally recommend you to use Cygwin rather than MinGW. The latter seems to lack
support of properly handling `try...catch` statements.

## API & Manual

API definition can be found at [docs/api/cgss-api.md](docs/api/cgss-api.md).

User manual can be found at [docs/manual/user-manual-1.2.md](docs/manual/user-manual-1.2.md).

## License

MIT License. See [LICENSE.md](LICENSE.md).

## Disclaimer

See [DISCLAIMER.md](DISCLAIMER.md).

## Notes

More tech details can be found in [Behind the Scenes](docs/behind-the-scenes.md).
