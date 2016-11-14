# libcgss

**libcgss** is a helper library for [Idolmaster Cinderella Girls Starlight Stage](http://cinderella.idolmaster.jp/sl-stage/)
(CGSS).

The project consists of a library (*libcgss*) and several other applications.

- Library
  - [*kawashima*](http://www.project-imas.com/wiki/Mizuki_Kawashima): HCA decoding sub-library, transferred from [kawashima](https://github.com/hozuki/kawashima).
  - [*takamori*](http://www.project-imas.com/wiki/Aiko_Takamori): Sub-library for polyfill.
- Applications
  - *hcaenc*: HCA encoding utility.
  - *hcacc*: HCA cipher conversion utility.
  - *hca2wav*: HCA decoding utility.

This project is written in C/C++, compared to C# of its sister project [**DereTore**](https://github.com/hozuki/DereTore),
for a better interoperability with other languages or libraries. The C# version of *hcaenc* and *hcacc*
can be found in **DereTore** as applications.

## Building

Requirements:

- CMake
- MSVC, *or* Cygwin/gcc, *or* MinGW/gcc (on Windows)
- gcc (on macOS and Linux) <sup>untested</sup>

## User Manual

See [here](docs/user-manual.md).

## License

MIT License. See [LICENSE.md](LICENSE.md).

## Disclaimer

See [here](DISCLAIMER.md).

## Notes

More tech details can be found in [Behind the Scenes](docs/behind-the-scenes.md).
