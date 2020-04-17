package moe.mottomo.cgss.takamori;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public final class JavaDuplexStream extends Stream {

    private final BufferedSeekableStream stream;

    public JavaDuplexStream(InputStream stream, long length) {
        this.stream = new BufferedSeekableStream(stream, length);
    }

    public JavaDuplexStream(OutputStream stream) {
        this.stream = new BufferedSeekableStream(stream);
    }

    public JavaDuplexStream(InputStream inputStream, OutputStream outputStream, long length) {
        this.stream = new BufferedSeekableStream(inputStream, outputStream, length);
    }

    @Override
    public int read(byte[] buffer, int offset, int count) {
        int read = 0;

        try {
            read = stream.read(buffer, offset, count);
        } catch (IOException e) {
            e.printStackTrace();
        }

        return read;
    }

    @Override
    public int write(byte[] buffer, int offset, int count) {
        int written = 0;

        try {
            written = stream.write(buffer, offset, count);
        } catch (IOException e) {
            e.printStackTrace();
        }

        return written;
    }

    @Override
    public boolean isWritable() {
        return stream.isWriteable();
    }

    @Override
    public boolean isReadable() {
        return stream.isReadable();
    }

    @Override
    public boolean isSeekable() {
        return stream.isSeekable();
    }

    @Override
    public void seek(long offset, StreamSeekOrigin origin) {
        long position = getPosition();
        long newPosition;

        switch (origin) {
            case Begin:
                newPosition = offset;
                break;
            case Current:
                newPosition = position + offset;
                break;
            case End:
                long length = getLength();
                newPosition = length + offset;
                break;
            default:
                throw new IllegalArgumentException("JavaStream.seek:origin");
        }

        if (newPosition < 0) {
            newPosition = 0;
        }

        setPosition(newPosition);
    }

    @Override
    public long getPosition() {
        return stream.getPosition();
    }

    @Override
    public void setPosition(long position) {
        stream.setPosition(position);
    }

    @Override
    public long getLength() {
        return stream.getLength();
    }

    @Override
    public void setLength(long length) {
        stream.setLength(length);
    }

    @Override
    public void flush() {
        try {
            stream.flush();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void copyTo(Stream destination, int bufferSize) {
        byte[] buffer = new byte[bufferSize];
        int read;

        do {
            read = this.read(buffer, 0, bufferSize);

            if (read > 0) {
                destination.write(buffer, 0, read);
            }
        } while (read > 0);
    }

}
