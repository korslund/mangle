#ifndef MANGLE_STREAM_STRINGWRITER_H
#define MANGLE_STREAM_STRINGWRITER_H

#include "../stream.hpp"
#include <string>

namespace Mangle {
namespace Stream {

/** A stream that writes to an std::string. You provide the string
    yourself, so if you are writing large amounts of data you can
    optimize by calling string::reserve() first. You can also call
    string::reserve() indirectly through Stream::reserve().

    The data is appended to any existing data in the string.
 */
class StringWriter : public Stream
{
  std::string &out;

 public:
  StringWriter(std::string &_out)
    : out(_out)
  {
    isSeekable = false;
    hasPosition = false;
    hasSize = false;
    isWritable = true;
    isReadable = false;
  }

  static StreamPtr Open(std::string &out)
  { return StreamPtr(new StringWriter(out)); }

  size_t write(const void* buf, size_t len)
  {
    out.append((const char*)buf, len);
    return len;
  }

  void reserve(size_t count)
  {
    out.reserve(out.size() + count);
  }
};

typedef boost::shared_ptr<StringWriter> StringWriterPtr;

}} // namespaces
#endif
