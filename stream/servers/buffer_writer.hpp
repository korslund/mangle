#ifndef MANGLE_STREAM_BUFFERWRITER_H
#define MANGLE_STREAM_BUFFERWRITER_H

#include "memory_stream.hpp"
#include <vector>

namespace Mangle {
namespace Stream {

/** A stream that writes to a contiguous memory buffer.

    The buffer is grown in size as needed, but it is highly
    recommended to pre-allocate the buffer using reserve() if you know
    the size up front.

    The class inherits from MemoryStream, so you can use it as a fully
    enabled read-write stream. Note the following caveats though:

    - Mangle does not separate write and read positions. So if you
      read and write to the same stream, make sure to keep track of
      the position yourself.

    - Writing to the stream, including using the getWPtr() functions,
      will invalidate all previous pointers returned from the
      stream. It also invalidates all streams obtained through
      MemoryStream::clone().
 */
class BufferWriter : public MemoryStream
{
  std::vector<char> buf;

 public:
  BufferWriter()
  {
    isWritable = true;
  }

  /** Get a WRITABLE pointer for 'count' bytes from the given
      position. Will allocate more memory as necessary. Does not
      affect the position pointer.
  */
  void *getWPtr(size_t pos, size_t count)
  {
    assert(buf.size() == size());

    size_t end = pos + count;
    if(end > buf.size())
      {
        // We needs us sum more buffer!
        buf.resize(end);

        // Reset data pointer and size
        size_t tmp = tell();
        set(&buf[0], buf.size());
        seek(tmp);

        assert(buf.size() == size());
        assert(size() == end);
      }

    // Return the pointer
    assert(end <= buf.size());
    return &buf[0] + pos;
  }

  /** Get a WRITABLE pointer for 'count' bytes from the current
      position. Like getPtr(count), this increases the position pointer
      as well.
  */
  void *getWPtr(size_t count)
  {
    void *res = getWPtr(tell(), count);
    seek(tell() + count);
    return res;
  }

  size_t write(const void* data, size_t len)
  {
    if(!len) return 0;

    // Copy the data
    memcpy(getWPtr(len), data, len);
    return len;
  }

  void reserve(size_t count)
  {
    buf.reserve(tell() + count);
  }
};

typedef boost::shared_ptr<BufferWriter> BufferWriterPtr;

}} // namespaces
#endif
