#pragma once
#include <memory>
template <typename T>
struct ListNode
{
	T data{};
	std::shared_ptr<ListNode<T>> next{};
	ListNode() = default;
	ListNode(const T& data, std::shared_ptr<ListNode<T>> nxt = nullptr);
};

template <typename T>
ListNode<T>::ListNode(const T& d, std::shared_ptr<ListNode<T>> nxt)
	:next{nxt},
	data{d}
{}

template <typename T>
std::shared_ptr<ListNode<T>> Find(std::shared_ptr<ListNode<const T>> head, const T& key)
{
	auto curr = head;
	for (; curr && curr->data != key; curr = curr->next)
	{ }
	return curr;
}

/*
* 1. We need to store the previous.
* 2. The head could be the key to delete so we need to possibly return new_head
*/
template <typename T>
std::shared_ptr<ListNode<T>> Delete(std::shared_ptr<ListNode<T>> head, const T& key)
{
	std::shared_ptr<ListNode<T>> prev = nullptr;
	auto curr = head;
	auto new_head = head;
	for (; curr && curr->data != key; curr = curr->next)
	{
		prev = curr;
	}

	//I didnt find key in list
	if (!curr)
		return new_head;

	if (!prev)
	{
		new_head = head->next;
	}
	else
	{
		prev->next = curr->next;
	}

	return new_head;
}

template<typename T>
std::shared_ptr<ListNode<T>> Reverse(std::shared_ptr<ListNode<T>> head)
{
	std::shared_ptr<ListNode<T>> prev{ nullptr };
	std::shared_ptr<ListNode<T>> next{ nullptr };
	std::shared_ptr<ListNode<T>> curr{head};

	while (curr)
	{
		next = curr->next;
		curr->next = prev;
		prev = curr;
		curr = next;
	}
	return prev;
}

template <typename T>
std::shared_ptr<ListNode<T>> MergeSortedList(std::shared_ptr<ListNode<const T>> first, std::shared_ptr<ListNode<const T>> second)
{
	/*
	* You need to return the head
	* Notice the while loop changes from the other merge which I use std::copy and check only one list in condition.
	* No extra space is used as we do not make any allocations.
	* Wort case runtime occurs when nodes are same length.
	* Best case is if one of the lists is empty.
	*/
	auto dummy = std::make_shared<ListNode<T>>();
	auto tail = dummy;

	while (first && second)
	{
		if (first->data < second->data)
		{
			tail->next = first;
			first = first->next;
		}
		else
		{
			tail->next = second;
			second = second->next;
		}
		tail = tail->next;
	}

	tail->next = first ? first : second;
	return dummy->next;
}

/*
* The head can possibly change if the i index is at the start of list
* My solution is to advance to the i while storing the previous node
* perform normal reverse and then fix up the next pointers
* Workout what the new head would be.
*/
template <typename T>
std::shared_ptr<ListNode<T>> ReverseSublist(std::shared_ptr<ListNode<T>> head, int left, int right)
{
	std::shared_ptr<ListNode<T>> prev{ nullptr };
	std::shared_ptr<ListNode<T>> curr{ head };
	std::shared_ptr<ListNode<T>> new_head{ head };

	int i = 1;
	while (i < left)
	{
		prev = curr;
		curr = curr->next;
		++i;
	}

	//Now we gotta reverse
	std::shared_ptr<ListNode<T>> tmp_prev{ nullptr };
	std::shared_ptr<ListNode<T>> tmp_next{ nullptr };
	while (i <= right)
	{
		tmp_next = curr->next;
		curr->next = tmp_prev;
		tmp_prev = curr;
		curr = tmp_next;
		++i;
	}

	if (prev)
	{
		prev->next->next = curr;
		prev->next = tmp_prev;
	}
	else
	{
		//If prev is null, make tmp_prev the new head
		head->next = curr;
		new_head = tmp_prev;
	}
	return new_head;
}

template <typename T>
std::shared_ptr<ListNode<T>> HasCycle(std::shared_ptr<ListNode<T>> head)
{
	auto slow = head, fast = head;
	std::shared_ptr<ListNode<T>> cycle{nullptr};
	int cycle_length = 0;
	/*
	* You made this mistake previously. The currect solution is as follows.
	* We need fast to be twice as advanced as slow but we cant simply say fast = fast->next->next as fast->next could be null.
	* So we start by setting slow and fast to head. 
	* We check for fast->next in while loop
	* We set fast to fast->next->next inside loop and we can now compare.
	*/
	while (slow && fast && fast->next)
	{
		fast = fast->next->next;
		if (slow == fast)
		{
			cycle = slow;
			//Counts number of elements in circular list by freezing one pointer and counting all iterations until u revisit
			do
			{
				++cycle_length;
				slow = slow->next;
			} while (slow != cycle);
			break;
		}
		slow = slow->next;
	}
	if (!cycle)
		return cycle;
	/*
	* We have a cycle and we know the length of cycle.
	* Increment one iter by length of cycle starting from head
	* increment both first and second iter and where the match is the start of the cycle.
	* Commit to memory
	*/
	auto iter1 = head;
	while (cycle_length--)
		iter1 = iter1->next;
	auto iter2 = head;
	while (iter1 != iter2)
	{
		iter1 = iter1->next;
		iter2 = iter2->next;
	}
	return iter1;
}
/*
* 1. WOrk out the lengths of both lists. Can put into a function.
* 2. Find the difference between both lists.
* 3. Advance the longer list by the diff. Can put into a function
* 4. Now walk both lists in lock step until you find a match
*/
template <typename T>
std::shared_ptr<ListNode<T>> FindIntersection(std::shared_ptr<ListNode<T>> list1, std::shared_ptr<ListNode<T>> list2)
{
	int list1size = 0, list2size = 0;
	//We can actually use one loop here to improve runtime.
	for (auto i = list1; i; i = i->next)
		++list1size;
	for (auto i = list2; i; i = i->next)
		++list2size;
	//which is the longer list?
	auto longer = (list1size > list2size) ? list1 : list2;
	//could do with an abs
	auto diff = (list1size > list2size) ? list1size - list2size : list2size - list1size;
	auto iter1 = longer;
	//could also use post decrement
	while (diff)
	{
		iter1 = iter1->next;
		--diff;
	}
	auto iter2 = longer == list1 ? list2 : list1;
	//Dint forget u can put the equality test in the while loop condition as we've done on a number of occassions.
	while (iter1 && iter2)
	{
		if (iter1 == iter2)
			return iter1;
		iter1 = iter1->next;
		iter2 = iter2->next;
	}
	return nullptr;
}

/*
* Deleting a node in a singly linked list requires a linear scan to identify the previous node before the target node.
* In a special case where the node to delete is not the tail, this can be done in O(1).
* The data in the node must be assignable.
*/

template <typename T>
void DeleteNotTail(std::shared_ptr<ListNode<T>> toDelete)
{
	toDelete->data = toDelete->next->data;
	toDelete->next = toDelete->next->next;
}

/*
* The instinctive play is to store a prev. It is important if you are searching for a node to delete.
* But given that we have a k, we can control our advance to land on one before the target and then we wont need prev
* but what happens if the list has one element and k is 1? or if k lands on the head? how would we know since we dont have a prev?
* We would know if iter2 is null after advancing
*/
template <typename T>
std::shared_ptr<ListNode<T>> RemoveKthLast(std::shared_ptr<ListNode<T>> head, int k)
{
	auto iter1 = head;
	auto iter2 = head;

	while (k--)
	{
		iter2 = iter2->next;
	}
	//If iter2 is null that means iter1 will remain at head and therefore head is the element to be deleted.
	if (!iter2)
		return head->next;

	//Will this get me to 1 before target?
	while (iter2->next)
	{
		iter2 = iter2->next;
		iter1 = iter1->next;
	}
	//iter1 is now 1 before target
	iter1->next = iter1->next->next;
	return head;
}

template <typename T>
std::shared_ptr<ListNode<T>> addTwoNumbers(std::shared_ptr<ListNode<T>> one, std::shared_ptr<ListNode<T>> two)
{
	std::shared_ptr<ListNode<T>> dummy{ new ListNode<T>{0} };
	auto tail = dummy;
	int carry = 0;
	while (one || two)
	{
		auto sum = carry;
		if (one)
		{
			sum += one->data;
			one = one->next;
		}
		if (two)
		{
			sum += two->data;
			two = two->next;
		}
		tail->next = std::make_shared<ListNode<T>>(sum % 10);
		carry = sum / 10;
		tail = tail->next;
	}
	if (carry)
		tail->next = std::make_shared<ListNode<T>>(carry);
	return dummy->next;
}

/*
* Will I ever need a new head? we shouldnt but lets find out.
* Typical solution is to use 2 pointers. So this is similar.
*/
template <typename T>
std::shared_ptr<ListNode<T>> RemoveDuplicatesSorted(std::shared_ptr<ListNode<T>> head)
{
	auto tail = head;
	while (tail)
	{
		auto next = head->next;
		while (next && (next->data == tail->data))
		{
			next = next->next;
		}
		tail->next = next;
		tail = next;
	}
	return head;
}

/*
* Input: head = [1,2,3,3,4,4,5]
* Output: [1,2,5]
* ANy item duplicated is throw away. 
* Start with the head and iterate until i find first unequal item. If current->next == first unequal item then current has no dupes. 
*/
template <typename T>
std::shared_ptr<ListNode<T>> RemoveKeyswithDuplicatesSorted(std::shared_ptr<ListNode<T>> head)
{
	std::shared_ptr<ListNode<T>> dummy = std::make_shared<ListNode<T>>();
	auto tail = dummy;
	auto current = head;
	while (current)
	{
		auto next = current->next;
		while (next && current->data == next->data)
		{
			next = next->next;
		}
		if (current->next == next)
		{
			tail->next = current;
			tail = tail->next;
		}
		current = next;
	}
	tail->next = nullptr;
	return dummy->next;
}

/*
* We had to draw a diagram to fix this.
* 1. Work out the tail
* 2. Work out the number of items in list say len.
* 3. Work out the len- kth item
* 4. point head to tail
* 5. Point len - kth item to nullptr
*/
template <typename T>
std::shared_ptr<ListNode<T>> RightShiftList(std::shared_ptr<ListNode<T>> head, int k)
{
	if (!head)
		return head;

	int len = 1;
	std::shared_ptr<ListNode<T>> tail = head;
	while (tail->next)
	{
		++len;
		tail = tail->next;
	}

	//k can be longer than len?
	k %= len;
	if (!k)
		return head;

	tail->next = head;
	int steps_to_new_head = len - k;
	auto new_tail = tail;
	while (steps_to_new_head--)
	{
		new_tail = new_tail->next;
	}

	auto new_head = new_tail->next;
	new_tail->next = nullptr;
	return new_head;
}
/*
* Can the head change? No
* DO we need to test inputs for null? maybe!
*/
template <typename T>
std::shared_ptr<ListNode<T>> EvenOddMerge(std::shared_ptr<ListNode<T>> head)
{
	if (!head || !head->next)
		return head;

	std::shared_ptr<ListNode<T>> dummyEven{ new ListNode<T>{0, head} };
	auto tailEven = dummyEven->next;
	std::shared_ptr<ListNode<T>> dummyOdd{ new ListNode<T>{0, head->next} };
	auto tailOdd = dummyOdd->next;
	auto node = head->next->next;
	for (int i = 0; node; ++i, node = node->next)
	{
		if (i % 2 == 0)
		{
			tailEven->next = node;
			tailEven = tailEven->next;
		}
		else
		{
			tailOdd->next = node;
			tailOdd = tailOdd->next;
		}
	}
	tailOdd->next = nullptr;
	tailEven->next = dummyOdd->next;
	return dummyEven->next;
}
