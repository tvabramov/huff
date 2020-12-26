#ifndef HUFF_H
#define HUFF_H

namespace huff {

bool encode(const char* ifn, const char* ofn);

bool decode(const char* ifn, const char* ofn);

}  // namespace huff

#endif  // HUFF_H