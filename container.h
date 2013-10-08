#include <string>

#ifndef CIRCLE_H
#define CIRCLE_H

#define TAB_WIDTH 4

/*
 * Please use CamelCase to define a variable. Such as "getPosAt", not "GetPosAt" or "get_pos_at"
 *
 * And note that allocate and free blocks of size bytes of memory when you insert or delete item from the editor_list.
 */

template <class T>
class _editor_list { // Container of the list class
	public:
	T value; 
	int sum;
	_editor_list *ch[2]; // next[0] means the next node, and next[1] means the previous node
	_editor_list(_editor_list* null = 0) {ch[0] = ch[1] = null; sum = 0;}
};

template <class T>
class editor_list {
	private:

	typedef _editor_list<T> _container;
	_container Tnull;

	public:
	typedef _container* iterator;

	bool operator ==(const int& oth) const {if (oth == '\t') return false; return false;}

	private:
	/*
	 * null is a virtual item which is used to simply the border condition.
	 * Note that null->ch[0] point at the last item of the editor_list ,and null->ch[1] point at the first item of the list.
	 * And null->ch[0] = null->ch[1] when the editor_list is empty.
	 * You will fall in love with this definition :)
	 */
	iterator null;

	/*
	 * head point at the first item of the editor_list.
	 * When the editor_list is empty, head equals at null.
	 */
	iterator head;

	public:
	editor_list() {head = null = &Tnull; null->ch[0] = null->ch[1] = null;}
	editor_list(T* l, T* r) {
		head = null = &Tnull;
		null->ch[0] = null->ch[1] = null;
		T* _l = l, *_r = r;
		while (_l != _r) {
			T now = *_l;
			_container *tmp = new _container;
			tmp->value = now;
			tmp->ch[1] = null;
			tmp->ch[0] = null->ch[0];
			null->ch[0]->ch[1] = tmp;
			null->ch[0] = tmp;
			_l++;
			if (_l == l) head = tmp;
		}
	}
	
	iterator begin() const {return head;}
	iterator end() const {return null;}

	/*
	 * Demo: get the index-th item from the begin of editor_list
	 */
	iterator getPos(const int& index) const;

	/*
	 * Count the number item of the editor_list.
	 */
	unsigned int size(void) const;

	/*
	 * Count the number of items before  the current item.
	 */
	unsigned int getWhere(const iterator& now) const;

	/*
	 * Get the next bias-th item from the begin position, when bias is positive.
	 * And the direction is previous when bias is negative.
	 */
	iterator getPosAt(const iterator& begin, const int& bias) const;

	/*
	 * Insert a item in the FRONT of the iterator "now"
	 * Return the iterator where the new item located
	 */
	iterator insert(const iterator& now, const T& value);


	/*
	 * Erase the item which located at the "now", and return the next iterator behind the "now"
	 */
	iterator erase( const iterator& now);

	/*
	 * Erase an interval from the begin to the from the end, "begin" inclusive and "end" NOT inclusive.
	 * Return the next iterator behind the deleted inteval
	 */
	iterator erase(const iterator& begin, const iterator& end) ;

	/*
	 * Find the first position where the pattern matched. Please use KMP algorithm.
	 * Note that do NOT just define a big array in a function, or it will stack overflow. 
	 * Use "static" keywords instead, and clear the array before every time used.
	 * Return the beginning of the first mathched. If there is no matched, just return null.
	 */
	iterator match(const std::string& pattern, const iterator& begin);

	/*
	 * clear the list
	 */
	void clear(void);

	/*
	 * Get the next iterator
	 */
	iterator Next(const iterator&);

	/*
	 * Get the previous iterator
	 */
	iterator Prev(const iterator&);

	/*
	 * Update the sum from now
	 */
	void Update(iterator tmp) {
		int t = tmp->ch[0]->sum;
		for (; tmp != null; tmp = tmp->ch[1]) {
			if (tmp->value == '\t') {
				t = (t + TAB_WIDTH) / TAB_WIDTH * TAB_WIDTH;
			} else t++;
			tmp->sum = t;
		}
	}
};

template<class T>
typename editor_list<T>::iterator editor_list<T>::getPos(const int& index) const {
	iterator ret = head;
	for (int i = 0; i < index && ret != null; i++) ret = ret->ch[1];
	return ret;
}

template<class T>
unsigned int editor_list<T>::size(void) const {
	unsigned int ret = 0;
	iterator tmp = head;
	while (tmp != null) tmp = tmp->ch[1], ret++;
	return ret;
}

template<class T>
unsigned int editor_list<T>::getWhere(const iterator& now) const {
	int ret = 0;
	iterator tmp = now;
	while (tmp->ch[0] != null) tmp = tmp->ch[0], ret++;
	return ret;
}

template<class T>
typename editor_list<T>::iterator editor_list<T>::getPosAt(const typename editor_list<T>::iterator& begin, const int &bias) const {
	iterator ret = begin; int dir = 1, b = bias; if (bias < 0) b = -bias, dir = 0;
	for (int i = 0; i < b && ret->ch[dir] != null; i++) ret = ret->ch[dir];
	return ret;
}

template<class T>
typename editor_list<T>::iterator editor_list<T>::insert(const typename editor_list<T>::iterator& now, const T& value) {
	iterator ret = new _container(null);
	ret->value = value;
	ret->ch[1] = now;
	ret->ch[0] = now->ch[0];
	now->ch[0]->ch[1] = ret;
	now->ch[0] = ret;
	if (now == head) head = ret;
	Update(ret);
	return ret;
}

template<class T>
typename editor_list<T>::iterator editor_list<T>::erase(const typename editor_list<T>::iterator &begin) {
	iterator ret = begin->ch[1];
	begin->ch[0]->ch[1] = begin->ch[1];
	begin->ch[1]->ch[0] = begin->ch[0];
	if (begin == head) head = begin->ch[1];
	delete begin;
	Update(ret);
	return ret;
}

template<class T>
typename editor_list<T>::iterator editor_list<T>::erase(const typename editor_list<T>::iterator &begin, const typename editor_list<T>::iterator& end) {
	begin->ch[0]->ch[1] = end;
	end->ch[0] = begin->ch[0];
	iterator tmp = begin, _t;
	while (tmp != end) {
		_t = tmp->ch[1];
		delete tmp;
		tmp = _t;
	}
	if (begin == head) head = end;
	Update(end);
	return end;
}

template<class T>
typename editor_list<T>::iterator editor_list<T>::match(const std::string& pattern, const iterator& begin) {
	int *next = new int[pattern.length()], j = -1; next[0] = -1;
	for (int i = 1; i < pattern.length(); i++) {
		while (j != -1 && pattern[i] != pattern[j + 1]) j = next[j];
		if (pattern[i] == pattern[j + 1]) j++;
		next[i] = j;
	}
	iterator tmp = begin; j = -1;
	while (tmp != null) {
		while (j != -1 && tmp->value != pattern[j + 1]) j = next[j];
		if (tmp->value == pattern[j + 1]) j++;
		if (j + 1 == pattern.length()) {
			for (int i = 1; i < pattern.length(); i++) tmp = tmp->ch[0];
			return tmp;
		}
		tmp = tmp->ch[1];
	}
	return null;
}

template<class T>
void editor_list<T>::clear(void) {
	iterator it = head, tmp;
	for (; it != null; it = it->ch[1]) {
		tmp = it->ch[1];
		delete it;
		it = tmp;
	}
}

template<class T>
typename editor_list<T>::iterator editor_list<T>::Next(const typename editor_list<T>::iterator& now) {
	return now->ch[1];
}

template<class T>
typename editor_list<T>::iterator editor_list<T>::Prev(const typename editor_list<T>::iterator& now) {
	return now->ch[0];
}

#endif
