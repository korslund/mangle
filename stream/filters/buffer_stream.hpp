#ifndef MANGLE_STREAM_BUFFER_H
#define MANGLE_STREAM_BUFFER_H

#include "../servers/buffer_writer.hpp"

namespace Mangle {
namespace Stream {

/** BufferStream is a stream used specifically for reading another
    stream into a memory buffer.

    The constructor is given a pointer to another stream, and that
    stream is then read into memory. The class handles cases where
    there is no known size and the input must be read in chuncks until
    EOF. However if the size IS known (ie. input->hasSize == true),
    the class is optimized to use one allocation and one read.

    After the constructor has finished, the class acts as an otherwise
    normal BufferWriter (full read/write memory stream) with the
    starting seek position set to 0.

    Typically you only need a read-only buffer after the initial
    setup. In that case it might be a good idea access the class
    through its MemoryStream ancestor class. Ie.:

    MemoryStreamPtr membuf(new BufferSream(myInput));

    ... use membuf as a MemoryStream from here on
 */

class BufferStream : public BufferWriter
{
 public:
  /*
    input = stream to copy
    ADD = each read increment (for streams without size())
   */
  BufferStream(StreamPtr input, size_t ADD = 32*1024)
    {
      assert(input);

      // Allocate memory, read the stream into it. Then call set()
      if(input->hasSize)
        {
          // We assume that we can get the position as well
          assert(input->hasPosition);

          // Calculate how much is left of the stream
          size_t left = input->size() - input->tell();

          // Allocate and read the data
          input->read(getWPtr(left), left);
        }
      else
        {
          // We DON'T know how big the stream is. We'll have to read
          // it in increments.

          std::vector<char> buf(ADD);
          while(!input->eof())
            {
              // Read one block of input data
              size_t read = input->read(&buf[0], ADD);

              // Write it to ourself
              write(&buf[0], read);

              // If we read less than expected, we should be at the
              // end of the stream
              assert(read == ADD || (read < ADD && input->eof()));
            }
        }

      // Reset the starting position to zero
      seek(0);
    }
};

typedef boost::shared_ptr<BufferStream> BufferStreamPtr;

}} // namespaces
#endif
