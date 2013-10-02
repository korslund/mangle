#ifndef MANGLE_STREAM_STRINGSERVER_H
#define MANGLE_STREAM_STRINGSERVER_H

#include "memory_stream.hpp"

namespace Mangle {
namespace Stream {

/** A specialized MemoryStream that takes an std::string as input. A
    local copy of the string is stored in the instance.

    NOTE on using MemoryStream::clone(): Clones are invalidated once
    the original StringStream goes out of scope and is deallocated.
 */
class StringStream : public MemoryStream
{
  const std::string str;

 public:
  StringStream(const std::string &_str)
    : str(_str)
  {
    // Call this after initializing the class variable str, since we
    // need the locally allocated pointer.
    set(str.c_str(), str.length());
  }

  static MemoryStreamPtr Open(const std::string &str)
  { return MemoryStreamPtr(new StringStream(str)); }
};

}} // namespaces
#endif
