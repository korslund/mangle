#include <iostream>
#include <string.h>

#include "../filters/cap_stream.hpp"
#include "../servers/memory_stream.hpp"

using namespace Mangle::Stream;
using namespace std;

void test(StreamPtr inp)
{
  cout << "Size: " << inp->size() << endl;
  cout << "Pos: " << inp->tell() << endl;
  cout << "Eof: " << inp->eof() << endl;
  char data[6];

  while(!inp->eof())
    {
      memset(data, 0, 6);
      cout << "\nReading " << inp->read(data, 5) << " bytes\n";
      cout << "Result: '" << data << "'\n";
      cout << "Pos: " << inp->tell() << endl;
      cout << "Eof: " << inp->eof() << endl;
    }
}

int main()
{
  StreamPtr orig(new MemoryStream("this is a test string!", 22));
  cout << "\nEntire stream:\n--------\n";
  test(orig);

  orig->seek(4);
  CapStreamPtr cap(new CapStream(orig,12));
  cout << "\nCapped stream:\n--------\n";
  test(cap);

  // Test reusability & exact read size (5 bytes)
  orig->seek(8);
  cap->setStream(orig, 5);
  cout << "\nAgain:\n--------\n";
  test(cap);

  return 0;
}
