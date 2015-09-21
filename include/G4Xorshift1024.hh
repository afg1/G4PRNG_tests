#include <cstdint>
#include <climits>

class G4Xorshift1024
{
    public:
        G4Xorshift1024(uint64_t seed);
        G4Xorshift1024();
        ~G4Xorshift1024();
    
    
    private:
        uint64_t shoot64();
        uint64_t state[16];
        uint64_t state64;
        
    public:
        double flat();
        uint64_t shoot();
        void seed(uint64_t);
        unsigned p;
};
