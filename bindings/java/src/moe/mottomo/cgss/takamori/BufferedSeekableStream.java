package moe.mottomo.cgss.takamori;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Arrays;

final class BufferedSeekableStream implements AutoCloseable {

    private final InputStream inputStream;
    private final OutputStream outputStream;

    private byte[] bytes;
    private long totalRead;
    private long position;
    private long length;

    public BufferedSeekableStream(InputStream inputStream, long length) {
        this(inputStream, null, length);
    }

    public BufferedSeekableStream(OutputStream outputStream) {
        this(null, outputStream, 0);
    }

    public BufferedSeekableStream(InputStream inputStream, OutputStream outputStream, long length) {
        if (inputStream == null && outputStream == null) {
            throw new IllegalArgumentException("inputStream and outputStream cannot be both null.");
        }

        this.inputStream = inputStream;
        this.outputStream = outputStream;
        this.length = length;
    }

    public boolean isReadable() {
        return inputStream != null;
    }

    public boolean isWriteable() {
        return outputStream != null;
    }

    public boolean isSeekable() {
        return true;
    }

    public int read(byte[] buffer, int offset, int count) throws IOException {
        return readInternal(buffer, offset, count, getPosition(), true);
    }

    public int write(byte[] buffer, int offset, int count) throws IOException {
        return writeInternal(buffer, offset, count, getPosition(), true);
    }

    public void flush() throws IOException {
        if (isWriteable()) {
            if (bytes != null) {
                int length = (int)getLength();
                outputStream.write(bytes, 0, length);
            }

            outputStream.flush();
        }
    }

    public long getPosition() {
        return position;
    }

    public void setPosition(long position) {
        if (position < 0) {
            throw new IllegalArgumentException("BufferedSeekableStream.setPosition:position");
        }

        long length = getLength();

        if (position > length) {
            ensureCapacity((int)position);
        }

        this.position = position;
    }

    public long getLength() {
        return length;
    }

    public void setLength(long length) {
        if (length < 0) {
            throw new IllegalArgumentException("BufferedSeekableStream.setLength:length");
        }

        long oldLength = getLength();

        if (length > oldLength) {
            ensureCapacity((int)length);
        } else if (length < oldLength) {
            Arrays.fill(bytes, (int)length, (int)(oldLength - 1), (byte)0);
        }

        this.length = length;
    }

    @Override
    public void close() throws Exception {
        flush();

        if (inputStream != null) {
            inputStream.close();
        }

        if (outputStream != null) {
            outputStream.close();
        }
    }

    private byte[] ensureCapacity(int capacity) {
        if (bytes == null) {
            bytes = new byte[capacity];
        } else {
            int oldSize = bytes.length;

            if (oldSize < capacity) {
                int newSize = getNextCapacitySize(oldSize, capacity);
                byte[] newBuffer = new byte[newSize];

                System.arraycopy(bytes, 0, newBuffer, 0, oldSize);

                bytes = newBuffer;
            }
        }

        return bytes;
    }

    private int readInternal(byte[] buffer, int offset, int count, long position, boolean setPosition) throws IOException {
        if (inputStream == null) {
            throw new UnsupportedOperationException();
        }

        int maxRead = computeAvailableBufferCount(buffer, offset, count);

        if (maxRead == 0) {
            return 0;
        }

        long newPosition = position + maxRead;

        // TODO: Sometimes we don't need this big...
        ensureCapacity((int)newPosition);

        int totalRead = 0;

        if (position < this.totalRead) {
            // We can read this part from our cache
            int read = (int)(this.totalRead - position);

            System.arraycopy(bytes, (int)(position + totalRead), buffer, offset, read);

            totalRead += read;
        }

        if (totalRead < maxRead) {
            // Read this part from input stream
            int read = inputStream.read(buffer, offset + totalRead, maxRead - totalRead);

            if (read > 0) {
                // Copy to our cache
                System.arraycopy(buffer, offset + totalRead, bytes, (int)(position + totalRead), read);
                this.totalRead += read;
            }

            totalRead += read;
        }

        newPosition = position + totalRead;

        if (setPosition) {
            this.position = newPosition;
        }

        return totalRead;
    }

    private int writeInternal(byte[] buffer, int offset, int count, long position, boolean setPosition) throws IOException {
        if (outputStream == null) {
            throw new UnsupportedOperationException();
        }

        int maxWrite = computeAvailableBufferCount(buffer, offset, count);

        if (maxWrite == 0) {
            return 0;
        }

        long newPosition = position + maxWrite;

        ensureCapacity((int)newPosition);

        System.arraycopy(buffer, offset, bytes, (int)position, maxWrite);

        if (setPosition) {
            this.position = newPosition;
        }

        return maxWrite;
    }

    private static int getNextCapacitySize(int current, int desired) {
        final float mul = 1.5f;
        final int maxDelta = 4 * 1024;

        int result = current;

        while (result < desired) {
            result = Math.round(result * mul);
        }

        if (result - desired > maxDelta) {
            result = desired + maxDelta;
        }

        return result;
    }

    private static int computeAvailableBufferCount(byte[] buffer, int offset, int count) {
        if (offset < 0) {
            throw new IllegalArgumentException("BufferedSeekableStream.computeAvailableBufferCount:offset");
        }

        if (count < 0) {
            return 0;
        }

        if (offset + count > buffer.length) {
            return Math.max(buffer.length - offset, 0);
        } else {
            return count;
        }
    }

}
