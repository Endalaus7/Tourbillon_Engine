#pragma once

#include "common.hpp"
#include <memory>

struct TBListMem
{
	std::shared_ptr<TBListMem> prev = nullptr;
	std::shared_ptr<TBListMem> next = nullptr;
	uint32_t level = 0;
};

template<typename T>
class TBList
{
private:
	size_t _size = 0; 
	std::shared_ptr<TBListMem> _head = nullptr;
	std::shared_ptr<TBListMem> _last = nullptr;
public:
	FORCE_INLINE const size_t size() { return _size; }
	FORCE_INLINE const std::shared_ptr<TBListMem> head() { return _head; }
	FORCE_INLINE const std::shared_ptr<TBListMem> last() { return _last; }
public:
	~TBList()
	{
		return;
	}
	void add_tail(std::shared_ptr<T> vnewlink)
	{
		std::shared_ptr<TBListMem> tb_newlink = std::static_pointer_cast<TBListMem>(vnewlink);
		if (!tb_newlink)return;

		tb_newlink->next = nullptr;
		tb_newlink->prev = _last;

		_head = _head ? _head : tb_newlink;
		if (_last)
			_last->next = tb_newlink;
		_last = tb_newlink;
		_size++;
	}


	void insert_after(std::shared_ptr<T> vprevlink, std::shared_ptr<T> vnewlink)
	{

	}
};