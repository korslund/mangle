#ifndef __MANGLE_STREAM_COPYSTREAM_HPP_
#define __MANGLE_STREAM_COPYSTREAM_HPP_

#include "../stream.hpp"
#include <vector>

namespace Mangle {
namespace Stream {

  /** CopyStream::copy() is a convencience function that reads from
      one stream and write to another.

      It's wrapped as a header-only static struct function for easier
      compiler setup.

      You can either read the whole stream, or stop after a given
      number of bytes.
   */
  struct CopyStream
  {
    static size_t copy(StreamPtr input, StreamPtr output,
                       size_t stopAfter = 0)
    {
      assert(input->isReadable);
      assert(output->isWritable);

      size_t toRead = stopAfter;

      if(!toRead)
        {
          // Calculate input size
          if(input->hasSize && input->hasPosition)
            toRead = input->size() - input->tell();
        }

      // Tell the output that we are about to write. If toRead is
      // still zero at this point, it means 'read to end of stream'.
      if(toRead)
        output->reserve(toRead);

      // Can we use pointers?
      if(input->hasPtr && toRead)
        {
          // That makes things easy
          output->write(input->getPtr(toRead), toRead);
          return toRead;
        }

      // Read in increments
      std::vector<char> buf(0x10000);
      size_t written = 0;
      while(!input->eof())
        {
          size_t count = buf.size();
          if(toRead && count > toRead)
            count = toRead;
          count = input->read(&buf[0], count);
          written += output->write(&buf[0], count);

          if(toRead)
            {
              toRead -= count;
              if(toRead == 0)
                break;
            }                 
        }
      return written;
    }
  };
}}
#endif
