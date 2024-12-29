#include <kernel/string.h>
#include <cpio.h>

static
size_t
decode_size(char const *size) {
  size_t acc = 0;
  for (size_t i=0; i<8; ++i) {
    acc <<= 4;
    if (size[i] <= '9') {
      acc |= (size_t)(size[i] - '0');
    } else if (size[i] <= 'F') {
      acc |= (size_t)(size[i] - 'A') + 10;
    } else {
      acc |= (size_t)(size[i] - 'a') + 10;
    }
  }
  return acc;
}

static
size_t
align_down(size_t n, size_t size) {
  return n - (n % size);
}

static
size_t
align_up(size_t n, size_t size) {
  return align_down(n + (size - 1), size);
}

struct cpio_newc_header const *
cpio_lookup(struct cpio_newc_header const *cpio, char const *filename) {
  while (strcmp((char const *)&cpio[1], "TRAILER!!!")) {
    if (strcmp((char const *)&cpio[1], filename) == 0)
      return cpio;

    cpio = (struct cpio_newc_header const *)(cpio_get_content(cpio) + align_up(cpio_get_size(cpio), 4));
  }
  return NULL;
}

size_t
cpio_get_size(struct cpio_newc_header const *cpio) {
  return decode_size(cpio->c_filesize);
}

char const *
cpio_get_content(struct cpio_newc_header const *cpio) {
  size_t headersize = align_up(sizeof(struct cpio_newc_header) + decode_size(cpio->c_namesize), 4);
  return ((char const *)cpio) + headersize;
}
