#include "Randomize.hh"

// Include timing feature
#include <ctime>
#include <climits>
#include <fstream>
#include <iostream>
#include <sstream>


// TestU01 stuff
extern "C"
{
#include "unif01.h"
#include "bbattery.h"
}



void Reverse(uint64_t* theInt)
{
    // Taken from the bit-twiddling guide here: http://graphics.stanford.edu/~seander/bithacks.html
    // This function reversed the bits in an unsigned 64 bit integer, for use performing tests on the reverse generators
//    unsigned int s = sizeof(*theInt)*std::numeric_limits<uint64_t>::digits;
//    unsigned int mask = ~0;
//    while ((s >>= 1) > 0)
//    {
//        mask ^= (mask << s);
//        *theInt = ((*theInt >> s) & mask) | ((*theInt << s) & ~mask );
//    }
    
    uint64_t v = *theInt;
    uint64_t r = v;
    int s = sizeof(v)*CHAR_BIT;//std::numeric_limits<uint64_t>::digits;
    for(v >>= 1; v; v >>=1)
    {
        r <<= 1;
        r |= v &1;
        s --;
    }
    r <<= s;
    
    *theInt = r;
    
}

void GenerateUniformSeeds(uint64_t* seedArray)
{
    // This function generates seeds uniformly spaced between 0 & 2^64 for testing
    uint64_t increment = UINT64_MAX/100;
    for(int i=0; i< 100; i++)
    {
        seedArray[i] = i*increment;
    }
}


// My xorshift1024* generator
#include "G4Xorshift1024.hh"


static const double max_uint64_t = static_cast<double>(UINT64_MAX);//std::numeric_limits<uint64_t>::max());


using namespace std;
using namespace CLHEP;

DualRand* dual = new DualRand();
HepJamesRandom* hjr = new HepJamesRandom();
MTwistEngine* mt = new MTwistEngine();
RanecuEngine* rce = new RanecuEngine();
Ranlux64Engine* rl64 = new Ranlux64Engine();
RanluxEngine* rl32 = new RanluxEngine();
RanshiEngine* rshi = new RanshiEngine();
G4Xorshift1024* xor1024 = new G4Xorshift1024();

double Dual(void)
{
    return dual->flat();
}

double James(void)
{
    return hjr->flat();
}

double Mersenne(void)
{
    return mt->flat();
}
double Ranecu(void)
{
    return rce->flat();
}
double Ranlux64(void)
{
    return rl64->flat();
}

double Ranlux32(void)
{
    return rl32->flat();
}
double Ranshi(void)
{
    return rshi->flat();
}

double Xor1024(void)
{
    return xor1024->flat();
}

// Reversed generators
double DualRev(void)
{
    uint64_t v = dual->operator unsigned int();
    Reverse(&v);
//    double max = static_cast<double>(std::numeric_limits<uint64_t>::max());
    return static_cast<double>(v)/max_uint64_t;
}

double JamesRev(void)
{
    uint64_t v = hjr->operator unsigned int();
    Reverse(&v);
//    double max = static_cast<double>(std::numeric_limits<uint64_t>::max());
    return static_cast<double>(v)/max_uint64_t;}

double MersenneRev(void)
{
    uint64_t v = mt->operator unsigned int();
    Reverse(&v);
//    double max = static_cast<double>(std::numeric_limits<uint64_t>::max());
    return static_cast<double>(v)/max_uint64_t;}
double RanecuRev(void)
{
    uint64_t v = rce->operator unsigned int();
    Reverse(&v);
//    double max = static_cast<double>(std::numeric_limits<uint64_t>::max());
    return static_cast<double>(v)/max_uint64_t;}
double Ranlux64Rev(void)
{
    uint64_t v = rl64->operator unsigned int();
    Reverse(&v);
//    double max = static_cast<double>(std::numeric_limits<uint64_t>::max());
    return static_cast<double>(v)/max_uint64_t;}

double Ranlux32Rev(void)
{
    uint64_t v = rl32->operator unsigned int();
    Reverse(&v);
//    double max = static_cast<double>(std::numeric_limits<uint64_t>::max());
    return static_cast<double>(v)/max_uint64_t;}
double RanshiRev(void)
{
    uint64_t v = rshi->operator unsigned int();
    Reverse(&v);
//    double max = static_cast<double>(std::numeric_limits<uint64_t>::max());
    return static_cast<double>(v)/max_uint64_t;}

double Xor1024Rev(void)
{
    uint64_t v = xor1024->shoot();
    Reverse(&v);
    
    return static_cast<double>(v)/max_uint64_t;}


void runTest(string test, unif01_Gen* gen, int indTest)
{
    if(indTest < 0)// Want to run whole tests
    {
        if(test == "SmallCrush")
        {
            bbattery_SmallCrush(gen);
        }
        else if(test == "Crush")
        {
            bbattery_Crush(gen);
        }
        else if(test == "BigCrush")
        {
            bbattery_BigCrush(gen);
        }
        else if(test ==  "Timer")
        {
            unif01_TimerRec res;
            res.mean = -1;
            int giga = int(1E9);
            unif01_TimerGen(gen, &res, giga, 1);
            cout << "Testing " << gen->name << " speed..." << endl;
            cout << "Time for 1E9 randoms: " << res.time << " s" << endl;
            cout << "Mean time per random number: " << res.mean << " s"  << endl;
            cout << "(I get: " << res.time/1E9 << " s/number)" << endl;
            cout << "(or " << res.time << " ns/number)" << endl;
        }
    }
    else// Run a specific test from bigCrush
    {
        int rep[106] = {0};
        rep[indTest] = 1;
        bbattery_RepeatBigCrush(gen, rep);
    }
}




int main(int argc, char** argv)
{
    unif01_Gen *gen;
    
    string selection(argv[1]);
    string test("SmallCrush");
    uint64_t seed;
    int indTest(-1);
    bool reverse;
    if(argc >= 5)
    {
        test = string(argv[2]);
        stringstream input(argv[3]);
        input >> seed;
        if(atoi(argv[4]) == 1)
        {
            reverse = true;
        }
        else
        {
            reverse = false;
        }
        if(argc == 6)
        {
            indTest = atoi(argv[5]);
        }
    }
    
    
    cout << test << " test selected..." << endl;
    
    if(selection == "-h")
    {
        cout << "Possible Generator Names:" << endl;
        cout << "DualRand" << endl;
        cout << "HepJamesRandom" << endl;
        cout << "MersenneTwister" << endl;
        cout << "Ranecu" << endl;
        cout << "Ranlux64" << endl;
        cout << "Ranlux32" << endl;
        cout << "Ranshi" << endl;
        cout << "Xor1024" << endl;
        
        cout <<"---------------------------" << endl << endl;
        
        cout << "Possible tests:" << endl;
        cout << "SmallCrush (~30s)" << endl;
        cout << "Crush (~1h)" << endl;
        cout << "BigCruch (~8h)" << endl;
        cout << "Timer (~2m)" << endl;
        
        cout <<"---------------------------" << endl << endl;
        
        cout << "Other Options:" << endl;
        cout << "Seed: A uint64_t to seed the generator with" << endl;
        cout << "Reverse: Give 1 for reverse tests, 0 for normal" << endl;
        
        cout <<"---------------------------" << endl << endl;

        cout << "Usage: " << argv[0] << " <Generator> <Test> <Seed> <Reverse?>" << endl;
    }
    else if(selection == "DualRand")
    {        
        dual->setSeed(seed, 1);
        if(reverse)
        {
            gen = unif01_CreateExternGen01("DualRand_Reverse", DualRev);
        }
        else
        {
            gen = unif01_CreateExternGen01("DualRand", Dual);
        }
        
        runTest(test, gen, indTest);
        unif01_DeleteExternGen01(gen);
        
    }
    else if(selection == "HepJamesRandom")
    {
        hjr->setSeed(seed);
        if(reverse)
        {
            gen = unif01_CreateExternGen01("HepJamesRandom_Reverse", JamesRev);
        }
        else
        {
            gen = unif01_CreateExternGen01("HepJamesRandom", James);
        }
        runTest(test, gen, indTest);
        unif01_DeleteExternGen01(gen);
        
    }
    else if (selection == "MersenneTwister")
    {
        mt->setSeed(seed, 1);
        if(reverse)
        {
            gen = unif01_CreateExternGen01("MersenneTwister_Reverse", MersenneRev);
        }
        else
        {
            gen = unif01_CreateExternGen01("MersenneTwister", Mersenne);
        }
        runTest(test, gen, indTest);
        unif01_DeleteExternGen01(gen);
        
    }
    else if(selection == "Ranecu")
    {
        rce->setSeed(seed);
        if(reverse)
        {
            gen = unif01_CreateExternGen01("Ranecu_Reverse", RanecuRev);
        }
        else
        {
            gen = unif01_CreateExternGen01("Ranecu", Ranecu);
        }
        runTest(test, gen, indTest);
        unif01_DeleteExternGen01(gen);
        
    }
    else if(selection == "Ranlux64")
    {     
        rl64->setSeed(seed);
        if(reverse)
        {
            gen = unif01_CreateExternGen01("Ranlux64_Reverse", Ranlux64Rev);
        }
        else
        {
            gen = unif01_CreateExternGen01("Ranlux64", Ranlux64);
        }
        runTest(test, gen, indTest);
        unif01_DeleteExternGen01(gen);
        
    }
    else if (selection == "Ranlux32")
    {
        rl32->setSeed(seed);
        if(reverse)
        {
            gen = unif01_CreateExternGen01("Ranlux32_Reverse", Ranlux32Rev);
        }
        else
        {
            gen = unif01_CreateExternGen01("Ranlux32", Ranlux32);
        }
        runTest(test, gen, indTest);
        unif01_DeleteExternGen01(gen);
        
    }
    else if(selection =="Ranshi")
    {
        rshi->setSeed(seed, 1);
        if(reverse)
        {
            gen = unif01_CreateExternGen01("Ranshi_Reverse", RanshiRev);
        }
        else
        {
            gen = unif01_CreateExternGen01("Ranshi", Ranshi);
        }
        runTest(test, gen, indTest);
        unif01_DeleteExternGen01(gen);
        
    }
    else if(selection =="Xor1024")
    {
        xor1024->seed(seed);
        if(reverse)
        {
            gen = unif01_CreateExternGen01("Xor1024_Reverse", Xor1024Rev);
        }
        else
        {
            gen = unif01_CreateExternGen01("Xor1024", Xor1024);
        }
        runTest(test, gen, indTest);
        unif01_DeleteExternGen01(gen);
    }
    
    else
    {
        cerr << "Incorrect generator name supplied. Re-run with -h to see a list of available generators" << endl;
    }
    
    return 0;
}
