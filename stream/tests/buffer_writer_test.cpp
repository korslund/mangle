#include <iostream>
#include "../servers/buffer_writer.hpp"

using namespace Mangle::Stream;
using namespace std;

void info(Stream &str)
{
  cout << "Pos/Size: " << str.tell() << " / " << str.size();
  cout << "   Value: '" << string((const char*)str.getPtr(), str.size()) << "'\n\n";
}

void write(Stream &str, const std::string &msg)
{
  cout << "Writing '" << msg << "' (" << msg.size() << " bytes)\n";
  str.write(msg.c_str(), msg.size());
}

BufferWriter wrt;

void test(const std::string &msg)
{
  write(wrt, msg);
  info(wrt);
}

void testPos(const std::string &msg, int pos)
{
  cout << "Pointer-writing '" << msg << "' at position " << pos << endl;
  char *p = (char*)wrt.getWPtr(pos, msg.size());
  msg.copy(p, msg.size());
  info(wrt);
}

int main()
{
  assert(wrt.isReadable && wrt.isWritable && wrt.isSeekable &&
         wrt.hasPosition && wrt.hasSize && wrt.hasPtr);

  info(wrt);
  test("abc");
  test("def");
  test("1234567890");
  cout << "Seeking to 8\n";
  wrt.seek(8);
  test("blah");
  test("12345");
  testPos("ABCD", 1);
  testPos("9999", 17);
  test("xxx");

  return 0;
}
