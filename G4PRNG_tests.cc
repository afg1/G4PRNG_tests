#include "Randomize.hh"

// Include timing feature
#include <ctime>
#include <climits>
#include <fstream>
#include <iostream>
#include <thread>

// TestU01 stuff
extern "C"
{
#include "unif01.h"
#include "bbattery.h"
}


// My xorshift1024* generator
#include "G4Xorshift1024.hh"


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


void runTest(string test, unif01_Gen* gen)
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
        cout << "Meran time per random number: " << res.mean << " s"  << endl;
        cout << "(I get: " << res.time/1E9 << " s/number)" << endl;
        cout << "(or " << res.time << " ns/number)" << endl;
    }
}




int main(int argc, char** argv)
{
    unif01_Gen *gen;
    
    string selection(argv[1]);
    string test("SmallCrush");
    if(argc ==3)
    {
        test = string(argv[2]);
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
    }
    else if(selection == "DualRand")
    {        
        gen = unif01_CreateExternGen01("DualRand", Dual);
        runTest(test, gen);
        unif01_DeleteExternGen01(gen);
        
    }
    else if(selection == "HepJamesRandom")
    {
        
        gen = unif01_CreateExternGen01("HepJamesRandom", James);
        runTest(test, gen);
        unif01_DeleteExternGen01(gen);
        
    }
    else if (selection == "MersenneTwister")
    {
        
        gen = unif01_CreateExternGen01("MersenneTwister", Mersenne);
        runTest(test, gen);
        unif01_DeleteExternGen01(gen);
        
    }
    else if(selection == "Ranecu")
    {
        
        gen = unif01_CreateExternGen01("Ranecu", Ranecu);
        runTest(test, gen);
        unif01_DeleteExternGen01(gen);
        
    }
    else if(selection == "Ranlux64")
    {        
        gen = unif01_CreateExternGen01("Ranlux64", Ranlux64);
        runTest(test, gen);
        unif01_DeleteExternGen01(gen);
        
    }
    else if (selection == "Ranlux32")
    {
        
        gen = unif01_CreateExternGen01("Ranlux32", Ranlux32);
        runTest(test, gen);
        unif01_DeleteExternGen01(gen);
        
    }
    else if(selection =="Ranshi")
    {
        
        gen = unif01_CreateExternGen01("Ranshi", Ranshi);
        runTest(test, gen);
        unif01_DeleteExternGen01(gen);
        
    }
    else if(selection =="Xor1024")
    {
        
        gen = unif01_CreateExternGen01("Xor1024", Xor1024);
        runTest(test, gen);
        unif01_DeleteExternGen01(gen);
    }
    
    else
    {
        cerr << "Incorrect generator name supplied. Re-run with -h to see a list of available generators" << endl;
    }
    return 0;
}
