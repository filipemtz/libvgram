
#include <wnn/VgRamNeuron.h>
#include <cstdlib>
#include <cstdio>


VgRamNeuron::VgRamNeuron(VgRamNeuronConfig config)
{
	_config = config;
}


void
VgRamNeuron::train(BitPattern *b, VgRamNeuronOutput *o)
{
	if (_patterns.size() < _config.memory_size)
	{
		_patterns.push_back(b);
		_outputs.push_back(o);
	}
	else
	{
		int sample_to_replace = 0;

		if (_config.replacement == ReplacementStrategyRandom)
			sample_to_replace = rand() % _patterns.size();
		else if (_config.replacement == ReplacementStrategyNearest)
		{
			VgRamNeuronResult r = nearests(b);
			sample_to_replace = r.ids[rand() % r.ids.size()];
		}
		else if (_config.replacement == ReplacementStrategyFarFromAverage)
		{
			BitPattern *avg = average_pattern_as_bits();
			VgRamNeuronResult r = farthests(avg);
			sample_to_replace = r.ids[rand() % r.ids.size()];
			long d = b->hamming(*avg);
			delete(avg);

			// if the new pattern is further from the average than
			// the most distant pattern, do not train.
			if (d > r.distance || d == 0)
			{
				delete(b);
				delete(o);
				return;
			}
		}
		else exit(printf("Error::VgRamNeuron::train:: Unknown replacement method!\n"));

		delete(_patterns[sample_to_replace]);
		delete(_outputs[sample_to_replace]);

		_patterns[sample_to_replace] = b;
		_outputs[sample_to_replace] = o;
	}
}


void
VgRamNeuron::replace_output(int id, VgRamNeuronOutput *o)
{
	if (id < 0 || id >= _patterns.size())
		exit(printf("Error::replace_output::Trying to replace invalid Id: %d (table size: %ld)\n",
			id, _patterns.size()));

	delete(_outputs[id]);
	_outputs[id] = o;
}


VgRamNeuronResult
VgRamNeuron::nearests(BitPattern *b)
{
	long d;
	VgRamNeuronResult r;

	r.distance = DBL_MAX;

	for (int i = 0; i < _patterns.size(); i++)
	{
		d = b->hamming(*_patterns[i]);

		if (d < r.distance)
		{
			r.distance = d;

			r.ids.clear();
			r.values.clear();

			r.ids.push_back(i);
			r.values.push_back(_outputs[i]);
		}
		else if (d == r.distance)
		{
			r.ids.push_back(i);
			r.values.push_back(_outputs[i]);
		}
	}

	return r;
}


VgRamNeuronResult
VgRamNeuron::farthests(BitPattern *b)
{
	long d;
	VgRamNeuronResult r;

	r.distance = -DBL_MAX;

	for (int i = 0; i < _patterns.size(); i++)
	{
		d = b->hamming(*_patterns[i]);

		if (d > r.distance)
		{
			r.distance = d;

			r.ids.clear();
			r.values.clear();

			r.ids.push_back(i);
			r.values.push_back(_outputs[i]);
		}
		else if (d == r.distance)
		{
			r.ids.push_back(i);
			r.values.push_back(_outputs[i]);
		}
	}

	return r;
}


std::vector<float>
VgRamNeuron::average_pattern_as_float()
{
	if (_patterns.size() <= 0)
		exit(printf("Error::VgRamNeuron::average_pattern::Trying to get the average pattern from an empty neuron."));

	long i, j;
	long nbits = _patterns[0]->size();
	std::vector<float> average_bit_pattern;

	for (j = 0; j < nbits; j++)
	{
		average_bit_pattern.push_back(0.0);

		for (i = 0; i < _patterns.size(); i++)
			average_bit_pattern[j] += (float) _patterns[i]->get(j);

		if (_patterns.size() > 0)
			average_bit_pattern[j] /= (float) _patterns.size();
	}

	return average_bit_pattern;
}


BitPattern*
VgRamNeuron::average_pattern_as_bits()
{
	std::vector<float> avg = average_pattern_as_float();
	BitPattern *avg_pattern = new BitPattern(avg.size());

	for (int i = 0; i < avg.size(); i++)
		if (avg[i] > 0.5)
			avg_pattern->set(i);

	return avg_pattern;
}


int 
VgRamNeuron::save(const char *filename)
{
	FILE *f = fopen(filename, "w");

	if (f == NULL)
		exit(printf("Error::VgRamNeuron::save::Unable to open file '%s'\n", filename));

	_config.save(f);
	fprintf(f, "%ld\n", _patterns.size());

	for (size_t i = 0; i < _patterns.size(); i++)
	{
		_patterns[i]->save(f);
		_outputs[i]->save(f);
		fprintf(f, "\n");
	}

	fclose(f);
	return 1;
}




