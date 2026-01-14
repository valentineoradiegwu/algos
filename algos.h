#pragma once
#include <iostream>
#include <vector>
#include <iterator>
#include <iostream>
#include <cmath>
#include <string>
#include <unordered_set>
#include <type_traits>
#include <future>
#include <thread>
#include "binarytree.h"
#include "singlelinked.h"

size_t CountBits(unsigned int);
size_t Count_Bits2(unsigned int input);
size_t TotalBits(unsigned int);
size_t CountBitsInRange(const uint64_t input, const uint64_t left, const uint64_t right);
short ParityBruteForce(unsigned long input);
short ParityBetterAverageCase(unsigned long input);
bool isPowerOfTwo(unsigned long input);
auto right_propagate_set_bit(unsigned long input);
void EvenOdd(std::vector<int>& input);
void EvenOdd1(std::vector<int>& input);
void EvenOddStable(std::vector<int>& input);
int Partition(std::vector<int>& input);
void DutchPartition(size_t pivot_idx, std::vector<int>& input);
bool CanJump(const std::vector<int>& nums);
int maxProfit(const std::vector<int>& prices);
bool IsPalindrome(const std::string& input);
bool isPalindrome2(const std::string& input);
bool IsPalindromicInt(int input);
bool IsPalindromicInt2(int input);
long ParseInt(const std::string& input);
int ParseInt2(const std::string& input);
std::string IntToString(long input);
/*std::vector<std::string> PrintStringPermutations(std::string& input);
std::vector<std::string> Merge(char someChar, std::vector<std::string> perms);*/
bool unique_chars(const std::string& input);
bool unique_chars2(const std::string& input);
void remove_dupes(char* input);
void remove_dupes1(char* input);
void removeDupesSorted(std::vector<int>& input);
bool are_anagrams(const std::string& one, const std::string& two);
void url_encode(std::string& input);
size_t ExtractNonZeroBytes(uint64_t input, unsigned char* destination);
size_t mystrcmp(const char* s1, const char* s2);
size_t mystrcmp2(const char* s1, const char* s2);
size_t mystrcmp3(const char* s1, const char* s2);
bool IsValidSudoku(const std::vector<std::vector<int>>& sudoku);
void PrintMatrixSpiral(const std::vector<std::vector<int>>& matrix);
std::vector<int> PrintMatrixSpiral2(const std::vector<std::vector<int>>& matrix);
void TransposeMatrix(std::vector<std::vector<int>>& in);
std::vector<std::vector<int>> MatrixProduct(const std::vector<std::vector<int>>& one, const std::vector<std::vector<int>>& two);
void PrintMatrix(const std::vector<std::vector<int>>& in);
void ReverseRowsMatrix(std::vector<std::vector<int>>& in);
void RotateMatrixClockwise(std::vector<std::vector<int>>& in);
void ReverseColumnsMatrix(std::vector<std::vector<int>>& in);
std::vector<std::vector<int>> PascalTriangle(int n);
std::vector<std::vector<int>> PascalTriangle2(int n);
std::vector<int> NextPermutation(std::vector<int>& in);
int ConvertToBaseTen(const std::string& in, int base);
std::string ConvertFromBaseTen(int decimal, int base);
double exp(double x, int y);
int ReplaceAndRemove(char array[], int size);
bool IsPalindromeAlphaInsensitive(const std::string& in);
void reverse(std::string& input, int startIdx, int endIdx);
void reverse(std::vector<int>& input, int startIdx, int endIdx);
void ReverseSentence(std::string& input);
int MaxSubarrayOnesAndZero(const std::vector<int>& in);
int MaxNonRepeatingSubString(const std::string& in);
std::string MultiplyStrings(const std::string& one, const std::string& two);
int RemoveDupes(std::vector<int>& input);
struct Rect
{
	int x;
	int y;
	int width;
	int height;
};
Rect RectangleIntersection(const Rect& first, const Rect& second);
struct Point
{
	int x;
	int y;
};
bool DoPointsFormARectangle(const Point& A, const Point& B, const Point& C, const Point& D);
std::string LookAndSay(int n);
std::string nextString(const std::string& s);
std::vector<std::string> GetValidIpAddress(const std::string& ip);
std::string RunlengthEncode(const std::string& in);
std::string RunlengthDecode(const std::string& in);
int strcmp(const char* one, const char* two);
std::vector<std::string> Split(const std::string& in, char delimeter, char escape);
std::vector<int> Primes(int n);
constexpr bool IsPrime(size_t n);
int FindSubstr1(const std::string& text, const std::string& pattern);
int FindSubstr2(const std::string& input, const std::string& key);
std::vector<int> twoSum(const std::vector<int>& nums, int target);
std::vector<int> twoSumSorted(const std::vector<int>& nums, int target);
std::vector<std::vector<int>> threeSum(std::vector<int>& nums, int target);
void RandomiseArray(std::vector<int>& input);
void PrintArrayRotatedLeft(const std::vector<int>& input, int k);
void PrintArrayRotatedRight(const std::vector<int>& input, int k);
void LeftShiftArray(std::vector<int>& input);
void RightShiftArray(std::vector<int>& input);
void RotateLeft(std::vector<int>& input, int k);
void RotateRight(std::vector<int>& input, int k);
void RotateRight2(std::vector<int>& input, int k);
int MinSwapsToBalance(const std::string& s);
int FirstUniqueSubstringWindow(const std::string& in, int windowSize);

//Stack
bool is_balanced(const std::string& input);
void PrintSubArrays(const std::vector<int>& input);
int EvaluateRPN(const std::string& expr); //Reverse Polish Notation
std::string ShortestDirPath(const std::string& path);
int longestValidParentheses(std::string s);
//Tree
std::vector<std::vector<int>> PrintLevelsBinTree1(std::shared_ptr<BinaryTree> head);
std::vector<std::vector<int>> PrintLevelsBinTree2(std::shared_ptr<BinaryTree> head);
std::vector<std::vector<int>> PrintLevelsBinTree3(std::shared_ptr<BinaryTree> head);
bool IsBalanced(const std::shared_ptr<const BinaryTree> root);
bool IsPerfect(const std::shared_ptr<const BinaryTree> root);
bool IsComplete(const std::shared_ptr<const BinaryTree> root);
int Height(const std::shared_ptr<const BinaryTree> root);
bool IsSymmetrical(const std::shared_ptr<const BinaryTree> root);
std::shared_ptr<const BinaryTree> LowestCommonAncestor(const std::shared_ptr<const BinaryTree> root, 
	const std::shared_ptr<const BinaryTree> one, 
	const std::shared_ptr<const BinaryTree> two);
std::shared_ptr<const BinaryTreeP> LowestCommonAncestor(std::shared_ptr<const BinaryTreeP> one,
	                                                    std::shared_ptr<const BinaryTreeP> two);
bool HasPathSum(const std::shared_ptr<const BinaryTree> root, int target);
int NumberOfElementsInSubTree(const std::shared_ptr<const BinaryTree> root);
std::vector<int> InOrderTraversalIterative(const std::shared_ptr<const BinaryTree> root);
std::shared_ptr<const BinaryTreeP> FindSuccessor(const std::shared_ptr<const BinaryTreeP> node);
std::vector<std::shared_ptr<const BinaryTree>> CreateListOfLeaves(const std::shared_ptr<const BinaryTree> root);
std::vector<std::shared_ptr<const BinaryTree>> ExteriorBinaryTree(const std::shared_ptr<const BinaryTree> root);
std::string removeDigit(std::string number, char digit);

//Heaps / Priority Queues
std::vector<std::string> TopK(const std::string& stream, int k);
std::vector<std::string> TopK1(const std::string& stream, int k);
std::vector<int> MergeKSortedArrays(const std::vector<std::vector<int>>& arrays);
auto MergeKSortedArrays(const std::vector<std::shared_ptr<ListNode<int>>>& arrays) -> decltype(arrays[0]);
std::vector<int> SortKIncresingDecreasingArray(const std::vector<int>& input);
std::vector<std::string> topKFrequent(const std::vector<std::string>& words, int k);
std::vector<std::vector<int>> kClosest(std::vector<std::vector<int>>& points, int k);
double RunningMedian(std::vector<int>::const_iterator begin, std::vector<int>::const_iterator end);
std::vector<int> KLargestInBinaryHeap(const std::vector<int>& heap, int k);
std::vector<int> SortApproximatelySortedData(std::vector<int>::const_iterator begin, const std::vector<int>::const_iterator end, int k);

//Searching
int BinarySearch(const std::vector<int>& input, int key);
std::string BinarySearchSortedFile(const std::string& fname, const std::string& key);
int FindFirstOfK(const std::vector<int>& input, int key);
int FindLastOfK(const std::vector<int>& input, int key);
std::pair<int, int> EqualRange(const std::vector<int>& input, int key);
int FindFirstGreaterThanK(const std::vector<int>& input, int key);
bool IsPrefixInSortedStringArray(const std::vector<std::string>& input, const std::string& prefix);
int FindSmallestInSortedRotated(const std::vector<int>& input);
int FindLargestInSortedRotated(const std::vector<int>& input);
int SearchInSortedRotated(const std::vector<int>& nums, int target);
int SquareRoot(int i);
bool SearchInSortedArray(const std::vector<std::vector<int>>& input, int k);
bool MatrixSearch(const std::vector<std::vector<int>>& matrix, int k);
int GenRandomIntInRange(int low, int high);
int FindKthLargest(std::vector<int>& nums, int k);
int FindMissingElement(std::vector<int>::const_iterator start, const std::vector<int>::const_iterator end);
std::pair<int, int> FindDuplicateAndMissingElement(const std::vector<int>& input);
std::pair<int, int> MinMax(const std::vector<int>& input);
std::string longestCommonPrefix(std::vector<std::string>& strs);
std::string longestCommonPrefix2(std::vector<std::string>& strs);

//HashTables
std::vector<std::vector<std::string>> FindAnagrams(const std::vector<std::string>& words);
bool CanFormPalindrome(const std::string& input);
bool CanFormPalindrome2(const std::string& input);
bool IsLetterConstructibleFromMagazine(const std::string& letter, const std::string& magazine);
int FindNearestRepetition(const std::vector<std::string>& paragraph);
std::pair<int, int> FindSmallestSubArrayCoveringSet(const std::vector<std::string>& paragraph, const std::unordered_set<std::string>& keywords);
int MaxSubArrayConsecutiveInteger(std::vector<int>& input);
int MaxSubArrayConsecutiveInteger1(const std::vector<int>& input);
std::vector<int> FindAllSubstrings(const std::string& sentence, const std::vector<std::string>& words);
int LengthOfLongestSubArrayWithSum(const std::vector<int>& input, int target);
int SubArraySum(const std::vector<int>& nums, int target);
int longestValidParentheses2(std::string s);

//Sorting
void InsertSort(std::vector<int>& in);
void SelectionSort(std::vector<int>& in);
std::vector<int> IntersectTwoSortedArrayNoDupes(const std::vector<int>& one, const std::vector<int>& two);
void MergeTwoSortedArraysIntoFirst(std::vector<int>& first, int m, const std::vector<int>& second, int n);
int HIndex(std::vector<int>& citations);
std::vector<std::vector<int>> insertInterval(std::vector<std::vector<int>>& intervals, std::vector<int>& newInterval);
std::vector<std::vector<int>> insertInterval1(std::vector<std::vector<int>>& intervals, std::vector<int>& newInterval);
std::vector<std::vector<int>> mergeIntervals(std::vector<std::vector<int>>& intervals);
int SmallestNonConstructibleValue(std::vector<int>& in);
int FindMaxSimultaneousEvents(const std::vector<std::pair<int, int>>& events);
void ReturnOnlyUniqueElements(std::vector<int>& input);
std::string MergeStrings(const std::vector<std::string>& words);
double FindMaxCap(double target, std::vector<double>& salaries);
auto OptimalScheduling(std::vector<std::tuple<std::string, int, int>> schedules) -> decltype(schedules);
std::vector<std::vector<int>> getSkyline(const std::vector<std::vector<int>>& buildings);

//BST
std::shared_ptr<BinarySearchTree> Search(std::shared_ptr<BinarySearchTree> root, int k);
bool IsBinaryTreeBST(std::shared_ptr<BinarySearchTree> root);
std::shared_ptr<BinarySearchTree> FindFirstGreaterThanK(std::shared_ptr<BinarySearchTree> root, int k);
std::shared_ptr<BinarySearchTree> FindFirstEqualToK(std::shared_ptr<BinarySearchTree> root, int k);
std::vector<int> FindKLargestInBST(std::shared_ptr<BinarySearchTree> root, int k);
std::shared_ptr<const BinarySearchTree> LowestCommonAncestor(const std::shared_ptr<const BinarySearchTree> root,
	const std::shared_ptr<const BinarySearchTree> one,
	const std::shared_ptr<const BinarySearchTree> two);
int FindClosestElementInSortedArrays(const std::vector<std::vector<int>>& arrays);
std::shared_ptr<BinarySearchTree> BuildMinHeightFromSortedArray(const std::vector<int>& sorted);
bool PairIsAncestorAndDescendant(const std::shared_ptr<const BinarySearchTree> possible_anc_or_desc_0,
								 const std::shared_ptr<const BinarySearchTree> possible_anc_or_desc_1,
	                             const std::shared_ptr<const BinarySearchTree> middle);
std::vector<int> RangeLookupBST(const std::shared_ptr<const BinarySearchTree> root, const std::pair<int, int>& range);

//Recursion
int gcd(int x, int y);
int gcd1(int x, int y);
int SumN(int N);
int SumN1(int N, int total = 0);
std::vector<std::pair<int, int>> TowerOfHanoi(int n);
std::vector<std::string> PhoneMnemonic(const std::string& number);
std::vector<std::string> PhoneMnemonic1(const std::string& number);
std::vector<std::vector<int>> Permutations(std::vector<int>& in);
std::vector<std::vector<int>> Permutations1(std::vector<int>& in);
std::vector<std::vector<std::string>> PowerSet(const std::vector<std::string>& in);
std::vector<std::vector<int>> PowerSet(const std::vector<int>& in);
std::vector<std::vector<int>> KCombinations(int n, int k);
std::vector<std::string> GenerateBalancedParenthesis(int k);
std::vector<std::vector<std::string>> PalindromeDecompositions(const std::string& s);
int NumCombinationForFinalScore(int final_score, const std::vector<int>& individual_scores);
int NumCombinationForFinalScore2(int final_score, const std::vector<int>& individual_scores);
std::vector<std::string> GetValidIpAddress2(const std::string& input);

//Dynamic Programming
int fib1(int n);
int FindMaximumSubarray(const std::vector<int>& input);
std::vector<std::string> DecomposeWordIntoDictionaryTerms(const std::string& word, const std::unordered_set<std::string>& dict);
int MinPathWeight(const std::vector<std::vector<int>>& triangle);
int NumberOfWaysToH(int no_of_steps, int max_steps);
int LongestNonDecreasingSubsequenceLength(const std::vector<int>& input);

//Greedy Algos
int CoinChange(int cents); //Only works for cents
std::vector<std::pair<int, int>> OptimumTaskAssignment(std::vector<int>& tasks);
int MinimumTotalWaitingTime(std::vector<int>& serviceTimes);

//Graphs
int numIslands(std::vector<std::vector<char>>& grid);
int numIslands2(std::vector<std::vector<char>>& grid);

template <typename T>
void PrintVector(const std::vector<T>& cont)
{
	std::cout << "Printing vector ....." << std::endl;
	for (const auto& i : cont)
	{
		std::cout << i << std::endl;
	}
}

template <typename CONT>
void PrintContainer(const CONT& cont)
{
	std::string sep = "";
	std::cout << "[";
	for (const auto& i : cont)
	{
		std::cout << sep <<  i;
		sep = " ";
	}
	std::cout << "]" << std::endl;
}

template <typename T, size_t N>
void PartitionBoolArray(std::array<T, N>& input)
{
	if (input.empty())
		return;
	size_t falseptr = 0, trueptr = input.size() - 1;
	while (falseptr < trueptr)
	{
		if (input[falseptr])
		{
			std::swap(input[falseptr], input[trueptr--]);
		}
		else
		{
			++falseptr;
		}
	}
}

template <typename BiIter>
void reverse(BiIter begin, BiIter end)
{
	while ((begin != end) && (begin != --end))
	{
		std::swap(*begin, *end);
		++begin;
	}
}

template <typename FIter1, typename FIter2>
bool equal(FIter1 begin1, FIter1 end1, FIter2 begin2)
{
	while (begin1 != end1)
	{
		if (!(*begin1 == *begin2))
			return false;
		++begin1;
		++begin2;
	}
	return true;
}

template <typename Iter1, typename Iter2>
void mergeSortedKeepDupes(Iter1 begin1, Iter1 end1, Iter1 begin2, Iter1 end2, Iter2 dest)
{
	while (begin1 != end1 && begin2 != end2)
	{
		if (*begin1 < *begin2)
		{
			*dest++ = *begin1++;
		}
		else
		{
			*dest++ = *begin2++;
		}
	}
	while (begin1 != end1)
	{
		*dest++ = *begin1++;
	}
	while (begin2 != end2)
	{
		*dest++ = *begin2++;
	}
}

//unique_copy only works on sorted ranges
template <typename FIter1, typename FIter2>
void unique_copy1(FIter1 begin, FIter1 end, FIter2 dest)
{
	auto elem = begin;
	while (elem != end)
	{
		//If i am not equal to my previous then copy me. Special consideration for the first element
		auto prev = elem; // Can u not also infer prev as elem - 1 ?
		if (elem == begin || !(*elem == *(--prev)))
		{
			*dest++ = *elem;
		}
		++elem;
	}
}
//There is an alternative solution which is to add the first element to result and compare each new elem
//to result and add if different. This does not work becase we cant get the last element from a back_inserter

template <typename FIter1, typename FIter2>
void unique_copy2(FIter1 begin, FIter1 end, FIter2 dest)
{
	if (begin == end)
		return;
	*dest = *begin;
	while (++begin != end)
	{
		if (!(*dest == *begin))
		{
			*(++dest) = *begin;
		}
	}
}

template <typename InputIter, typename OuputIter>
OuputIter partial_sum1(InputIter begin, InputIter end, OuputIter dest)
{
	if (begin == end)
		return dest;

	//If u dont have a default ctor this solution is screwed. Lets rework
	typename std::iterator_traits<InputIter>::value_type sum = *begin;
	*dest = sum;
	while (++begin != end)
	{
		sum += *begin;
		*++dest = sum;
	}
	return ++dest;
}

template <typename T, size_t N>
size_t arrayLength(T(&arr)[N])
{
	return N;
}

//result_of is available in versions <= 17, and invoke_result is available in versions >= 17
template <typename FUN, typename... ARGS>
std::future<typename std::invoke_result<FUN, ARGS...>::type> spawn_task(FUN&& fun, ARGS&&... args)
{
	using result_type = typename std::invoke_result<FUN, ARGS...>::type;
	std::packaged_task<result_type(ARGS...)> p_task{ std::forward<FUN>(fun) };
	auto fut = p_task.get_future();
	std::thread t{ std::move(p_task), args... };
	t.detach();
	return fut;
}

template <typename T, typename... PARAMS>
void Print(const T& firstArg, const PARAMS&... otherArgs)
{
	std::cout << firstArg << std::endl;
	if constexpr (sizeof...(otherArgs) > 0)
	{
		Print(otherArgs...);
	}
}

