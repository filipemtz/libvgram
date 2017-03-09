
#ifndef __VGRAM_NEURON_H__
#define __VGRAM_NEURON_H__

#include <vector>
#include <cfloat>
#include <wnn/BitPattern.h>
#include <cstdio>


class VgRamNeuronOutput
{
	public:
		VgRamNeuronOutput() {}
		~VgRamNeuronOutput() {}
		// return 0 if failed, and a number different of 0 if succeded.
		virtual int save(FILE *) = 0;
		virtual int load(FILE *) = 0;
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

		int save(FILE *f)
		{
			fprintf(f, "%ld %d ", memory_size, (int) replacement);
			return 1;
		}

		int load(FILE *f)
		{
			int repl;
			fscanf(f, "%ld %d ", &memory_size, &repl);
			replacement = (ReplacementStrategy) repl;
			return 1;
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
		void replace_training_sample(int id, BitPattern *b, VgRamNeuronOutput *o);
		VgRamNeuronResult nearests(BitPattern *b);
		VgRamNeuronResult farthests(BitPattern *b);
		std::vector<float> average_pattern_as_float();
		BitPattern* average_pattern_as_bits();
		long NumTrainedPatterns() { return _patterns.size(); }
		// return 0 if failed, and a number different of 0 if succeded.
		int save(FILE *f);

		template<class OutputType>
		int load(FILE *f)
		{
			_config.load(f);
	
			size_t n_patterns;
			fscanf(f, "%ld ", &n_patterns);

			for (size_t i = 0; i < n_patterns; i++)
			{
				_patterns.push_back(new BitPattern());
				_outputs.push_back((VgRamNeuronOutput*) new OutputType());
				_patterns[i]->load(f);
				_outputs[i]->load(f);
			}

			return 1;
		}

};


#endif
