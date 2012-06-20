#ifndef MANGLE_NULLSTREAM_H
#define MANGLE_NULLSTREAM_H

#include "../stream.hpp"
#include <string.h>

namespace Mangle {
namespace Stream {

/** A NullStream ignores all data written to it, and can be read for
    an infinite amount of zero bytes.
 */
class NullStream : public Stream
{
 public:
  NullStream()
  {
    isReadable = true;
    isWritable = true;
    isSeekable = false;
    hasPosition = false;
    hasSize = false;
    hasPtr = false;
  }

  size_t read(void *buf, size_t count)
  {
    memset(buf, 0, count);
    return count;
  }
  size_t write(const void *buf, size_t count) { return count; }
  bool eof() const { return false; }
};

}} // namespaces
#endif
