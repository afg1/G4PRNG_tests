

generators = "DualRand HepJamesRandom MersenneTwister Ranecu Ranshi Xor1024".split()
tests = "SmallCrush".split()# Crush BigCrush".split()
reverse = "0 1".split()


for gen in generators:
    for test in tests:
        for rev in reverse:
            i = 0
            with open("equidistSeeds", 'r') as seeds:
                for seed in seeds:
                    parline = "./G4PRNG_tests {0} {1} {2} {3} > {1}_{0}_{4}_rev={3}.txt".format(gen, test, seed.strip(), rev, i)
                    print(parline)
                    i += 1