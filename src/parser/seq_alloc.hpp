#pragma once
#include <vector>
#include <memory>
#include <new>

namespace squid
{

typedef unsigned char byte;

class memory_bin
{
	int size;
	int next;
	byte *memory;
public:
	memory_bin(int size);
	~memory_bin();
	byte *allocate(int amount);
	bool can_allocate(int amount);
};

typedef std::unique_ptr<memory_bin> memory_bin_ptr;
typedef std::vector<memory_bin_ptr> memory_bin_list;

class seq_alloc
{
	memory_bin_list memory_bins;
	int bin_size;

public:
	seq_alloc(int bin_size = 4096); //4Kb default bin size
	byte *_alloc(int amount);

	template<class Class>
	Class *alloc() 
	{ 
		return new(_alloc(sizeof Class)) Class();
	}

	template<class Class, class A1>
	Class *alloc(A1 a1) 
	{ 
		return new(_alloc(sizeof Class)) Class(a1);
	}

	template<class Class, class A1, class A2>
	Class *alloc(A1 a1, A2 a2) 
	{ 
		return new(_alloc(sizeof Class)) Class(a1, a2);
	}

	template<class Class, class A1, class A2, class A3>
	Class *alloc(A1 a1, A2 a2, A3 a3) 
	{ 
		return new(_alloc(sizeof Class)) Class(a1, a2, a3);
	}

	template<class Class, class A1, class A2, class A3, class A4>
	Class *alloc(A1 a1, A2 a2, A3 a3, A4 a4) 
	{ 
		return new(_alloc(sizeof Class)) Class(a1, a2, a3, a4);
	}
};

}