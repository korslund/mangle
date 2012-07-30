#include <iostream>
#include <string.h>

#include "../servers/memory_stream.hpp"
#include "../servers/string_writer.hpp"
#include "../clients/copy_stream.hpp"

using namespace Mangle::Stream;
using namespace std;

void test(Stream *inp, size_t lim=0)
{
  string out;

  cout << "lim=" << lim << " size=" << inp->size()
       << " pos=" << inp->tell() << " left="
       << (inp->size() - inp->tell());
  lim = CopyStream::copy(StreamPtr(inp), StringWriter::Open(out), lim);
  cout << " wrote=" << lim << " result='" << out << "'\n";
}

int main()
{
  Stream *s = new MemoryStream("hello world", 11);
  test(s);
  s = new MemoryStream("hello world", 11);
  s->hasPtr = false;
  test(s);
  s = new MemoryStream("hello world", 11);
  s->hasSize = false;
  s->hasPtr = false;
  test(s);
  s = new MemoryStream("hello world", 11);
  s->hasSize = false;
  s->hasPtr = false;
  s->seek(4);
  test(s);
  s = new MemoryStream("hello world", 11);
  s->seek(4);
  test(s);
  s = new MemoryStream("hello world", 11);
  s->hasSize = false;
  s->hasPtr = false;
  s->seek(1);
  test(s,6);
  s = new MemoryStream("hello world", 11);
  s->hasPtr = false;
  s->seek(1);
  test(s,6);
  s = new MemoryStream("hello world", 11);
  s->seek(1);
  test(s,6);

  return 0;
}
