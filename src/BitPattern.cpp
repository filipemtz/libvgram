
#include <cmath>
#include <ostream>
#include <cstdlib>
#include <ostream>
#include <cassert>
#include <cstring>
#include <cstdio>
#include <iostream>
#include <wnn/BitPattern.h>


long
BitPattern::_compute_num_slots(long size)
{
	long n = (size) / BITS_PER_SLOT;
	long r = (size) % BITS_PER_SLOT;

	if (r > 0)
		n += 1;

	return n;
}


long
BitPattern::_compute_slot(long bit_id)
{
	return bit_id / BITS_PER_SLOT;
}


long
BitPattern::_compute_shift(long bit_id)
{
	return bit_id % BITS_PER_SLOT;
}


void
BitPattern::_set_extra_bits_to_zero()
{
	long num_bits_outside_pattern = BITS_PER_SLOT - BitPattern::_compute_shift(_size);

	NEURON_MEMORY_TYPE shifted_right = ((NEURON_MEMORY_TYPE) _storage[_num_slots - 1]) << num_bits_outside_pattern;
	NEURON_MEMORY_TYPE shifted_back = ((NEURON_MEMORY_TYPE) shifted_right) >> num_bits_outside_pattern;

	// to set the bits out of the pattern limits to zero
	_storage[_num_slots - 1] = shifted_back;

}

void
BitPattern::_alloc(long size)
{
	_size = size;
	_num_slots = BitPattern::_compute_num_slots(_size);
	_storage = (NEURON_MEMORY_TYPE *) calloc(_num_slots, sizeof(NEURON_MEMORY_TYPE));
}


void
BitPattern::_resize(long new_size)
{
	long previous_num_slots = _num_slots;

	_size = new_size;
	_num_slots = BitPattern::_compute_num_slots(_size);
	_storage = (NEURON_MEMORY_TYPE *) realloc(_storage, _num_slots * sizeof(NEURON_MEMORY_TYPE));

	// set the new slots to zero
	for (long i = previous_num_slots; i < _num_slots; i++)
		_storage[i] = 0;
}


void
BitPattern::_copy_storage(const BitPattern &b)
{
	assert(_storage);
	assert(_num_slots == b._num_slots);

	memcpy(_storage, b._storage, b._num_slots * sizeof(NEURON_MEMORY_TYPE));
}


long
BitPattern::hamming(const BitPattern &a, const BitPattern &b)
{
	BitPattern c = (a ^ b);
	return c.count_ones();
}


BitPattern::BitPattern(long num_bits)
{
	_alloc(num_bits);
	setZeros();
}


BitPattern::BitPattern(const BitPattern &b)
{
	_alloc(b._size);
	_copy_storage(b);
}


BitPattern::~BitPattern()
{
	free(_storage);
}


BitPattern
BitPattern::operator!() const
{
	BitPattern out(_size);

	for (long i = 0; i < _num_slots; i++)
		out._storage[i] = ~_storage[i];

	out._set_extra_bits_to_zero();
	return out;
}


BitPattern
BitPattern::operator=(const BitPattern &b)
{
	if (b._size != _size)
		_resize(b._size);

	_copy_storage(b);
	return (*this);
}


BitPattern
BitPattern::operator^(const BitPattern &b) const
{
	if (_num_slots != b._num_slots)
		exit(printf("BitPattern::Error::operator^::Number of bits must be equals in the patterns (%ld != %ld)\n", size(), b.size()));

	BitPattern out(_size);

	for (long i = 0; i < _num_slots; i++)
		out._storage[i] = _storage[i] ^ b._storage[i];

	return out;
}


BitPattern
BitPattern::operator&(const BitPattern &b) const
{
	if (_num_slots != b._num_slots)
		exit(printf("BitPattern::Error::operator&::Number of bits must be equals in the patterns (%ld != %ld)\n", size(), b.size()));

	BitPattern out(_size);

	for (long i = 0; i < _num_slots; i++)
		out._storage[i] = _storage[i] & b._storage[i];

	return out;
}


BitPattern
BitPattern::operator|(const BitPattern &b) const
{
	if (_num_slots != b._num_slots)
		exit(printf("BitPattern::Error::operator|::Number of bits must be equals in the patterns (%ld != %ld)\n", size(), b.size()));

	BitPattern out(_size);

	for (long i = 0; i < _num_slots; i++)
		out._storage[i] = _storage[i] | b._storage[i];

	return out;
}


bool 
BitPattern::operator==(const BitPattern &b)
{
	if (b.size() != size())
	{
		printf("Warning::BitPattern::operator!=::Bit patterns of different sizes!\n");
		return false;
	}

	for (long i = 0; i < _num_slots; i++)
		if (_storage[i] != b._storage[i])
			return false;

	return true;
}


bool 
BitPattern::operator!=(const BitPattern &b)
{
	if (b.size() != size())
	{
		printf("Warning::BitPattern::operator!=::Bit patterns of different sizes!\n");
		return true;
	}

	for (long i = 0; i < _num_slots; i++)
		if (_storage[i] != b._storage[i])
			return true;

	return false;
}


std::ostream&
operator<<(std::ostream& os, const BitPattern &b)
{
	int dim = sqrt(b._size);

	//for (long i = 0; i < BITS_PER_SLOT * b._num_slotsb._size*/; i++)
	for (long i = 0; i < b._size; i++)
	{
		long sl = BitPattern::_compute_slot(i);
		long sh = BitPattern::_compute_shift(i);

		NEURON_MEMORY_TYPE l = ((NEURON_MEMORY_TYPE) 1) << sh;

		if ((b._storage[sl] & l) != 0)
			os << 1;
		else
			os << 0;

		if (i % dim == 0 && i != 0)
			os << "\n";
	}

	return os;
}


long
BitPattern::size() const
{
	return _size;
}


long
BitPattern::count_ones() const
{
	long n = 0;

	for (long i = 0; i < _num_slots; i++)
		n += __builtin_popcountl(_storage[i]);

	return n;
}


long
BitPattern::count_zeros() const
{
	return _size - count_ones();
}


void
BitPattern::set(long id)
{
	long sl = BitPattern::_compute_slot(id);
	long sh = BitPattern::_compute_shift(id);

	if (sl > _num_slots)
		exit(printf("BitPattern::Error::get::Bit out of bounds\n"));

	NEURON_MEMORY_TYPE l = ((NEURON_MEMORY_TYPE) 1) << sh;
	_storage[sl] = _storage[sl] | l;
}


void
BitPattern::unset(long id)
{
	long sl = BitPattern::_compute_slot(id);
	long sh = BitPattern::_compute_shift(id);

	if (sl > _num_slots)
		exit(printf("BitPattern::Error::get::Bit out of bounds\n"));

	NEURON_MEMORY_TYPE l = ((NEURON_MEMORY_TYPE) 1) << sh;
	_storage[sl] = _storage[sl] & (~l);
}


int
BitPattern::get(long id) const
{
	long sl = BitPattern::_compute_slot(id);
	long sh = BitPattern::_compute_shift(id);

	if (sl > _num_slots)
		exit(printf("BitPattern::Error::get::Bit out of bounds\n"));

	NEURON_MEMORY_TYPE l = ((NEURON_MEMORY_TYPE) 1) << sh;
	if (_storage[sl] & l) return 1;
	else return 0;
}


void
BitPattern::flip(long id)
{
	if (get(id)) unset(id);
	else set(id);
}


void
BitPattern::setOnes()
{
	for (long i = 0; i < _num_slots; i++)
	{
		_storage[i] = 0;
		_storage[i] = ~(_storage[i]);
	}

	_set_extra_bits_to_zero();
}


void
BitPattern::setZeros()
{
	for (long i = 0; i < _num_slots; i++)
		_storage[i] = 0;
}


long
BitPattern::hamming(const BitPattern &b) const
{
	return BitPattern::hamming(*this, b);
}


int 
BitPattern::save(FILE *f)
{
	fprintf(f, "%ld ", _size);

	for (int i = 0; i < _num_slots; i++)
		fprintf(f, "%ld ", _storage[i]);

	return 1;
}


int 
BitPattern::load(FILE *f)
{
	long num_bits;

	if (_storage)
		free(_storage);

	fscanf(f, "%ld ", &num_bits);	
	_alloc(num_bits);

	for (int i = 0; i < _num_slots; i++)
		fscanf(f, "%ld", &(_storage[i]));

	return 1;
}


