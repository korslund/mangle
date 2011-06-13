#ifndef MANGLE_STREAM_CAP_H
#define MANGLE_STREAM_CAP_H

#include "../stream.hpp"
#include <stdexcept>

namespace Mangle {
namespace Stream {

/** A read filter that limits the total readable number of bytes to a
    given amount. Returns eof() after that.

    This can be used as a simplified alternative to slice_stream that:
    - doesn't need or support seeking
    - doesn't support pointer operations
    - is read-only

    It is perfect for sequential reading of archive files, where you
    just want to read a part of a file once, especially if the outer
    stream is compressed and doesn't support seeking.

    The stream has size() and tell() which reflect the internal size
    and position within the cap stream, starting at zero and ending at
    size().

    Premature eof()s in the source stream, ie. running out of data
    before 'size' has been reached, will cause an exception to be
    thrown.
 */
class CapStream : public Stream
{
protected:
  StreamPtr src;
  size_t total, pos;

 public:
  CapStream(StreamPtr _src, size_t _size)
    : src(_src), total(_size), pos(0)
  {
    assert(src->isReadable);

    isReadable = true;
    isSeekable = false;
    isWritable = false;
    hasSize = true;
    hasPosition = true;
    hasPtr = false;
  }

  size_t read(void *buf, size_t count)
  {
    if(count + pos > total)
      count = total-pos;

    size_t r = src->read(buf, count);
    pos += count;

    if(r != count || // Not enough bytes were read, OR:
       (src->eof() && !eof())) // Source eof'ed without us.
      throw std::runtime_error("CapStream: to little data in source stream");

    return count;
  }

  size_t tell() const { return pos; }
  size_t size() const { return total; }
  bool eof() const { return pos >= total; }
};

typedef boost::shared_ptr<CapStream> CapStreamPtr;

}} // namespaces
#endif
