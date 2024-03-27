// Personal.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <array>
#include <string>
#include <regex>
#include <iterator>
#include <thread>
#include "singlelinked.h"
#include "algos.h"
#include "lru.h"
#include "countintervals.h"
#include "parallelaccumulate.h"
#include "staticvector.h"
#include "spsc_overwrite.h"

int main()
{
	std::cout << "Hello World!\n";
	std::cout << "The number of set bits in 13 is " << CountBits(13) << std::endl;
	std::cout << "The toal number of bits in 4096 is " << CountTotalBits(4096) << std::endl;
	std::cout << "Parity for 13 is " << ParityBruteForce(13) << std::endl;
	std::vector<int> vec1{ 5, 2, 4, 5, 7, 9, 11, 23 };
	std::vector<int> vec2{ vec1 };
	EvenOdd(vec1);
	PrintVector(vec1);

	EvenOdd1(vec2);
	PrintVector(vec2);

	std::vector<int> vec3{ 5, 2, 4, 5, 7, 9, 1, 23 };
	DutchPartition(3, vec3);
	PrintVector(vec3);

	DutchPartition(3, vec3);
	PrintVector(vec3);

	std::array<bool, 7> boolCont{ true, false, false, true, false, true, true };
	PartitionBoolArray(boolCont);
	PrintContainer(boolCont);

	std::cout << "atoi = " << ParseInt("-123456") << std::endl;
	std::cout << "atoi = " << ParseInt("-2147483648") << std::endl;
	std::cout << "atoi = " << ParseInt("2147483647") << std::endl;
	try
	{
		std::cout << "atoi = " << ParseInt("2147483648") << std::endl;
	}
	catch (const std::exception& ex)
	{
		std::cout << "Caught exception " << ex.what() << std::endl;
	}
	std::cout << "Int2Str = " << IntToString(-123456) << std::endl;
	std::cout << "Simple ParseInt2 -123456 = " << ParseInt2("-123456") << std::endl;

	const std::string etn{ "IRS Currency USD;2" };
	const static std::regex currencyOverlay("IRS\\s+Currency\\s+([A-Z]{3});[0-9]+");
	std::smatch results;
	if (std::regex_match(etn, results, currencyOverlay))
	{
		std::cout << "OISonIRS Currency " + results[1].str() + ";1" << std::endl;
	}

	char test[] = "aaaabcfuuud";
	remove_dupes(test);
	std::cout << "Remove dupes = " << test << std::endl;
	char test1[] = "aaaabcfuuud";
	remove_dupes1(test1);
	std::cout << "Remove dupes = " << test1 << std::endl;
	std::cout << "Are Anagrams?  " << are_anagrams("listen", "silent") << std::endl;
	std::string url = "www.val obi oradiegwu.com ";
	url_encode(url);
	std::cout << "url = " << url << std::endl;
	uint64_t i = 256;
	unsigned char dest[sizeof(uint64_t)];
	auto bytes = ExtractNonZeroBytes(i, dest);
	std::cout << "No of bytes = " << bytes << std::endl;
	std::vector<int> sort1{ 1, 3, 5 };
	std::vector<int> sort2{ 1, 2, 4 };
	std::vector<int> out;
	mergeSortedKeepDupes(sort1.cbegin(), sort1.cend(), sort2.cbegin(), sort2.cend(), std::back_inserter(out));
	PrintContainer(out);
	out.clear();
	std::vector<int> sort3{ 1, 1, 1, 3, 3, 4, 5, 6, 6, 6, 6, 7, 8, 9, 10, 10 };
	unique_copy1(sort3.begin(), sort3.end(), std::back_inserter(out));
	PrintContainer(out);
	int anArray[5] = { 1, 2, 3, 4, 5 };
	std::cout << "size = " << arrayLength(anArray) << std::endl;
	std::cout << "Is IsPalindromicInt = " << IsPalindromicInt(23032) << std::endl;
	std::cout << "Is 23032 IsPalindromicInt = " << IsPalindromicInt2(23032) << std::endl;
	std::cout << "Is 2332 IsPalindromicInt = " << IsPalindromicInt2(2332) << std::endl;
	std::cout << "Is 2 IsPalindromicInt = " << IsPalindromicInt2(2) << std::endl;
	std::cout << "Is 45 IsPalindromicInt = " << IsPalindromicInt2(45) << std::endl;
	PrintMatrix(PascalTriangle(5));
	std::cout << "8B to base 10 = " << ConvertToBaseTen("8B", 16) << std::endl;
	std::cout << "8B to base 10 = " << ConvertToBaseTen("-8B", 16) << std::endl;
	std::cout << "exp(2.0, 8) = " << exp(2.0, 8) << std::endl;
	std::cout << "exp(2.0, -8) = " << exp(2.0, -8) << std::endl;
	std::string names{ "Val Obi Nna" };
	ReverseSentence(names);
	std::cout << names << std::endl;
	std::cout << MultiplyStrings("99", "99") << std::endl;
	std::cout << nextString("1211") << std::endl;
	std::cout << RunlengthEncode("aaaabcccaa") << std::endl;
	std::cout << RunlengthDecode(RunlengthEncode("aaaabcccaa")) << std::endl;
	Split("Val\\;Obi", ';', '\\');
	Split("\\;;;", ';', '\\');
	Primes(20);
	//auto idx = Substr1("fooawawaybla", "away");
	auto idx = FindSubstr1("awaaa", "away");
	std::cout << EvaluateRPN("3,4,+,2,*,1,+") << std::endl;
	auto sorted_elems = SortKIncresingDecreasingArray({ 57, 131, 493, 294,221, 339, 418, 452, 442, 190 });
	PrintVector(sorted_elems);
	const std::vector<int> str{ 1,0,3,5,2,0,1 };
	const auto ret = RunningMedian(str.begin(), str.end());
	std::cout << "Overall median = " << ret << std::endl;
	std::cout << "prefix = " << IsPrefixInSortedStringArray({ "obi", "oradiegwu", "valentine" }, "valo") << '\n';
	std::cout << FindLargestInSortedRotated({ 378,478,550,631,103,203,220,234,279,368 }) << '\n';
	LRUCache<int, std::string> lru{ 5 };
	const std::string sentence{ "amanaplanacanal" };
	const std::vector<std::string> words{ "can", "apl", "ana" };
	FindAllSubstrings(sentence, words);
	IntersectTwoSortedArrayNoDupes({ 2,3,3,5,5,6,7,7,8,12 }, { 5,5,6,8,8,9,10,10 });
	CountIntervals intervals{};
	intervals.add(8, 43);
	intervals.add(13, 16);
	intervals.add(26, 33);
	intervals.add(28, 36);
	intervals.add(29, 37);
	std::cout << "count = " << intervals.count() << '\n';
	intervals.add(34, 46);
	intervals.add(10, 23);

	std::cout << "hardware concurrency = " << std::thread::hardware_concurrency() << std::endl;
	std::vector<int> nums{};
	nums.reserve(1000);
	for (int i = 1; i <= 1000; ++i)
		nums.push_back(i);
	ParallelAccumulate acc{ nums, 0 };
	const auto cum = acc.Accummulate();
	std::cout << "sum = " << cum << std::endl;

	const auto moves = TowerOfHanoi(3);
	for (const auto& mnemonic : PhoneMnemonic1("23"))
	{
		std::cout << mnemonic << std::endl;
	}
	const auto sets = PowerSet({ "apple", "bananna", "mango" });
	for (const auto& i : sets)
	{
		PrintContainer(i);
	}

	//test push_back
	val::utils::static_vector<std::string, 10> s_vec{};
	s_vec.push_back(std::string{"Val"});
	std::cout << "Size = " << s_vec.size() << ". Capacity = " << s_vec.capacity() << std::endl;
	s_vec.push_back(std::string{ "Obi" });
	s_vec.push_back(std::string{ "Oradiegwu" });

	//Test copy ctor
	val::utils::static_vector<std::string, 10> s_vec1{s_vec};
	for (const auto& i : s_vec1)
		std::cout << "s_vect1: " << i << std::endl;

	//test front and back
	std::cout << "Front = " << s_vec.front() << std::endl;
	std::cout << "Back = " << s_vec.back() << std::endl;

	//test move ctor
	val::utils::static_vector<std::string, 10> s_vec3{ std::move(s_vec1) };
	std::cout << "Size = " << s_vec3.size() << ". Capacity = " << s_vec.capacity() << std::endl;

	//The moved vector should be empty now
	std::cout << "s_vect1 size: " << s_vec1.size() << std::endl;

	for (const auto& i : s_vec3)
		std::cout << "s_vect3 after move ctor: " << i << std::endl;

	//test emplace_back
	s_vec3.emplace_back(10, '*');
	std::cout << "Back after emplace_back = " << s_vec3[s_vec3.size() - 1] << std::endl;

	//test copy asignment
	val::utils::static_vector<std::string, 10> s_vec4{};
	s_vec4.emplace_back("bar");
	s_vec4 = s_vec3;
	for (const auto& i : s_vec4)
		std::cout << "s_vect4 after copy assignment: " << i << std::endl;
	val::utils::static_vector<std::string, 10> s_vec5{};
	s_vec4 = s_vec5;
	std::cout << "s_vec4 after assignment size = " << s_vec4.size() << std::endl;

	//test move assignment
	s_vec5 = std::move(s_vec3);
	for (const auto& i : s_vec5)
		std::cout << "s_vect5 after move assignment: " << i << std::endl;
	s_vec5.push_back(std::string{"bar"});
	for (const auto& i : s_vec5)
		std::cout << "s_vect5 after move push_back: " << i << std::endl;

	//test initialiser list
	val::utils::static_vector<std::string, 10> s_vec6{"orange", "banana", "mango"};
	for (const auto& i : s_vec6)
		std::cout << "s_vect6 after Initialiser list ctor: " << i << std::endl;
	val::utils::static_vector<int, 10> s_vec7{ 1, 2, 3 };

	val::utils::spsc_overwrite<int, 4> spsc_queue{};
	auto outcome = spsc_queue.pop();
	std::cout << "outcome of pop when empty = " << std::boolalpha << bool(outcome) << std::endl;
	for (int i = 1; i < 7; ++i)
		spsc_queue.push(i);
	outcome = spsc_queue.pop();
	std::cout << "Popped = " << *outcome << std::endl;

	spsc_queue.push(7);
	spsc_queue.push(8);
	spsc_queue.push(9);

	outcome = spsc_queue.pop();
	std::cout << "Popped = " << *outcome << std::endl;

	for (int i = 10; i < 20; ++i)
		spsc_queue.push(i);

	while (outcome = spsc_queue.pop())
		std::cout << "Popped = " << *outcome << std::endl;
	std::cout << "Should be empty = " << std::boolalpha << spsc_queue.empty() << std::endl;

	spsc_queue.push(22);
	std::cout << "Should NOT be empty = " << std::boolalpha << spsc_queue.empty() << std::endl;
}
