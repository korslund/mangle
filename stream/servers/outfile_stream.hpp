#ifndef MANGLE_OSTREAM_FILESERVER_H
#define MANGLE_OSTREAM_FILESERVER_H

#include "std_ostream.hpp"
#include <fstream>

namespace Mangle {
namespace Stream {

/** File stream based on std::ofstream, only supports writing.
 */
class OutFileStream : public StdOStream
{
  std::ofstream file;

 public:
  /**
     By default we overwrite the file. If append=true, then we will
     open an existing file and seek to the end instead.
   */
  OutFileStream(const std::string &name, bool append=false)
    : StdOStream(&file)
    {
      std::ios::openmode mode = std::ios::binary;
      if(append)
        mode |= std::ios::app;
      else
        mode |= std::ios::trunc;

      file.open(name.c_str(), mode);

      if(file.fail())
        throw std::runtime_error("OutFileStream: failed to open file " + name);
    }
  ~OutFileStream() { flush(); file.close(); }

  static StreamPtr Open(const std::string &name)
  { return StreamPtr(new OutFileStream(name)); }

  // Write a buffer directly to a file
  static size_t Write(const std::string &file, const void *ptr, size_t count)
  { return Open(file)->write(ptr,count); }

  /* Convenience function to write a string to a file. Note that
     newlines are NOT automatically included, you have to add those to
     the string yourself.
  */
  static size_t Write(const std::string &file, const std::string &data)
  { return Write(file, data.c_str(), data.size()); }
};

typedef boost::shared_ptr<OutFileStream> OutFileStreamPtr;

}} // namespaces
#endif
