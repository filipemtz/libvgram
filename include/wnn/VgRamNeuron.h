
#ifndef __VGRAM_NEURON_H__
#define __VGRAM_NEURON_H__

#include <vector>
#include <cfloat>
#include <wnn/BitPattern.h>


class VgRamNeuronOutput
{
	public:
		VgRamNeuronOutput() {}
		~VgRamNeuronOutput() {}
};


typedef enum
{
	ReplacementStrategyRandom,
	ReplacementStrategyNearest,
	ReplacementStrategyFarFromAverage
}ReplacementStrategy;


class VgRamNeuronConfig
{
	public:

		long memory_size;
		ReplacementStrategy replacement;

		static VgRamNeuronConfig CreateDefaultConfig()
		{
			VgRamNeuronConfig c;

			c.memory_size = 100;
			c.replacement = ReplacementStrategyRandom;

			return c;
		}
};


class VgRamNeuronResult
{
	public:
		double distance;
		std::vector<int> ids;
		std::vector<VgRamNeuronOutput*> values;
};


class VgRamNeuron
{
	std::vector<BitPattern*> _patterns;
	std::vector<VgRamNeuronOutput*> _outputs;
	VgRamNeuronConfig _config;

	public:
		VgRamNeuron(VgRamNeuronConfig = VgRamNeuronConfig::CreateDefaultConfig());
		void train(BitPattern *b, VgRamNeuronOutput *o);
		void replace_output(int id, VgRamNeuronOutput *o);
		VgRamNeuronResult nearests(BitPattern *b);
		VgRamNeuronResult farthests(BitPattern *b);
		std::vector<float> average_pattern_as_float();
		BitPattern* average_pattern_as_bits();

};


#endif
