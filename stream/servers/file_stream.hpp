#ifndef MANGLE_STREAM_FILESERVER_H
#define MANGLE_STREAM_FILESERVER_H

#include "std_stream.hpp"
#include <fstream>
#include <stdexcept>

namespace Mangle {
namespace Stream {

/** Very simple file input stream, based on std::ifstream
 */
class FileStream : public StdStream
{
  std::ifstream file;

 public:
  FileStream(const std::string &name)
    : StdStream(&file)
    {
      file.open(name.c_str(), std::ios::binary);

      if(file.fail())
        throw std::runtime_error("FileStream: failed to open file " + name);
    }
  ~FileStream() { file.close(); }

  static StreamPtr Open(const std::string &name)
  { return StreamPtr(new FileStream(name)); }

  // Convenience function to read a buffer directly from a file, then
  // close it
  static size_t Read(const std::string &file, void *ptr, size_t count)
  { return Open(file)->read(ptr, count); }
};

typedef boost::shared_ptr<FileStream> FileStreamPtr;

}} // namespaces
#endif
