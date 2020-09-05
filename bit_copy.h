#ifndef _BITCPY_H
#define _BITCPY_H

void bitcpy_ori(void *_dest,
                size_t _write,
                const void *_src,
                size_t _read,
                size_t _count);

void bitcpy_naive(void *_dest,
                  size_t _write,
                  const void *_src,
                  size_t _read,
                  size_t _count);

void bitcpy(void *_dest,
            size_t _write,
            const void *_src,
            size_t _read,
            size_t _count);

static inline void dump_8bits(uint8_t _data)
{
    for (int i = 0; i < 8; ++i)
        printf("%d", (_data & (0x80 >> i)) ? 1 : 0);
}
static inline void dump_binary(uint8_t *_buffer, size_t _length)
{
    for (unsigned int i = 0; i < _length; ++i)

        dump_8bits(*_buffer++);
}
#endif
