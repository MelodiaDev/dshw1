/*
 * Please use CamelCase to define a variable. Such as "getPosAt", not "GetPosAt" or "get_pos_at"
 *
 * And note that allocate and free blocks of size bytes of memory when you insert or delete item from the list.
 */

#ifndef CIRCLE_H
#define CIRCLE_H

/*
 * special key
 */
#define ENTER 0
#define LEFT 1
#define RIGHT 2
#define UP 3
#define DOWN 4
#define ESC 5

/*
 * control command
 */
#define INTO_INSERT_I 'i'
#define INTO_INSERT_A 'a'
#define INSERT_LINE_o 'o'
#define INSERT_LINE_O 'O'
#define OUT_INSERT ESC
#define GO_LEFT 'h'
#define GO_RIGHT 'l'
#define GO_UP 'k'
#define GO_DOWN 'j'
#define DELETE 'd'
#define PASTE 'p'

/*
 * mode
 */
#define NORMAL 0
#define INSERT 1
#define COMMAND 2

template <class T>
class _list { // Container of the list class
	public:
	T value; 
	_list *next[2]; // next[0] means the next node, and next[1] means the previous node
	_list(_list* null = 0) {next[0] = next[1] = null;}
};

template <class T>
class List {
	private:

	typedef _list<T> _container;
	typedef _container* iterator;
	_container Tnull;

	/*
	 * null is a virtual item which is used to simply the border condition.
	 * Note that null->ch[0] point at the last item of the list ,and null->ch[1] point at the first item of the list.
	 * And null->ch[0] = null->ch[1] when the list is empty.
	 * You will fall in love with this definition :)
	 */
	iterator null;

	/*
	 * head point at the first item of the list.
	 * When the list is empty, head equals at null.
	 */
	iterator head;

	public:
	List() {null = &Tnull;}
	
	const iterator& begin() const {return head;}
	const iterator& end() const {return null;}

	/*
	 * Demo: get the index-th item from the begin of list
	 */
	const iterator& getPos(const int& index) const {
		iterator ret = head; int _index = index;
		while (ret->ch[1] != null && _index--) ret = ret->ch[1];
		return ret;
	}

	/*
	 * Count the number item of the list.
	 */
	unsigned int size(void) const {
	}

	/*
	 * Count the number of items before  the current item.
	 */
	unsigned int getWhere(const iterator& now) const {
	}

	/*
	 * Get the next bias-th item from the begin position, when bias is positive.
	 * And the direction is previous when bias is negative.
	 */
	const iterator& getPosAt(const iterator& begin, const int& bias) const {
	}

	/*
	 * Insert a item in the FRONT of the iterator "now"
	 * Return the iterator where the new item located
	 */
	const iterator& insert(const iterator& now, const T& value) {
	}


	/*
	 * Erase the item which located at the "now", and return the next iterator behind the "now"
	 */
	const iterator& erase( const iterator& now) {
	}

	/*
	 * Erase an interval from the begin to the from the end, "begin" inclusive and "end" NOT inclusive.
	 * Return the next iterator behind the deleted inteval
	 */
	const iterator& erase(const iterator& begin, const iterator& end) {
	}

	/*
	 * Find the first position where the pattern matched. Please use KMP algorithm.
	 * Note that do NOT just define a big array in a function, or it will stack overflow. 
	 * Use "static" keywords instead, and clear the array before every time used.
	 * Return the beginning of the first mathched. If there is no matched, just return null.
	 */
	const iterator& match(const List& pattern) {
	}
};

#endif
