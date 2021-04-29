
#define ENABLE_TEMP

#ifdef ENABLE_TEMP

#include <cstdint>
#include <cstdlib>


class Stream // TODO: temp
{
public:
    bool available();
    uint8_t read();
    bool write(uint8_t*, size_t);
    bool write(uint8_t);
};

#endif
