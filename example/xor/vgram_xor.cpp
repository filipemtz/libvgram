
#include <wnn/VgRamNeuron.h>
#include <iostream>


class XorOutput : public VgRamNeuronOutput
{
public:
	int bit;
	XorOutput(int bbit = -1) { bit = bbit; }

	virtual int save(FILE *f) { return fprintf(f, "%d ", bit); }
	virtual int load(FILE *f) { return fscanf(f, "%d ", &bit); }
};


void
xor_test()
{
	VgRamNeuron v;

	BitPattern *b1 = new BitPattern(2);
	BitPattern *b2 = new BitPattern(2);
	BitPattern *b3 = new BitPattern(2);
	BitPattern *b4 = new BitPattern(2);

	b1->setZeros();
	b2->set(0);
	b3->set(1);
	b4->setOnes();

	v.train(b1, new XorOutput(0));
	v.train(b2, new XorOutput(1));
	v.train(b3, new XorOutput(1));
	v.train(b4, new XorOutput(0));

	std::cout << "Nearests: " << std::endl;
	std::cout << "B1: " << (*b1) << " Output: " << ((XorOutput*) v.nearests(b1).values[0])->bit << " Dist: " << v.nearests(b1).distance << std::endl;
	std::cout << "B2: " << (*b2) << " Output: " << ((XorOutput*) v.nearests(b2).values[0])->bit << " Dist: " << v.nearests(b2).distance << std::endl;
	std::cout << "B3: " << (*b3) << " Output: " << ((XorOutput*) v.nearests(b3).values[0])->bit << " Dist: " << v.nearests(b3).distance << std::endl;
	std::cout << "B4: " << (*b4) << " Output: " << ((XorOutput*) v.nearests(b4).values[0])->bit << " Dist: " << v.nearests(b4).distance << std::endl;

	std::cout << "Farthests: " << std::endl;
	std::cout << "B1: " << (*b1) << " Output: " << ((XorOutput*) v.farthests(b1).values[0])->bit << " Dist: " << v.farthests(b1).distance << std::endl;
	std::cout << "B2: " << (*b2) << " Output: " << ((XorOutput*) v.farthests(b2).values[0])->bit << " Dist: " << v.farthests(b2).distance << std::endl;
	std::cout << "B3: " << (*b3) << " Output: " << ((XorOutput*) v.farthests(b3).values[0])->bit << " Dist: " << v.farthests(b3).distance << std::endl;
	std::cout << "B4: " << (*b4) << " Output: " << ((XorOutput*) v.farthests(b4).values[0])->bit << " Dist: " << v.farthests(b4).distance << std::endl;

	FILE *f = fopen("xor.txt", "w");
	v.save(f);
	fclose(f);
}


void
xor_load_test()
{
	VgRamNeuron v;

	FILE *f = fopen("xor.txt", "r");
	v.load<XorOutput>(f);
	fclose(f);

	FILE *g = fopen("xor2.txt", "w");
	v.save(g);
	fclose(g);
}


int
main()
{
	xor_test();
	xor_load_test();
}
