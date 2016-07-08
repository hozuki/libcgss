# Behind the Scenes

Currently we use `hcaenc_lite.dll` to encode HCA files from WAVE files.
More detailed information is unknown for now. So here we mainly talk
about the cipher conversion between HCA files.

## How HCA Cipher Conversion Works

The basis to everything is: the deciphering and the decoding process
are independent. Thanks to [@hyspace](https://github.com/hyspace) for
figuring this out. Moreover, the checksum verification process is also
independent. In the whole HCA to WAVE streamline, their orders are as below:

> HCA data frame =\> Checksum verification -\> Deciphering -\> Decoding =\> WAVE data block

Thus here, all the settings, such as sampling rate, channel count, compression
quality, only matter in the decoding process. All we need to know in the first
two steps is the frame size, which is described in the HCA header.

### 1. The Checksum

The header itself, along with all the audio frames, is signed with a checksum.
The checksum is calculated through hashing all the data bytes (see `CHCA::Checksum()`).
Remember, data bytes, and data bytes only. If we consider the header as a special
frame, all the frames in an HCA file can be described as below:

```plain
|              Data            | Checksum |
|------------------------------|----------|
|  dd   dd   dd   ..   dd   dd   cc   cc  |
```

As you can see, the last two bytes (a `WORD`) are always the checksum. If the data is
corrupted, verifying the checksum will expose the corrupted frame. Changing the data
is very likely for the decoder to recognize the frame is corrupted, causing an error.
We are not able to bypass the security check, so we must calculate the checksum ourselves.

Luckily, the algorithm is interesting. It is based on an XOR table. Have you noticed that,
this table is carefully designed, and it has a great characteristic:

> For byte sequence *S*(*b<sub>1</sub>*, *b<sub>2</sub>*, ..., *b<sub>n</sub>*), use the checksum function *f* to calculate
> its checksum *c*(*c<sub>1</sub>*, *c<sub>2</sub>*):
>
> *f*(*S*) -\> *c*
>
> Concatenate *c* after *S* to form a new sequence *S'*(*b<sub>1</sub>*, *b<sub>2</sub>*, ..., *b<sub>n</sub>*, *c<sub>1</sub>*, *c<sub>2</sub>*).
> Now the checksum of *S'* is 0:
>
> *f*(*S'*) -\> 0

To pass the checksum verification, the checksum of the whole frame should be 0. It can be
easily met by adding the checksum of real data after the data bytes, in big endian.

### 2. Decryption and Encryption

Decryption of HCA file is simple. It only uses a substitution table for byte mapping:

> *f* := *b* -\> *b'*

Under all three cipher types, all 256 bytes are uniquely mapped in the substitution tables.
So the inverse function, which is also the encryption function, is unique, given a well-formed
substitution table.

Type 0 is a special case whose table maps a byte to itself. Obviously, type 0 is the same as
plain text. Before decoding, the decryption function always decrypts the encrypted to plain
text. With another encryption function, the plain text can be once again encrypted. By the way,
type 1 is another special case, whose substitution table is static (generated without any argument).

Thus re-encryption is also simple:

1. use cipher module #1, configured with cipher type and keys (if necessary), to decrypt the HCA file;
2. the audio data is in plain text;
3. use cipher module #2, configured with cipher type and keys (if necessary), to encrypt the HCA file.

The final HCA file is completed encrypted or still plain text, depending on the configuration of cipher
module #2.
