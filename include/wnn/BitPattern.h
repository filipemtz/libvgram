
#ifndef __BIT_PATTERN_H__
#define __BIT_PATTERN_H__

#include <ostream>
#include <cstdio>

#define NEURON_MEMORY_TYPE unsigned long
#define BITS_PER_SLOT (8 * sizeof(NEURON_MEMORY_TYPE))

class BitPattern
{
	long _size;
	long _num_slots;
	NEURON_MEMORY_TYPE *_storage;

	static long _compute_num_slots(long size);
	static long _compute_slot(long bit_id);
	static long _compute_shift(long bit_id);

	void _set_extra_bits_to_zero();

	void _alloc(long size);
	void _resize(long new_size);
	void _copy_storage(const BitPattern &b);

public:

	static long hamming(const BitPattern &a, const BitPattern &b);

	BitPattern(long num_bits = BITS_PER_SLOT);
	BitPattern(const BitPattern &b);
	~BitPattern();

	BitPattern operator!() const;
	BitPattern operator=(const BitPattern &b);
	BitPattern operator^(const BitPattern &b) const;
	BitPattern operator&(const BitPattern &b) const;
	BitPattern operator|(const BitPattern &b) const;
	bool operator==(const BitPattern &b);
	bool operator!=(const BitPattern &b);

	friend std::ostream& operator<<(std::ostream& os, const BitPattern &b);

	long size() const;
	long count_ones() const;
	long count_zeros() const;
	void set(long id);
	void unset(long id);
	int get(long id) const;
	void flip(long id);
	void setOnes();
	void setZeros();
	long hamming(const BitPattern &b) const;
	// return 0 if failed, and a number different of 0 if succeded.
	int save(FILE *f);
	int load(FILE *f);
};


#endif
