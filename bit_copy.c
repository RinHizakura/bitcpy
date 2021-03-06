#include <stdint.h>
#include <stdio.h>

static const uint8_t read_mask[] = {
    0x00, /*	== 0	00000000b	*/
    0x80, /*	== 1	10000000b	*/
    0xC0, /*	== 2	11000000b	*/
    0xE0, /*	== 3	11100000b	*/
    0xF0, /*	== 4	11110000b	*/
    0xF8, /*	== 5	11111000b	*/
    0xFC, /*	== 6	11111100b	*/
    0xFE, /*	== 7	11111110b	*/
    0xFF  /*	== 8	11111111b	*/
};

static const uint8_t write_mask[] = {
    0xFF, /*	== 0	11111111b	*/
    0x7F, /*	== 1	01111111b	*/
    0x3F, /*	== 2	00111111b	*/
    0x1F, /*	== 3	00011111b	*/
    0x0F, /*	== 4	00001111b	*/
    0x07, /*	== 5	00000111b	*/
    0x03, /*	== 6	00000011b	*/
    0x01, /*	== 7	00000001b	*/
    0x00  /*	== 8	00000000b	*/
};

void bitcpy_ori(void *_dest,      /* Address of the buffer to write to */
                size_t _write,    /* Bit offset to start writing to */
                const void *_src, /* Address of the buffer to read from */
                size_t _read,     /* Bit offset to start reading from */
                size_t _count)
{
    uint8_t data, original, mask;
    size_t bitsize;
    size_t read_lhs = _read & 7;
    size_t read_rhs = 8 - read_lhs;
    const uint8_t *source = _src + (_read / 8);
    size_t write_lhs = _write & 7;
    size_t write_rhs = 8 - write_lhs;
    uint8_t *dest = _dest + (_write / 8);

    while (_count > 0) {
        data = *source++;
        bitsize = (_count > 8) ? 8 : _count;
        if (read_lhs > 0) {
            data <<= read_lhs;
            if (bitsize > read_rhs)
                data |= (*source >> read_rhs);
        }

        if (bitsize < 8)
            data &= read_mask[bitsize];

        original = *dest;
        if (write_lhs > 0) {
            mask = read_mask[write_lhs];
            if (bitsize > write_rhs) {
                *dest++ = (original & mask) | (data >> write_lhs);
                original = *dest & write_mask[bitsize - write_rhs];
                *dest = original | (data << write_rhs);
            } else {
                if ((bitsize - write_lhs) > 0)
                    mask = mask | write_mask[8 - (bitsize - write_lhs)];
                *dest++ = (original & mask) | (data >> write_lhs);
            }
        } else {
            if (bitsize < 8)
                data = data | (original & write_mask[bitsize]);
            *dest++ = data;
        }

        _count -= bitsize;
    }
}

void bitcpy_naive(void *_dest,      /* Address of the buffer to write to */
                  size_t _write,    /* Bit offset to start writing to */
                  const void *_src, /* Address of the buffer to read from */
                  size_t _read,     /* Bit offset to start reading from */
                  size_t _count)
{
    uint8_t data, original;
    size_t bitsize;
    size_t read_lhs = _read & 7;
    size_t read_rhs = 8 - read_lhs;
    const uint8_t *source = _src + (_read / 8);
    size_t write_lhs = _write & 7;
    size_t write_rhs = 8 - write_lhs;
    uint8_t *dest = _dest + (_write / 8);

    while (_count > 0) {
        data = *source++;
        bitsize = (_count > 8) ? 8 : _count;
        data <<= read_lhs;
        data |= (bitsize > read_rhs) ? (*source >> read_rhs) : 0;

        data &= read_mask[bitsize];
        original = *dest;
        int idx = bitsize > write_rhs ? 8 : 8 - (write_rhs - bitsize);
        *dest++ = (original & (read_mask[write_lhs] | write_mask[idx])) |
                  (data >> write_lhs);
        idx = bitsize < write_rhs ? 0 : bitsize - write_rhs;
        *(dest) = (*dest & write_mask[idx]) | (data << write_rhs);

        _count -= bitsize;
    }
}

void bitcpy(void *_dest,      /* Address of the buffer to write to */
            size_t _write,    /* Bit offset to start writing to */
            const void *_src, /* Address of the buffer to read from */
            size_t _read,     /* Bit offset to start reading from */
            size_t _count)
{
    size_t read_lhs = _read & 7;
    size_t read_rhs = 8 - read_lhs;
    const uint8_t *source = _src + (_read / 8);
    size_t write_lhs = _write & 7;
    size_t write_rhs = 8 - write_lhs;
    uint8_t *dest = _dest + (_write / 8);


    if (write_lhs != 0 && _count >= write_rhs) {
        bitcpy_naive(_dest, _write, _src, _read, write_rhs);

        if (write_rhs > read_rhs)
            source++;

        _write += write_rhs;
        _read += write_rhs;
        _count -= write_rhs;

        dest++;

        read_lhs = _read & 7;
        read_rhs = 8 - read_lhs;

        /* No update is ok because we dont need this after
            write_lhs = _write & 7;
            write_rhs = 8 - write_lhs; */
    }


    int iter_cnt = _count >> 3;
    int iter = _count >> 3;
    _count &= 7;

    while (--iter_cnt >= 0) {
        *dest++ = (*source << read_lhs) | ((*(source + 1) >> read_rhs));
        source++;
    }

    bitcpy_naive(_dest, _write + (iter << 3), _src, _read + (iter << 3),
                 _count);
}
