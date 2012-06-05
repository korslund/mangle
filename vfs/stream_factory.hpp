#ifndef MANGLE_STREAM_FACTORY_H
#define MANGLE_STREAM_FACTORY_H

#include "../stream/stream.hpp"
#include <string>

namespace Mangle {
namespace VFS {

/** This generic interface may be used for anything that creates
    Streams from strings.
*/

class StreamFactory
{
 public:
  /// Virtual destructor
  virtual ~StreamFactory() {}

  /** Open a new data stream. Deleting the object (letting all the
      pointers to it go out of scope) must be enough to close it.

      Is allowed to return empty streams if the object could not be
      found/created.
  */
  virtual Stream::StreamPtr open(const std::string &name) = 0;
};
typedef boost::shared_ptr<StreamFactory> StreamFactoryPtr;
}} // namespaces
#endif
