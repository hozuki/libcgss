# hcatools

**hcatools** is a set of tools related to HCA audio. Currently it contains:

- **hcaenc**: HCA encoder utility.
- **hcacc**: HCA cipher conversion utility.

In addition, [kawashima](https//github.com/Hozuki/kawashima) is a separated
library for decoding HCA audio.

## hcaenc

**hcaenc** is used to encode an HCA audio file from a WAVE audio file.
Internally it calls `hcaencEncodeToFile` API in `hcaenc_lite.dll`
included in [ADX2LE](http://www.adx2le.com) suite.

Usage:

```cmd
hcaenc.exe <input WAVE> <output HCA>
```

Example:

```cmd
hcaenc.exe input.wav output.hca
```

Please note that the HCA encoded by `hcaenc_lite.dll` is always statically
encrypted (cipher type 1). Its sampling rate is fixed at `44100` Hz.

Due to copyright reasons, `hcaenc_lite.dll` is not supplied in this repository.
Please download ADX2LE suite manually.

## hcacc

**hcacc** is a utility that can convert HCA files from one cipher type to
another.

Usage:

```cmd
hcacc.exe <input HCA> <output HCA> [-ot <output HCA cipher type>]
          [-i1 <input HCA key 1 (if necessary)>] [-i2 <input HCA key 2 (if necessary)>]
          [-o1 <output HCA key 1>] [-o2 <output HCA key 2>]
```

The valid cipher types are:

- `0` (not encrypted);
- `1` (statically encrypted);
- `56` (encrypted with a key pair).

The cipher type of the input HCA file is automatically detected, and it cannot be
set by code.

Example:

```cmd
hcacc.exe input.hca output.hca -ot 0
```

Assume we are using an HCA file encoded by **hcaenc** (or `hcaenc_lite.dll`),
and this command will convert the whole `input.hca` from cipher type 1 to
cipher type 0, and write to the new file `output.hca`.

This utility is extremly useful when creating HCA files for games. Since
`hcaenc_lite.dll` is hardcoded as type 1-only, the encoded HCA may not be
accepted by games that use type 56 (which also have an independent
key pair) or type 0 HCA audio files. A little conversion should do the magic
and solve this problem.

## License & Disclaimer

[WTFPL](http://www.wtfpl.net/txt/copying/)

[Disclaimer](DISCLAIMER.md)

## Notes

Suggested compiling toolchains are MSVC, MinGW/gcc, and Cygwin/gcc, because you will
have to invoke a function in a DLL.

The C# version of **hcaenc** and **hcacc** can be found in [DereTore](https://github.com/Hozuki/DereTore),
as presentational applications.

More tech details can be found in [Behind the Scenes](BEHIND_THE_SCENES.md).
