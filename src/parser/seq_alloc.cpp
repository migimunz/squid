#include "seq_alloc.hpp"
#include <stdexcept>

namespace squid
{

memory_bin::memory_bin(int size)
	:size(size),
	 next(0),
	 memory(NULL)
{
	memory = new byte[size];
}

memory_bin::~memory_bin()
{
	delete[] memory;
}

bool memory_bin::can_allocate(int amount)
{
	return (size - (next + amount)) >= 0;
}

byte *memory_bin::allocate(int amount)
{
	if(can_allocate(amount))
	{
		byte *ret_addr = memory + next;
		next += amount;
		return ret_addr;
	}
	else
	{
		return NULL;
	}
}

seq_alloc::seq_alloc(int bin_size)
	:memory_bins(),
	 bin_size(bin_size)
{
}

byte *seq_alloc::_alloc(int amount)
{
	if(amount > bin_size)
	{
		throw std::runtime_error("Bin size too small.");
	}
	if(memory_bins.empty() || !memory_bins.back()->can_allocate(amount))
	{
		memory_bins.push_back(memory_bin_ptr(new memory_bin(bin_size)));
	}
	return memory_bins.back()->allocate(amount);
}

}