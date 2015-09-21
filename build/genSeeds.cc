#include <climits>
#include <iostream>
#include <cstdint>

using std::cout;
using std::endl;


int main(void)
{
    uint64_t d = UINT64_MAX/100;
    for(int i=0; i < 101; i++)
    {
        cout << i*d << endl;
    }


    return 0;
}