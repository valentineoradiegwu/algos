#include "algos.h"
#include <stdexcept>
#include <algorithm>
#include <math.h>
#include <cassert>
#include <numeric>
#include <unordered_map>
#include <deque>
#include <queue>
#include <stack>
#include <functional>
#include <sstream>
#include <tuple>
#include <bitset>
#include <unordered_set>
#include <set>
#include <array>
#include <fstream>
#include <filesystem>


/*
1. The idea is to use bit shifting.
2. We use the left shift so that we can test the last bit (LSB)
3. Once a bit is shifted off, it is replaced by 0 on the MSB
TIME COMPLEXITY
The best case scenario is when the input is 0 and that is O(1)
The worst case scenario is when the input is > 0 i.e O(n)
SPACE COMPLEXITY
No extra space since the algo works in place and is therefore O(1)
*/
size_t CountBits(unsigned int input)
{
	size_t count = 0;
	while (input)
	{
		count += input & 1;
		input >>= 1;
	}
	return count;
}
/*
* More efficient since we loop by the number of times we have set bits rather than on every bit as above.
*/
size_t Count_Bits2(unsigned int input)
{
	size_t count = 0;
	while (input)
	{
		input &= (input - 1);
		++count;
	}
	return count;
}

size_t TotalBits(unsigned int input)
{
	return int(log2(input)) + 1;
}
/*
The Parity is 1 if their is an odd number of bits in the binary data and its 0 if the number is even.
At first glance, we could use the CountBits to tally number of bits and check if odd or even. The 
issue is that if we are trying to tally a very long input, we would need to allocate a large variable
to store the count. Instead we use xor to flip between 0 and 1 since we dont care about absolute count.
*/
short ParityBruteForce(unsigned long input)
{
	short parity = 0;
	while (input)
	{
		parity ^= input & 1;
		input >>= 1;
	}
	return parity;
}

short ParityBetterAverageCase(unsigned long input)
{
	short res = 0;
	while (input)
	{
		res ^= 1;
		input &= (input - 1);
	}
	return res;
}

/*
* 1. 
*/
size_t CountBitsInRange(const uint64_t input, const uint64_t left, const uint64_t right)
{
	uint64_t mask = (uint64_t)1 << right;
	size_t count = 0;
	for (int i = left; i <= right; ++i)
	{
		count += mask & input;
		mask <<= 1;
	}
	return count;
}

bool isPowerOfTwo(unsigned long input)
{
	return input != 0 && (input & (input - 1)) == 0;
}

auto right_propagate_set_bit(unsigned long input)
{
	return input | (input - 1);
}
/*
* Same algo as the partition in quicksort. It is not stable though.
*/
void EvenOdd(std::vector<int>& input)
{
	int partition = 0;
	for (int i = 0; i < input.size(); ++i)
	{
		if (input[i] % 2 == 0)
		{
			std::swap(input[i], input[partition++]);
		}
	}
}
/*
This solution is stable.
{12, 34, 45, 9, 8, 90, 3}
*/
void EvenOdd1(std::vector<int>& input)
{
	size_t partition = 0;
	for (size_t i = 0; i < input.size(); ++i) {
		if ((input[i] & 1) == 0) {
			// Rotates by shifting all elements
			int temp = input[i];  // Save the even element
			for (size_t j = i; j > partition; --j) {
				input[j] = input[j - 1];  // Shift elements right
			}
			input[partition] = temp;  // Place even element at partition
			++partition;
		}
	}
}

/*
Isnt this code exactly same as partitioning via a key? or similar at least.
*/
int Partition(std::vector<int>& input)
{
	if (input.empty())
		return -1;

	int key = input[input.size() - 1];
	int partitionIdx = 0;
	for (size_t i = 0; i < input.size() - 1; ++i)
	{
		if (input[i] <= key)
		{
			std::swap(input[i], input[partitionIdx]);
			++partitionIdx;
		}
	}
	std::swap(input[partitionIdx], input[input.size() - 1]);
	return partitionIdx;
}

/*
The Dutch National Flag where we partition an array into 3 segments.
The first will contain elements less than a pivot.
The second will contain elements equal to the pivot.
And finally the third will contain elements greater than the pivot.

This can be trivially accomplished if we ue 3 extra vectors to collect these elements
and then assemble them back into a vector.

It is less trivial if it is done inplace
*/

void DutchPartition(size_t pivot_idx, std::vector<int>& input)
{
	if (input.empty())
		return;

	auto pivot = input[pivot_idx];
	size_t smaller = 0, runner = 0, larger = input.size() - 1;
	while (runner < larger)
	{
		if (input[runner] < pivot)
		{
			std::swap(input[smaller++], input[runner++]);
		}
		else if (input[runner] > pivot)
		{
			std::swap(input[larger--], input[runner]);
		}
		else
		{
			++runner;
		}
	}
}

//My take on the problem and it works like a charm.
bool CanJump(const std::vector<int>& nums)
{
	int current = nums.size() - 1;
	while (current > 0)
	{
		int runner = current - 1;
		while (nums[runner] < (current - runner))
		{
			--runner;
			if (runner < 0) break;
		}
		current = runner;
	}
	return current == 0;
}

int maxProfit(const std::vector<int>& prices) 
{
	int max_so_far = 0; //I guess not doing any trade is an option in which case our worst case profit is 0
	int best_buy = prices[0];
	for (int i = 0; i < prices.size(); ++i)
	{
		max_so_far = std::max(max_so_far, prices[i] - best_buy);
		best_buy = std::min(best_buy, prices[i]);
	}
	return max_so_far;
}

/*
* Here you can buy and sell multiple times
*/
int maxProfit2(std::vector<int>& prices) {
	int maxProfit = 0;
	for (int i = 1; i < prices.size(); ++i)
	{
		if (prices[i] > prices[i - 1])
			maxProfit += prices[i] - prices[i - 1];
	}
	return maxProfit;
}

bool IsPalindrome(const std::string& input)
{
	for (size_t i = 0, j = input.size() - 1; i < j; ++i, --j)
	{
		if (input[i] != input[j])
			return false;
	}
	return true;
}

bool isPalindrome2(const std::string& input)
{
	return std::equal(input.begin(), input.begin() + input.size() / 2, input.rbegin());
}

/*
* The general idea is to extract the MSD and LSD and compare and then drop them off.
* To fetch the LSD, you simply do input % 10
* To fetch the MSB, you do input / (10 ^ n) where n is number of digits - 1
* To chop off the LSD, you do input / 10
* To chop off the MSD, you do input % (10 ^ n)
* How do we work out the number of digits? floor(Log10(input)) + 1
*/
bool IsPalindromicInt(int input)
{
	int N = floor(log10(input)) + 1;
	int divisor = pow(10, N - 1);
	while (N > 1)
	{
		if ((input % 10) != (input / divisor))
			return false;
		input %= divisor;
		input /= 10;
		N -= 2;
		divisor /= 100;
	}
	return true;
}
/*
* We work out the reverse and compare to the remaining
* Note that there is a corner case if the input is a single digit
* There is another corner case for odd number digits. Solved
* And yet another corner case when the last digit is 0. For it to be a palindrome, then the number must be 0.
*/
bool IsPalindromicInt2(int input)
{
	if (input < 0 || input % 10 == 0 && input != 0)
		return false;
	int reversed = 0;
	while (input > reversed)
	{
		reversed = (reversed * 10) + (input % 10);
		input /= 10;
	}
	return (input == reversed) || (reversed / 10 == input);
}
/*
* Its interesting how the library detects overflow or underflow.
* See https://code.woboq.org/gcc/libiberty/strtol.c.html
* An unsigned int (or is it long) can maybe have a range from [-2147483648..2147483647]
* We accumulate the result up to a cut-off which is one less the max/min i.e -214748364 and 214748364
* There are 3 possibilities me reckons
* 1. If we are less than the cutoff then the next char can be any valid char from 0 - 9.
* 2. If we are greater than the cutoff, then appending the next char will move it to [over/under]flow
* 3. If we are precisely on the cutoff then the next char has to <= 7 on +ve or <= 8 on negative path.
*/
long ParseInt(const std::string& input)
{
	long result = 0;
	int current = 0;
	while (current < input.size() && isspace(input[current]))
	{ 
		++current;
	}
	//At this point I am pointing at first non-space or have overshot the string
	if (current >= input.size())
		return result;

	auto negative = false;

	if (input[current] == '-' || input[current] == '+')
	{
		if (input[current] == '-')
			negative = true;
		++current;
	}

	long cutoff = abs(negative ? std::numeric_limits<long>::min() / 10 : std::numeric_limits<long>::max() / 10);
	char cutlevel = abs(negative ? std::numeric_limits<long>::min() % 10 : std::numeric_limits<long>::max() % 10) + '0';
	std::cout << "cutoff = " << cutoff << " cutlevel = " << cutlevel << std::endl;

	for (size_t i = current; i < input.size(); ++i)
	{
		auto digit = input[i];
		if (result > cutoff || (result == cutoff && digit > cutlevel))
		{
			if (negative)
				throw std::underflow_error{ "" };
			else
				throw std::overflow_error{ "" };
		}
		if (digit < '0' || digit > '9')
			continue;
		result = result * 10 + (digit - '0');
	}

	if (negative)
		result *= -1;

	return result;
}

/*
* Incase the interviewer wants bare bones and no frills StringToInt
*/
int ParseInt2(const std::string& input)
{
	int result = 0;
	if (input.empty())
		return result;
	return (input[0] == '-' ? -1 : 1) * std::accumulate(input.begin() + (input[0] == '-'),
			input.end(),
			result,
			[](int res, char c) {return (res * 10) + (c - '0'); });
}

/*
* Do you really need a do while?
*/
std::string IntToString(long input)
{
	auto negative = input < 0;
	input = std::abs(input);
	std::string res{};
	do {
		auto digit = input % 10;
		res.push_back(digit + '0');
		input /= 10;
	} while (input);

	if (negative)
		res.push_back('-');
	return {res.rbegin(), res.rend()}; 
}

//Doing this check in place sacrifices time (performance) for space
bool unique_chars(const std::string& input)
{
	for (size_t i = 0; i < input.size(); ++i)
	{
		for (size_t j = 0; j < i; ++j)
		{
			if (input[j] == input[i])
				return false;
		}
	}
	return true;
}

/*
	Given extra space, we can record all the chars we've seen so far.
	This can be done by a std::set but the check for the existence of a key will be logarithmic
	If we assume the chars will be ascii, we can use an array of 256 bools and index into the array with the char 
*/
bool unique_chars2(const std::string& input)
{
	bool mychars[256] = { false };
	for (auto achar : input)
	{
		if (mychars[achar])
			return false;
		mychars[achar] = true;
	}
	return true;
}

void remove_dupes(char* input)
{
	char* runner = input;
	char* end = input;
	while (*runner)
	{
		//Look back to see if I have seen this char before
		//This is odd...why not look only in unique set?
		char* j = input;
		for (; j < runner; ++j)
		{
			if (*j == *runner)
				break;
		}
		//If so, advance runner only
		//if not, copy char at runner to end and advance both runner and end
		if (j == runner)
		{
			*end = *runner;
			++end;
		}

		++runner;
	}
	*end = '\0';
}

//"aaaabcfuuud"
/*
* This is a bit mad. For decades the solution I had in mind wasnt the solution that actually played out
* Why scan the chars from 0 to i to find a duplicate rather than scan from 0 to end?
*/
void remove_dupes1(char* input)
{
	char* end = input + 1;
	for (auto i = input + 1; *i; ++i)
	{
		char* j = input;
		for (; j < end; ++j)
		{
			if (*j == *i)
			{
				break;
			}
		}
		if (j == i)
		{
			*end = *i;
			++end;
		}
	}
	*end = '\0';
}

/*
* This sort of algorithms are usually easy to visualise but the implementation can take on 2 forms.
* If we find the runner is equal to the partition, do we copy b4 we increment or copy after we increment?
* Copy b4 increment tends to make the algo easier.
*/
void removeDupesSorted(std::vector<int>& input)
{
	int tail = 1;
	for (int runner = 1; runner < input.size(); ++runner)
	{
		if (input[runner] != input[tail - 1])
		{
			input[tail] = input[runner];
			++tail;
		}
	}
	input.erase(input.begin() + tail, input.end());
}

bool are_anagrams(const std::string& one, const std::string& two)
{
	if (one.size() != two.size())
		return false;
	int chars[256] = { 0 };
	int unique_chars = 0;

	for (auto each_char : one)
	{
		if (chars[each_char] == 0)
			++unique_chars;
		++chars[each_char];
	}

	for (size_t i = 0; i < two.size(); ++i)
	{
		if (chars[two[i]] == 0)
			return false;
		--chars[two[i]];
		if (chars[two[i]] == 0)
			--unique_chars;
		if (unique_chars == 0)
			return i == two.size() - 1;
	}
	return false;
}

void url_encode(std::string& url)
{
	const auto count = std::count_if(url.begin(), url.end(), [](char c) {return c == ' '; });
	if (!count)
		return;
	const size_t new_size = url.size() + count * 2;
	int ptr1 = url.size() - 1;
	url.resize(new_size);
	int ptr2 = new_size - 1;
	while (ptr1 >= 0 && ptr2 >= 0)
	{
		if (url[ptr1] != ' ')
			url[ptr2--] = url[ptr1--];
		else
		{
			url[ptr2--] = '0';
			url[ptr2--] = '2';
			url[ptr2--] = '%';
			--ptr1;
		}
	}
}

size_t ExtractNonZeroBytes(uint64_t input, unsigned char* destination)
{
	size_t count = 0;
	while(input)
	{
		unsigned char b = input & 0xff;
		if (b)
		{
			*destination = b;
			++destination;
			++count;
		}
		input >>= 8;
	}
	return count;
}

size_t mystrcmp(const char* s1, const char* s2)
{
	//Normally one would break if the strings were different lengths but computing the length on a c string is a linear op
	//the strings may be different lenghts so one has to cater for that.
	while (*s1 && *s2)
	{
		if (*s1 < *s2)
		    return -1;
		if (*s2 < *s1)
			return 1;
		++s1;
		++s2;
	}
	if (!*s1)
		return *s2 ? -1 : 0;
 
	return 1;
}

size_t mystrcmp2(const char* s1, const char* s2)
{
	while (*s1 || *s2)
	{
		if (!*s1)
			return -1;
		if (!*s2)
			return 1;
		if (*s1 < *s2)
			return -1;
		if (*s2 < *s1)
			return 1;
		++s1;
		++s2;
	}
	return 0;
}

size_t mystrcmp3(const char* s1, const char* s2)
{
	while (*s1)
	{
		if (!*s2)
			return 1;
		if (*s1 < *s2)
			return -1;
		if (*s2 < *s1)
			return 1;
		++s1;
		++s2;
	}
	if (*s2)
		return -1;
	return 0;
}
/*
* Given a list [1, 2, 3], we want to number of permutations i.e n!.
* This was surprisingly difficult to work out. This solution doesnt seem efficient though.
* This is a classical case of recursion as you would need a fixed number of loops to do it iteratively
[1 2 3]
[1 3 2]
[2 1 3]
[2 3 1]
[3 1 2]
[3 2 1]
*/
void Permutate(const std::vector<int>& in)
{
	for (int i = 0; i < in.size(); ++i)
	{
		for (int j = 0; j < in.size(); ++j)
		{
			for (int k = 0; k < in.size(); ++k)
			{
				if (k != j && k != i && i != j)
				{
					std::cout << "[" << in[i] << " " << in[j] << " " << in[k] << "]" << '\n';
				}
			}
		}
	}
}

/*std::vector<std::string> PrintStringPermutations(std::string& input)
{
	if (i == j)
	{
		std::cout << input[i] << std::endl;
		return;
	}
	//Merge(input[j], PrintStringPermutations(i, j -1, input))
}*/

//sudoku[rowNum][0-9]
bool IsValidRow(const std::vector<std::vector<int>>& sudoku, int rowNum)
{
	std::deque<bool> is_present(sudoku.size() + 1, false);
	for (int col = 0; col < sudoku.size(); ++col)
	{
		if (sudoku[rowNum][col])
		{
			if (is_present[sudoku[rowNum][col]])
				return false;
			is_present[sudoku[rowNum][col]] = true;
		}
	}
	return true;
}
//sudoku[0-9][colNum]
bool IsValidColumn(const std::vector<std::vector<int>>& sudoku, int colNum)
{
	std::deque<bool> is_present(sudoku.size() + 1, false);
	for (int row = 0; row < sudoku.size(); ++row)
	{
		if (sudoku[row][colNum])
		{
			if (is_present[sudoku[row][colNum]])
				return false;
			is_present[sudoku[row][colNum]] = true;
		}
	}
	return true;
}
bool IsValidSquare(const std::vector<std::vector<int>>& sudoku)
{
	for (int row = 0; row < sudoku.size(); row += 3)
	{
		for (int col = 0; col < sudoku.size(); col += 3)
		{
			std::deque<bool> is_present(sudoku.size() + 1, false);
			for (int i = row; i < row + 3; ++i)
			{
				for (int j = col; j < col + 3; ++j)
				{
					if (sudoku[i][j])
					{
						if (is_present[sudoku[i][j]])
							return false;
						is_present[sudoku[i][j]] = true;
					}
				}
			}
		}
	}
	return true;
}

bool IsValidSudoku(const std::vector<std::vector<int>>& sudoku)
{
	for (int row = 0; row < sudoku.size(); ++row)
	{
		if (!IsValidRow(sudoku, row))
			return false;
	}

	for (int col = 0; col < sudoku.size(); ++col)
	{
		if (!IsValidColumn(sudoku, col))
			return false;
	}
	return IsValidSquare(sudoku);
}

void PrintMatrixSpiral(const std::vector<std::vector<int>>& matrix)
{
	int startRow = 0;
	int startCol = 0;
	int endRow = matrix.size();
	int endCol = matrix[0].size();

	//What if u have a vector like {{0}}?
	while (startRow < endRow && startCol < endCol)
	{
		for (int i = startCol; i < endCol; ++i)
		{
			std::cout << matrix[startRow][i] << " ";
		}
		++startRow;
		for (int i = startRow; i < endRow; ++i)
		{
			std::cout << matrix[i][endCol - 1] << " ";
		}
		--endCol;
		if (startRow < endRow)
		{
			for (int i = endCol - 1; i >= startCol; --i)
			{
				std::cout << matrix[endRow - 1][i] << " ";
			}
			--endRow;
		}
		if (startCol < endCol)
		{
			for (int i = endRow - 1; i >= startRow; --i)
			{
				std::cout << matrix[i][startCol] << " ";
			}
			++startCol;
		}
	}
}

void print_sub_matrix(const std::vector<std::vector<int>>& matrix, int offset, std::vector<int>& res)
{
	//There is a corner case for odd numbered matrix.
	if (offset == matrix.size() - offset - 1)
	{
		res.push_back(matrix[offset][offset]);
	}

	for (int col = offset; col < matrix[0].size() - offset - 1; ++offset)
	{
		res.push_back(matrix[offset][col]);
	}

	for (int row = offset; row < matrix.size() - offset - 1; ++row)
	{
		res.push_back(matrix[row][matrix[0].size() - offset - 1]);
	}

	for (int col = matrix[0].size() - offset - 1; col > offset; --col)
	{
		res.push_back(matrix[matrix.size() - offset - 1][col]);
	}

	for (int row = matrix.size() - offset - 1; row > offset; --row)
	{
		res.push_back(matrix[row][offset]);
	}
}

/*
* If the matrix in n x n, another solution can be employed that can operate consistently on each submatrix.
* 
*/
std::vector<int> PrintMatrixSpiral2(const std::vector<std::vector<int>>& matrix)
{
	std::vector<int> res;
	res.reserve(matrix.size() * matrix[0].size());
	for (int i = 0; i < std::ceil(0.5 * matrix.size()); ++i)
	{
		print_sub_matrix(matrix, i, res);
	}
	return res;
}

void TransposeMatrix(std::vector<std::vector<int>>& in)
{
	for (int i = 0; i < in.size(); ++i)
	{
		for (int j = i; j < in[0].size(); ++j)
		{
			std::swap(in[i][j], in[j][i]);
		}
	}
}

std::vector<std::vector<int>> MatrixProduct(const std::vector<std::vector<int>>& one, const std::vector<std::vector<int>>& two)
{
	if (one[0].size() != two.size())
		return {};

	std::vector<std::vector<int>> res(one.size(), std::vector<int>(two[0].size()));
	for (int col = 0; col < two[0].size(); ++col)
	{
		for (int row = 0; row < one.size(); ++row)
		{
			for (int idx = 0; idx < one[0].size(); ++idx)
			{
				res[row][col] += one[row][idx] * two[idx][col];
			}
		}
	}
	return res;
}

void PrintMatrix(const std::vector<std::vector<int>>& in)
{
	std::cout << "Printing Matrix" << std::endl;
	for (int i = 0; i < in.size(); ++i)
	{
		for (int j = 0; j < in[i].size(); ++j)
		{
			std::cout << in[i][j] << " ";
		}
		std::cout << '\n';
	}
	std::cout << '\n';
}

void ReverseRowsMatrix(std::vector<std::vector<int>>& in)
{
	if (in.empty())
		return;
	for (int row = 0; row < in.size(); ++row)
	{
		for (int startCol = 0, endCol = in[0].size() - 1; startCol < endCol; ++startCol, --endCol)
		{
			std::swap(in[row][startCol], in[row][endCol]);
		}
	}
}
/*
* Rotating an n X n matrix can come in a variety of flavours
* It can either be clockwise or anticlockwise
* It can also be 90, 180 and 270 degrees.
* Clockwise the steps are
* 1. Transpose the matrix which is swapping ith row for ith col
* 2. Reverse each row
* 3. Repeat same 2 operations for each 90 degrees flip
* Anticlockwise the steps are
* 1. Transpose the matrix which is swapping ith row for ith col
* 2. Reverse each col
* 3. Repeat same 2 operations for each 90 degrees flip
*/
void RotateMatrixClockwise(std::vector<std::vector<int>>& in)
{
	TransposeMatrix(in);
	ReverseRowsMatrix(in);
}

void ReverseColumnsMatrix(std::vector<std::vector<int>>& in)
{
	if (in.empty())
		return;
	for (int col = 0; col < in[0].size(); ++col)
	{
		for (int startRow = 0, endRow = in.size() - 1; startRow < endRow; ++startRow, --endRow)
		{
			std::swap(in[startRow][col], in[endRow][col]);
		}
	}
}

std::vector<std::vector<int>> PascalTriangle(int n)
{
	std::vector<std::vector<int>> pascalTriangle{};
	for (int i = 0; i < n; ++i)
	{
		std::vector<int> thisRow{};
		if (pascalTriangle.empty())
		{
			thisRow.push_back(1);
		}
		else
		{
			const auto& prevRow = pascalTriangle.back();
			for (int j = 0; j <= i; ++j)
			{
				if (j == 0 || j == i)
					thisRow.push_back(1);
				else
					thisRow.push_back(prevRow[j] + prevRow[j - 1]);
			}
		}
		pascalTriangle.push_back(std::move(thisRow));
	}
	return pascalTriangle;
}

std::vector<std::vector<int>> PascalTriangle2(int n)
{
	std::vector<std::vector<int>> res{};
	res.reserve(n);
	for (int i = 0; i < n; ++i)
	{
		if (i == 0)
		{
			res.push_back({ 1 });
			continue;
		}
		const auto& prev = res.back();
		std::vector<int> temp{};
		temp.reserve(prev.size() + 1);
		for (int i = 0; i < prev.size() + 1; ++i)
		{
			const int val = (i > 0 ? prev[i - 1] : 0) + (i < prev.size() ? prev[i] : 0);
			temp.push_back(val);
		}
		res.push_back(std::move(temp));
	}
	return res;
}

/*
* Look at the number 6534210. What do u think the next permutation should be?
* By visual inspection we know the next number will be 65[next number in 34210] = 6540123
* 1.Search from the right to the next position that is not sorted.
* 2. Find the next highest number in the resulting range.
* 3. Swap the number that is the next highest with one that breaks the ascending order sort.
* 4. reverse the remaining
*/
std::vector<int> NextPermutation(std::vector<int>& in)
{
	auto insertion_point = std::is_sorted_until(in.rbegin(), in.rend());
	if (insertion_point == in.rend())
		return {};
	auto next_biggest = std::upper_bound(in.rbegin(), insertion_point, *insertion_point);
	std::iter_swap(insertion_point, next_biggest);
	std::reverse(in.rbegin(), insertion_point);
	return in;
}

//Do we need to cater for negative? Overflow? empty space?
int ConvertToBaseTen(const std::string& in, int base)
{
	int result = 0;
	const auto is_negative = in[0] == '-';
	const auto lambda = [base](int res, char c)
	{
		const int digit = isdigit(c) ? c - '0' : c - 'A' + 10;
		return (res * base) + digit;
	};
	result = std::accumulate(in.begin() + is_negative, in.end(), result, lambda);
	return (is_negative ? -1 : 1) * result;
}

std::string ConvertFromBaseTen(int decimal, int base)
{
	std::string res{};
	const auto is_negative = decimal < 0;
	decimal = std::abs(decimal);
	while (decimal)
	{
		const auto lsd = decimal % base;
		res += lsd < 10 ? (lsd + '0') : (lsd + 'A' - 10);
		decimal /= base;
	}
	if (is_negative)
		res += '-';
	return { res.rbegin(), res.rend() };
}

/*
* Spread sheet cols go from A - Z, AA - ZZ, AAA - ZZZ
* Decode the column to an int.
* 1. A has an int value of 1 and Z has an int value of 26.
* 2. It is a base 26 number system since the number system can be represented by 26 chars.
*/
int SpreadSheetGetColId(const std::string& in)
{
	return std::accumulate(in.begin(), in.end(), 0, [](int res, char c) {return res * 26 + (c - 'A' + 1); });
}

/*
* If the power is negative, we could compute the solution as though it were +ve then compute the inverse of the final result.
* There is an alternative solution which is to convert x to 1/x. 
* Also ensure you negate the exponent or power if its negative. I think the left shift will be screwed up otherwise.
*/
double exp(double x, int y)
{
	double result = 1.0;
	double base = x;
	if (y < 0)
	{
		x = 1.0 / x;
		y = -y;
	}
	while (y)
	{
		if (y & 1)
		{
			result *= base;
		}
		base *= base;
		y >>= 1;
	}
	return result;
}
/*
* The new size could be less than original size.
* Do the remove first before the replace so flip.
* Consider using a while loop for the replace code.
*/
int ReplaceAndRemove(char array[], int size)
{
	//remove b's
	int a_count = 0, end = 0;
	for (int i = 0; i < size; ++i)
	{
		if (array[i] != 'b')
		{
			array[end++] = array[i];
		}
		if (array[i] == 'a')
			++a_count;
	}
	int new_size = end + a_count ;

	//replace a's
	for (int i = end - 1, j = new_size - 1; i >= 0 && j >= 0 && i != j; --i)
	{
		if (array[i] != 'a')
			array[j--] = array[i];
		else
		{
			array[j--] = 'd';
			array[j--] = 'd';
		}
	}
	return new_size;
}
/*
* Cant let either while loops drop out of each end.
* Should they even cross each other? 
* Is an empty string a palindrome? i.e one that has all non-alpha's? In which case the while loops will run to the end
*/
bool IsPalindromeAlphaInsensitive(const std::string& in)
{
	int start = 0, end = in.size() - 1;
	while (start < end)
	{
		while (start < end && !isalnum(in[start]))
			++start;
		while (start < end && !isalnum(in[end]))
			--end;
		if (tolower(in[start]) != tolower(in[end]))
			return false;
		++start;
		--end;
	}
	return true;
}

void reverse(std::vector<int>& input, int startIdx, int endIdx)
{
	while (startIdx < endIdx)
	{
		std::swap(input[startIdx++], input[endIdx--]);
	}
}

void reverse(std::string& input, int startIdx, int endIdx)
{
	for (int i = startIdx, j = endIdx; i < j; ++i, --j)
	{
		std::swap(input[i], input[j]);
	}
}

//Recursive reverse
void reverseRecursive(std::string& input, int startIdx, int endIdx)
{
	if (startIdx > endIdx)
		return;
	std::swap(input[startIdx], input[endIdx]);
	reverseRecursive(input, startIdx+1, endIdx-1);
}

void ReverseSentence2(std::string& input)
{
	if (input.empty())
		return;
	std::reverse(input.begin(), input.end());
	int start = 0;
	for (int end = input.find(' ', start); end != std::string::npos; start = end + 1, end = input.find(' ', start))
	{
		std::reverse(input.begin() + start, input.begin() + end);
	}
	std::reverse(input.begin() + start, input.end());
}

void ReverseSentence(std::string& input)
{
	if (input.empty())
		return;
	reverse(input, 0, input.size() - 1);
	int startSentence = 0, endSentence = 0;
	while (endSentence = input.find(' ', startSentence), endSentence != std::string::npos)
	{
		reverse(input, startSentence, endSentence - 1);
		startSentence = endSentence + 1;
	}
	reverse(input, startSentence, input.size() - 1);
}
/*
* Keep on netting out the 1's and 0's and save the sum in a map next to index.
* If we find a sum more than once it means the number of 0's is equal to the number of 1's traversed between current index and 
* previous index saved.
*/
int MaxSubarrayOnesAndZero(const std::vector<int>& in)
{
	int max = 0, sum = 0;
	std::unordered_map<int, int> sumMap{ {0, -1}};
	for (int i = 0; i < in.size(); ++i)
	{
		sum += in[i] == 0 ? -1 : 1;
		auto iter = sumMap.find(sum);
		//Could use insert and check second value in pai to determine existence
		if (iter != sumMap.end())
		{
			max = std::max(max, i - iter->second);
		}
		else
		{
			sumMap[sum] = i;
		}
	}
	return max;
}

int MaxSubarrayOnesAndZero2(const std::vector<int>& in)
{
	int count = 0;
	int max = 0;
	std::unordered_map<int, int> map{ {0, -1} };
	for (int i = 0; i < in.size(); ++i)
	{
		count += in[i] ? 1 : -1;
		const auto& [iter, inserted] = map.insert({ count, i });
		if (!inserted)
		{
			max = std::max(max, i - iter->second);
		}
	}
	return max;
}

int MaxNonRepeatingSubString(const std::string& in)
{
	int max = 0, start = 0, end = 0;
	bool seenMap[256] = { false };
	while (end < in.size())
	{
		if (!seenMap[in[end]])
		{
			max = std::max(max, end - start + 1);
			seenMap[in[end]] = true;
			++end;
		}
		else
		{
			seenMap[in[start]] = false;
			++start;
		}
	}
	return max;
}

std::string MultiplyStrings(const std::string& num1, const std::string& num2)
{
	const int len1 = num1.size();
	const int len2 = num2.size();
	std::string res(len1 + len2, '0');
	for (int i = len1 - 1; i >= 0; --i)
	{
		int carry = 0;
		for (int j = len2 - 1; j >= 0; --j)
		{
			const int value = (num1[i] - '0') * (num2[j] - '0') + (res[i + j + 1] - '0') + carry;
			res[i + j + 1] = value % 10 + '0';
			carry = value / 10;
		}
		std::cout << "I bet u are zero " << res[i] << std::endl;
		res[i] += carry;
	}
	auto idx = res.find_first_not_of('0');
	if (idx == std::string::npos)
		return "0";
	return res.substr(idx);
}
int RemoveDupes(std::vector<int>& input)
{
	int end = 1;
	for (int i = 1; i < input.size(); ++i)
	{
		int j = 0;
		while (j < end)
		{
			if (input[j] == input[i])
				break;
			++j;
		}
		if (i == j)
			input[end++] = input[i];
	}
	return end;
}

/*
* We dont know if first is the near side or not. Therein lies a bit of complication
*/
bool rectanglesDontIntersect(const Rect& first, const Rect& second)
{
	return (first.x + first.width < second.x) ||
		(second.x + second.width < first.x) ||
		(first.y + first.height < second.y) ||
		(second.y + second.height < first.y);
}

/*
* 2 rectangles do not intersect if EITHER of the following is true
* The x points do not intersect i.e end of first on xaxis is < start of second on xaxis.
* The y points do not intersect i.e end of girst on yaxis is < start of second on yaxis.
*/
Rect RectangleIntersection(const Rect& first, const Rect& second)
{
	if (rectanglesDontIntersect(first, second))
		return {0,0,-1,-1};
	return {
		std::max(first.x, second.x),
		std::max(first.y, second.y),
		std::min(first.x + first.width, second.x + second.width) - std::max(first.x, second.x),
		std::min(first.y + first.height, second.y + second.height) - std::max(first.y, second.y)
	};
}

bool DoPointsFormARectangle(const Point& A, const Point& B, const Point& C, const Point& D)
{
	//We actually dont need the sqrt for our purposes
	const auto l1 = pow(A.x - B.x, 2) + pow(A.y - B.y, 2);
	const auto l2 = pow(A.x - C.x, 2) + pow(A.y - C.y, 2);
	const auto l3 = pow(A.x - D.x, 2) + pow(A.y - D.y, 2);
	const auto l4 = pow(B.x - C.x, 2) + pow(B.y - C.y, 2);
	const auto l5 = pow(B.x - D.x, 2) + pow(B.y - D.y, 2);
	const auto l6 = pow(C.x - D.x, 2) + pow(C.y - D.y, 2);
	std::vector<double> lines{l1, l2, l3, l4, l5, l6};
	std::sort(lines.begin(), lines.end());
	//Comparison of floating points is not precise
	return lines[0] == lines[1] && lines[2] == lines[3] && lines[4] == lines[5];
}

//If the points are integers then we can use the following
bool DoPointsFormARectangle2(const Point& A, const Point& B, const Point& C, const Point& D)
{
	//We actually dont need the sqrt for our purposes
	const int l1 = pow(A.x - B.x, 2) + pow(A.y - B.y, 2);
	const int l2 = pow(A.x - C.x, 2) + pow(A.y - C.y, 2);
	const int l3 = pow(A.x - D.x, 2) + pow(A.y - D.y, 2);
	const int l4 = pow(B.x - C.x, 2) + pow(B.y - C.y, 2);
	const int l5 = pow(B.x - D.x, 2) + pow(B.y - D.y, 2);
	const int l6 = pow(C.x - D.x, 2) + pow(C.y - D.y, 2);
	return (l1 ^ l2 ^ l3 ^ l4 ^ l5 ^ l6) == 0;
}

std::string nextString(const std::string& s)
{
	std::string result{};
	for (int i = 0; i < s.size(); ++i)
	{
		int count = 1;
		while ((i + 1) < s.size() && s[i] == s[i + 1])
		{
			++count;
			++i;
		}
		result.push_back('0' + count);
		result.push_back(s[i]);
	}
	return result;
}

std::string LookAndSay(int n)
{
	std::string seed{ "1" };
	for (int i = 1; i < n; ++i)
	{
		seed = nextString(seed);
		std::cout << i << "th value = " << seed << std::endl;
	}
	return seed;
}

bool is_valid_ip(const std::string& part)
{
	//stick an assert that size cant be 0.
	return part.length() < 4 && (part[0] != '0' || part.length() == 1) && stoi(part) <= 255;
}

/*
* Also break once the number of chars is > 3 since max is 255. Yet to be implemented. DOne
*/
std::vector<std::string> GetValidIpAddress(const std::string& input)
{
	std::vector<std::string> res{};
	for (int i = 0; i < input.size() - 3 && i < 3; ++i)
	{
		for (int j = i + 1; j < input.size() - 2 && j - i <= 3; ++j)
		{
			for (int k = j + 1; k < input.size() - 1 && k - j <= 3; ++k)
			{
				int l = k + 1;
				const std::string first{ input.substr(0, i + 1) };
				const std::string second{ input.substr(i + 1, j - i) };
				const std::string third{ input.substr(j + 1, k - j) };
				const std::string fourth{ input.substr(l) };
				if (is_valid_ip(first) && is_valid_ip(second) && is_valid_ip(third) && is_valid_ip(fourth))
					res.push_back(first + "." + second + "." + third + "." + fourth);
			}
		}
	}
	return res;
}

std::string RunlengthEncode(const std::string& in)
{
	std::string res{};
	for (int i = 0; i < in.size(); ++i)
	{
		int count = 1;
		while ((i + 1) < in.size() && in[i] == in[i + 1])
		{
			++count;
			++i;
		}
		//Can use push_back as += uses push_back anyway
		res += ('0' + count);
		res += in[i];
	}
	return res;
}

/*
* Alternatively one would use isdigit to 
*/
std::string RunlengthDecode(const std::string& in)
{
	std::string result{};
	if (in.size() % 2 != 0)
		return result;

	for (int i = 0; i < in.size(); i += 2)
	{
		int count = in[i] - '0';
		result.append(count, in[i + 1]);
	}
	return result;
}

/*
* U dont need to test both char pointers
*/
int strcmp(const char* one, const char* two)
{
	while (*one && *one == *two)
	{
		++one;
		++two;
	}
	return static_cast<unsigned int>(*one) - static_cast<unsigned int>(*two);
}
/*
* 1. find_first_of is the c++ equivalent of c's strbrk
* 2. std::string::substr takes an index as first arg and a length
*/
std::vector<std::string> Split(const std::string& in, char delimeter, char escape)
{
	std::vector<std::string> res{};
	if (in.empty())
		return res;
	int startIdx = 0;
	std::string token;
	const std::string delOrSep = {escape, delimeter};
	auto delOrSepIdx = in.find_first_of(delOrSep, startIdx);
	while (delOrSepIdx != std::string::npos)
	{
		if (in[delOrSepIdx] == escape)
		{
			token.append(in.substr(startIdx, delOrSepIdx - startIdx));
			++delOrSepIdx; //skip past the escape char to append char after it.
			if (delOrSepIdx >= in.size() || in[delOrSepIdx] != delimeter)
				throw std::invalid_argument("A delimeter must follow the escape");
			token += in[delOrSepIdx]; //Add escaped item
		}
		else
		{
			if (!token.empty())
			{
				token.append(in.substr(startIdx, delOrSepIdx - startIdx));
				res.push_back(token);
				token.clear();
			}
			else
			{
				res.push_back(in.substr(startIdx, delOrSepIdx - startIdx));
			}
		}
		startIdx = delOrSepIdx + 1;

		if (startIdx >= in.size())
		{
			/*
			* startIdx could be past end in "Val\;"
			* startIdx could be past end in "Val;"
			*/
			if (!token.empty())
				res.push_back(token);
			return res;
		}
		delOrSepIdx = in.find_first_of(delOrSep, startIdx);
	}


	//I have some parts
	if (!token.empty())
	{
		token.append(in.substr(startIdx, in.size() - startIdx));
		res.push_back(token);
	}
	else
	{
		res.push_back(in.substr(startIdx, in.size() - startIdx));
	}
	return res;
}

std::vector<int> Primes(int n)
{
	std::vector<int> primes{};
	if (n < 2)
		return primes;
	std::deque<bool> primeArray(n + 1, true);
	primeArray[0] = primeArray[1] = false;
	for (int i = 2; i < primeArray.size(); ++i)
	{
		if (primeArray[i])
		{
			primes.push_back(i);
			for (int j = i + i; j < primeArray.size(); j += i)
			{
				primeArray[j] = false;
			}
		}
	}
	return primes;
}

constexpr bool IsPrime(size_t n)
{
	for (size_t d = 2; d <= n / 2; ++d)
	{
		if (n % d == 0)
			return false;
	}
	return n > 1;
}

//fooawawaybla search for away
/*
* The most important take-away here is the std::string::compare.
* Note that std::string::find basically does same thing. U can look for a char or a string.
* Note that empty.find("") returns 0 and not npos so your check is wrong
*/
int FindSubstr1(const std::string& text, const std::string& pattern)
{
	for (int i = 0; text.size() - i >= pattern.size(); ++i)
	{
		const auto found = text.compare(i, pattern.size(), pattern);
		if (found == 0)
			return i;
	}
	return -1;
}

//RabinKarp
/*
* An alternative way of computing the power is base ^ (key_size -1)
* Suffers from int overflow.
* Note that empty.find("") returns 0 and not npos so your check is wrong
*/
int FindSubstr2(const std::string& input, const std::string& key)
{
	if (key.size() > input.size() || input.empty() || key.empty())
		return std::string::npos;
	int power = 1;
	int hash_key = 0, hash_window = 0;
	const int base = 26;
	const int key_size = key.size();

	for (int i = 0; i < key_size; ++i)
	{
		power = i ? power * base : 1;
		hash_key = hash_key * base + key[i];
		hash_window = hash_window * base + input[i];
	}

	if (hash_key == hash_window && input.compare(0, key_size, key) == 0)
	{
		return 0;
	}

	for (int i = key.size(); i < input.size(); ++i)
	{
		//remove input[i - key_size] from hash
		// Add input[i] to hash
		hash_window -= input[i - key_size] * power;
		hash_window = hash_window * base + input[i];
		if (hash_key == hash_window && input.compare(i - key_size + 1, key_size, key) == 0)
		{
			return i - key_size + 1;
		}
	}
	return std::string::npos;
}

std::vector<int> twoSum(const std::vector<int>& nums, int target)
{
	std::unordered_map<int, int> sumMap{};
	for (int i = 0; i < nums.size(); ++i)
	{
		const auto delta = target - nums[i];
		const auto iter = sumMap.find(delta);
		if (iter != sumMap.end())
		{
			return { iter->second , i };
		}
		sumMap.insert({ nums[i], i });
	}
	return {};
}

std::vector<int> twoSumSorted(const std::vector<int>& nums, int target)
{
	int left = 0, right = nums.size() - 1;
	while (left < right)
	{
		const auto sum = nums[left] + nums[right];
		if (sum > target)
			--right;
		else if (sum < target)
			++left;
		else
			return { left, right };
	}
	return {};
}

/*
* In the original 3 sum problem on leetcode, the target is 0
* An added requirement is that the 3 numbers that make up the sum must be distinct.
* To make this generic I will add an explicit target.
* 1. Sort the vector.
* 2. for each item, work out the diff and find a 2 sum using the algo for sorted 2 sums.
* 3. It is complicated with the fact that we dont want any duplicate sets.
*/
std::vector<std::vector<int>> threeSum(std::vector<int>& nums, int target)
{
	std::vector<std::vector<int>> res{};
	if (nums.size() < 3)
		return res;
	std::sort(nums.begin(), nums.end());
	for (int i = 0; i < nums.size() - 2; ++i)
	{
		if (i > 0 && nums[i] == nums[i - 1])
		{
			continue;
		}

		int twosumtarget = target - nums[i];
		int left = i + 1, right = nums.size() - 1;
		while (left < right)
		{
			const int sum = nums[left] + nums[right];
			if (sum == twosumtarget)
			{
				res.push_back({ nums[i], nums[left], nums[right] });
				++left;
				--right;
				while (left < right && nums[left] == nums[left - 1])
				{
					++left;
				}
				while (left < right && nums[right] == nums[right + 1])
				{
					--right;
				}
			}
			else if (sum < twosumtarget)
				++left;
			else
				--right;
		}
	}
	return res;
}

//Fisher-moore's algorithm
void RandomiseArray(std::vector<int>& input)
{
	for (int i = input.size() - 1; i > 0; --i)
	{
		const auto rand_idx = rand() % (i + 1);
		std::swap(input[i], input[rand_idx]);
	}
}

void PrintArrayRotatedLeft(const std::vector<int>& input, int k)
{
	k = k % input.size();
	for (int i = 0; i < input.size(); ++i)
	{
		const int idx = (i + k) % input.size();
		std::cout << input[idx] << " ";
	}
}

void PrintArrayRotatedRight(const std::vector<int>& input, int k)
{
	k = k % input.size();
	const int offset = input.size() - k;
	for (int i = 0; i < input.size(); ++i)
	{
		const int idx = (i + offset) % input.size();
		std::cout << input[idx] << " ";
	}
}

void LeftShiftArray(std::vector<int>& input)
{
	if (input.size() < 2)
		return;
	const auto temp = input.front();
	for (int i = 1; i < input.size(); ++i)
	{
		input[i - 1] = input[i];
	}
	input.back() = temp;
}

void RightShiftArray(std::vector<int>& input)
{
	if (input.size() < 2)
		return;
	const auto temp = input.back();
	for (int i = input.size() - 2; i >= 0; --i)
	{
		input[i + 1] = input[i];
	}
	input.front() = temp;
}

/*
* (, {, [
* Perfect for a stack. remember that a string can be used as a stack<char>
* ([]){()}
*/
bool is_balanced(const std::string& input)
{
	std::stack<char> matches{};
	for (const auto c : input)
	{
		switch (c)
		{
		case '(': matches.push(')'); break;
		case '[': matches.push(']'); break;
		case '{': matches.push('}'); break;
		default: {
			if (matches.empty() || matches.top() != c)
				return false;
			matches.pop();
		}
		}
	}
	return matches.empty();
}

//No extra memory from stack but only works if their is one type of bracket
bool is_balanced2(const std::string s)
{
	int balance = 0;
	for (char c : s)
	{
		if (c == '(')
			++balance;
		else
			--balance;
		if (balance < 0)
			return false;

	}
	return balance == 0;
}
int longestValidParentheses(std::string s) 
{
	int max = 0;
	std::stack<int> stk{};
	stk.push(-1);

	for (int i = 0; i < s.size(); ++i)
	{
		if (s[i] == '(')
		{
			stk.push(i);
		}
		else
		{
			stk.pop();
			if (stk.empty())
				stk.push(i);
			else
			{
				max = std::max(max, i - stk.top());
			}
		}
	}
	return max;
}
/*
* Intuitively this solution lends itself to recursion but the solution in EPI is brilliant
* and also exposes one to how syntaxes are parsed. The use of getline is extremely efficient
* and easy.
*/
int EvaluateRPN(const std::string& expr)
{
	std::unordered_map<std::string, std::function<int(int, int)>> operators{
		{"+", [](int a, int b) {return a + b; }},
		{"-", [](int a, int b) {return a - b; }},
		{"*", [](int a, int b) {return a * b; }},
		{"/", [](int a, int b) {return a / b; }}
	};
	std::stack<int> operands{};
	std::istringstream is{ expr };
	std::string operand;
	while (std::getline(is, operand, ','))
	{
		auto iter = operators.find(operand);
		if (iter != operators.end())
		{
			const auto operand1 = operands.top();
			operands.pop();
			const auto operand2 = operands.top();
			operands.pop();
			operands.push(iter->second(operand1, operand2));
		}
		else
		{
			operands.push(stoi(operand));
		}
	}
	return operands.top();
}
/*
* .. means parent dir
* . means current dir
* when we see a .. we pop the previous dir but the previous dir might be either a / or a ... If its /, its an error
* if its .. then we have nothing to pop.
*/
std::string ShortestDirPath(const std::string& path)
{
	std::string res{};
	if (path.empty())
		return res;
	//I used a std::stack previously but it doesnt have an iterator interface. We'll change to vector which can be used as a stack.
	std::vector<std::string> paths{};
	if (path.front() == '/')
		paths.push_back("/");
	std::string token;
	std::istringstream is{ path };
	while (std::getline(is, token, '/'))
	{
		//check if we can pop the parent
		if (token == "..")
		{
			if (paths.empty() || paths.back() == "..")
			{
				paths.push_back(token);
			}
			else
			{
				if (paths.back() == "/")
					throw std::invalid_argument{ "cant have /.. since root has no parent" };
				paths.pop_back();
			}
		}
		else
		{
			//I am either a . or '' or string
			if (token != "." && !token.empty())
				paths.push_back(token);
		}
	}
	//Need to assemble them at this point.
	if (!paths.empty())
	{
		for (int i = 0; i < paths.size(); ++i)
		{
			if (i == 0 || res == "/")
				res += path[i];
			else
				res += "/" + path[i];
		}
	}
	return res;
}
/*
* This is a problem that is trivial if implemented naively.
* Basically its a linear scan of the sequence comparing to the current min and then current max which is 2N - 1 comparisons
* Comparisons for user types can be expensive. By working on the elements in pairs, we do 3 compares by pair.
* 1. First compare to determine the min and mix of pair.
* 2. Second compare to compare local min with global min.
* 3. Third compare to compare local max with global max.
* 4. Total compares is number of pairs times 3 i.e (input.size / 2) * 3
*/
std::pair<int, int> MinMax(const std::vector<int>& input)
{
	if (input.empty())
		throw std::invalid_argument{"Input is empty"};
	if (input.size() < 2)
		return {input.front(), input.front()};

	int min, max;
	std::tie(min, max) = std::minmax(input[0], input[1]);

	for (int i = 2; i + 1 < input.size(); i += 2)
	{

		const auto [localmin, localmax] = std::minmax(input[i], input[i + 1]);
		min = std::min(min, localmin);
		max = std::max(max, localmax);
	}

	if (input.size() % 2)
	{
		min = std::min(min, input.back());
		max = std::max(max, input.back());
	}
	return { min, max };
}

/*
* The solution is to take the first element in vector as the prefix.
* Next compare this with next string in the vector and use substr to find the common prefix
* Do next for other strings.
*/
std::string longestCommonPrefix(const std::vector<std::string>& strs)
{
	if (strs.empty())
		return "";
	std::string prefix = strs.front();
	if (prefix.empty())
		return prefix;
	for (int i = 1; i < strs.size(); ++i)
	{
		const std::string& element = strs[i];
		int j = 0;
		int len = std::min(prefix.size(), element.size());
		while (j < len && element[j] == prefix[j])
		{
			++j;
		}
		prefix = prefix.substr(0, j);
		if (prefix.empty())
			return "";
	}
	return prefix;
}

/*
* This is an interesting alternative
* Sort the strings and only compare the first and the last.
* Dominated by nlog(n) sort. Much faster than the first solution above.
*/
std::string longestCommonPrefix2(std::vector<std::string>& strs)
{
	if (strs.empty())
		return "";
	if (strs.size() < 2)
		return strs.front();
	std::sort(strs.begin(), strs.end());
	std::string prefix;
	const auto& smallest = strs.front();
	const auto& biggest = strs.back();
	for (int i = 0; i < smallest.size() && smallest[i] == biggest[i]; ++i)
	{
		prefix += smallest[i];
	}
	return prefix;
}

/*
* The first solution uses 2 queues, one for current and one for next.
*/
std::vector<std::vector<int>> PrintLevelsBinTree1(std::shared_ptr<BinaryTree> head)
{
	std::vector<std::vector<int>> res{};
	if (!head)
		return res;
	std::queue<std::shared_ptr<BinaryTree>> current;
	current.push(head);

	while (!current.empty())
	{
		std::vector<int> currentLine{};
		std::queue<std::shared_ptr<BinaryTree>> next;
		while (!current.empty())
		{
			const auto item = current.front();
			currentLine.push_back(item->data);
			current.pop();
			if (item->left)
			{
				next.push(item->left);
			}
			if (item->right)
			{
				next.push(item->right);
			}
		}
		res.push_back(currentLine);
		current = next;
	}
	return res;
}
/*
* Second solution uses a delimiter to demarcate the lines. We use nullptr.
* when we find a null in the queue we know we are starting a new line
*/
std::vector<std::vector<int>> PrintLevelsBinTree2(std::shared_ptr<BinaryTree> head)
{
	std::vector<std::vector<int>> res{};
	if (!head)
		return res;

	std::queue<std::shared_ptr<BinaryTree>> items{};
	items.push(head);
	items.push(nullptr);
	std::vector<int> currentLine{};
	while (!items.empty())
	{
		const auto item = items.front();
		items.pop();
		//If its null, we need to push line, clear line and start next line
		if (!item)
		{
			res.push_back(currentLine);
			currentLine.clear();
			//You get an infinite loop without this line
			if (!items.empty())
				items.push(nullptr);
			continue;
		}

		currentLine.push_back(item->data);
		if (item->left)
		{
			items.push(item->left);
		}
		if (item->right)
		{
			items.push(item->right);
		}
	}
	return res;
}
/*
* Most technical of the 3 solutions. 
* The queue size tells us the number of items at a given level. Buffer this in a variable
* Iterate queue size number of times, which will dequeue all items at current level and leave items enqueued for next level.
*/
std::vector<std::vector<int>> PrintLevelsBinTree3(std::shared_ptr<BinaryTree> head)
{
	std::vector<std::vector<int>> res{};
	if (!head)
		return res;

	std::queue<std::shared_ptr<BinaryTree>> items{};
	items.push(head);

	while (!items.empty())
	{
		auto count = items.size();
		std::vector<int> currentLine{};
		while (count--)
		{
			const auto item = items.front();
			items.pop();
			currentLine.push_back(item->data);

			if (item->left)
			{
				items.push(item->left);
			}
			if (item->right)
			{
				items.push(item->right);
			}
		}
		res.push_back(currentLine);
	}
	return res;
}

void RotateLeft(std::vector<int>& input, int k)
{
	k %= input.size();
	reverse(input, 0, input.size() - 1);
	reverse(input, input.size() - k, input.size() - 1);
	reverse(input, 0, input.size() - k - 1);
}
void RotateRight(std::vector<int>& input, int k)
{
	k %= input.size();
	reverse(input, 0, input.size() - 1);
	reverse(input, 0, k - 1);
	reverse(input, k, input.size() - 1);
}

void RotateRight2(std::vector<int>& input, int k)
{
	if (input.size() < 2)
		return;
	k %= input.size();
	if (k == 0)
		return;
	std::vector<int> tmp(input.size());
	for (int i = 0; i < input.size(); ++i)
	{
		const auto idx = (i + k) % input.size();
		tmp[idx] = input[i];
	}
	input = std::move(tmp);
}
/*
* Keep track of all closed brackets that are orphaned without an open.
*/
int MinSwapsToBalance(const std::string& s)
{
	int opencount = 0, closecount = 0;
	for (char bracket : s)
	{
		if (bracket == '[')
			++opencount;
		else
		{
			if (opencount)
				--opencount;
			else
				++closecount;
		}
	}
	return (closecount + 1) / 2;
}

//Assuming that the string only has lower case.
// in.size() - i >= windowSize
// b -> 7; size = 12
//[asfgtkhbczmn]
int FirstUniqueSubstringWindow(const std::string& in, int windowSize)
{
	int pos = -1;
	if (windowSize > in.size())
		return pos;
	std::bitset<256> chars{};
	for (int i = 0; in.size() - i >= windowSize; ++i)
	{
		chars ^= 1 << (in[i] - 'a');
		if (i >= windowSize)
			chars ^= 1 << (in[i - windowSize] - 'a');
		if (chars.count() == windowSize)
			return i + 1;
	}
	return pos;
}

/*
* Can be used for subarrays and substrings.
* total number of subarrays is n(n+1)/2
*/
void PrintSubArrays(const std::vector<int>& input)
{
	const int len = input.size();
	for (int i = 0; i < len; ++i)
	{
		for (int j = i; j < len; ++j)
		{
			for (int k = i; k <= j; ++k)
			{
				std::cout << input[k] << " ";
			}
			std::cout << std::endl;
		}
	}
}

//.................................................................................
//BinaryTree
//.................................................................................

/*
* The bit that catches me out is the additional checks that left and right are balanced.
*/
bool IsBalanced(const std::shared_ptr<const BinaryTree> root)
{
	if (!root)
		return true;
	return std::abs(Height(root->left) - Height(root->right)) <= 1 && IsBalanced(root->left) && IsBalanced(root->right);
}

int Height(const std::shared_ptr<const BinaryTree> root)
{
	if (!root)
		return 0;
	return std::max(Height(root->left), Height(root->right)) + 1;
}
/*
* Full...balanced....complete and perfect. All qualities of a binary tree.
* A perfect binary tree is one where all the internal nodes are full i.e have both left and right
* and the leaves are all on the same level. Now for the solution.
*/

int Depth(const std::shared_ptr<const BinaryTree> root)
{
	int depth = 0;
	if (!root)
		return 0;
	auto iter = root;
	while (iter)
	{
		++depth;
		iter = iter->left;
	}
	return depth;
}

//Keep on recursing until i find a leaf and check if the height is same as i passed in.
bool IsPerfectHelper(const std::shared_ptr<const BinaryTree> node, int currentHeight, int expectedHeight)
{
	if (!node->left && !node->right)
		return currentHeight + 1 == expectedHeight;
	if (!node->left || !node->right) //can also use node->left ^ node->right
		return false;
	return IsPerfectHelper(node->left, currentHeight + 1, expectedHeight) &&
		IsPerfectHelper(node->right, currentHeight + 1, expectedHeight);
}

bool IsPerfect(const std::shared_ptr<const BinaryTree> root)
{
	if (!root)
		return true;
	const int depth = Depth(root);
	return IsPerfectHelper(root, 0, depth);
}

bool IsComplete(const std::shared_ptr<const BinaryTree> root)
{
	if (!root)
		return true;
	std::queue<std::shared_ptr<const BinaryTree>> q{};
	q.push(root);
	bool end = false;
	while (!q.empty())
	{
		auto node = q.front();
		q.pop();
		if (node->left)
		{
			if (end) return false;
			q.push(node->left);
		}
		else
		{
			end = true;
		}
		if (node->right)
		{
			if (end) return false;
			q.push(node->right);
		}
		else
		{
			end = true;
		}
	}
	return true;
}

bool checkSymetrical(const std::shared_ptr<const BinaryTree> left, const std::shared_ptr<const BinaryTree> right)
{
	if (!left && !right)
		return true;
	if (left && right)
	{
		return left->data == right->data &&
			checkSymetrical(left->right, right->left) &&
			checkSymetrical(left->left, right->right);
	}
	return false;
}

bool IsSymmetrical(const std::shared_ptr<const BinaryTree> root)
{
	if (!root)
		return true;
	return checkSymetrical(root->left, root->right);
}

std::pair<int, std::shared_ptr<const BinaryTree>> getCommonAncestor(const std::shared_ptr<const BinaryTree> root,
	const std::shared_ptr<const BinaryTree> one,
	const std::shared_ptr<const BinaryTree> two)
{
	if (!root)
		return {0, nullptr};
	const auto leftresults = getCommonAncestor(root->left, one, two);
	if (leftresults.first == 2)
		return leftresults;
	const auto rightresults = getCommonAncestor(root->right, one, two);
	if (rightresults.first == 2)
		return rightresults;
	const int count = leftresults.first + rightresults.first + (root == one) + (root == two);
	return {count, count == 2 ? root : nullptr};
}

std::shared_ptr<const BinaryTree> LowestCommonAncestor(const std::shared_ptr<const BinaryTree> root,
	const std::shared_ptr<const BinaryTree> one,
	const std::shared_ptr<const BinaryTree> two)
{
	if (!root)
		return root;
	return getCommonAncestor(root, one, two).second;
}

/*
* In the first impl with nodes that dont have have a pointer to the parent, we start the traversal from the root node.
* This does not take advantage of proximity for example siblings. With the parent info, we can go up alternatingly and 
* add check if we've seen the parent in a hash map.
*/
std::shared_ptr<const BinaryTreeP> LowestCommonAncestor(std::shared_ptr<const BinaryTreeP> one, 
	                                                    std::shared_ptr<const BinaryTreeP> two)
{
	std::unordered_set<std::shared_ptr<const BinaryTreeP>> parents_seen_so_far{};
	while (one || two)
	{
		if (one)
		{
			const auto iterpair1 = parents_seen_so_far.insert(one);
			if (!iterpair1.second)
				return *iterpair1.first;
			one = one->parent;
		}
		if (two)
		{
			const auto iterpair2 = parents_seen_so_far.insert(two);
			if (!iterpair2.second)
				return *iterpair2.first;
			two = two->parent;
		}
	}
	return nullptr;
}

bool HasPathSum(const std::shared_ptr<const BinaryTree> root, int target)
{
	if (!root)
		return false;
	if (!root->left && !root->right)
		return root->data == target;
	return HasPathSum(root->left, target - root->data) || HasPathSum(root->right, target - root->data);
}

int countHelper(const std::shared_ptr<const BinaryTree> subTree)
{
	if (!subTree)
		return 0;
	const auto leftcount = countHelper(subTree->left);
	const auto rightcount = countHelper(subTree->right);
	return leftcount + rightcount + 1;
}

int NumberOfElementsInSubTree(const std::shared_ptr<const BinaryTree> root)
{
	if (!root)
		return 0;
	return countHelper(root->left);
}

std::vector<int> InOrderTraversalIterative(const std::shared_ptr<const BinaryTree> root)
{
	std::vector<int> res{};
	std::stack<std::pair<std::shared_ptr<const BinaryTree>, bool>> to_process{};
	to_process.push({ root, false });
	while (!to_process.empty())
	{
		auto [node, processed] = to_process.top();
		to_process.pop();
		if (node)
		{
			if (processed)
				res.push_back(node->data);
			else
			{
				to_process.push({ node->right, false });
				to_process.push({ node, true });
				to_process.push({ node->left, false });
			}
		}
	}
	return res;
}

/*
* If the Node has a right subtree, then the successor must be in the right sub tree. It is the lefttmost item in the 
* right subtree or the right child if the right subtree has no left subtree.
* if the Node doesnt have a right subtree then there are 2 scenarios
* 1. It is the left subtree of its parent which means successor is the parent.
* 2. It is the right subtree of its parent which means we have to keep traversing iteratively up until we go left.
*/
std::shared_ptr<const BinaryTreeP> FindSuccessor(const std::shared_ptr<const BinaryTreeP> node)
{
	if (!node)
		return node;
	auto iter = node;
	if (iter->right)
	{
		iter = iter->right;
		while (iter->left)
		{
			iter = iter->left;
		}
		return iter;
	}
	while (iter->parent && iter->parent->right == iter)
	{
		iter = iter->parent;
	}
	return iter->parent;
}

void AddLeavesInSubtree(const std::shared_ptr<const BinaryTree> node, std::vector<std::shared_ptr<const BinaryTree>>& list)
{
	if (!node)
		return;
	if (!node->left && !node->right)
	{
		list.push_back(node);
		return;
	}
	AddLeavesInSubtree(node->left, list);
	AddLeavesInSubtree(node->right, list);
}

std::vector<std::shared_ptr<const BinaryTree>> CreateListOfLeaves(const std::shared_ptr<const BinaryTree> root)
{
	std::vector<std::shared_ptr<const BinaryTree>> res{};
	AddLeavesInSubtree(root, res);
	return res;
}
void Leaves(std::shared_ptr<const BinaryTree> node, std::vector<std::shared_ptr<const BinaryTree>>& boundaries)
{
	if (!node)
		return;
	if (!node->left && !node->right)
		boundaries.push_back(node);
	else
	{
		Leaves(node->left, boundaries);
		Leaves(node->right, boundaries);
	}
}

/*
* If you have a leftnode, collect it.
* If there is no leftnode, go right
* Do not collect the leaf as the Leaves function will get it.
*/
void LeftBoundary(std::shared_ptr<const BinaryTree> node, std::vector<std::shared_ptr<const BinaryTree>>& boundaries)
{
	if (!node)
		return;
	if (!node->left && !node->right)
		return;
	boundaries.push_back(node);
	if (node->left)
		LeftBoundary(node->left, boundaries);
	else
		LeftBoundary(node->right, boundaries);
}

void RightBoundary(std::shared_ptr<const BinaryTree> node, std::vector<std::shared_ptr<const BinaryTree>>& boundaries)
{
	if (!node)
		return;
	if (!node->left && !node->right)
		return;
	boundaries.push_back(node);
	if (node->right)
		RightBoundary(node->right, boundaries);
	else
		RightBoundary(node->left, boundaries);
}

std::vector<std::shared_ptr<const BinaryTree>> ExteriorBinaryTree(const std::shared_ptr<const BinaryTree> root)
{
	std::vector<std::shared_ptr<const BinaryTree>> boundaries{};
	if (!root)
		return boundaries;
	boundaries.push_back(root);
	LeftBoundary(root->left, boundaries);
	Leaves(root->left, boundaries);
	Leaves(root->right, boundaries);
	RightBoundary(root->right, boundaries);
	return boundaries;
}

/*
* You are given a string number representing a positive integer and a character digit.
* Return the resulting string after removing exactly one occurrence of digit from number such 
* that the value of the resulting string in decimal form is maximized.
* You solved this one but had little confidence.
* Go from left to right and if you find a digit greater than the digit in question, thats the one to remove.
* If we traverse the end and havent found one where the next digit is greater, then delete the last occurence.
*/
std::string removeDigit(std::string number, char digit) 
{
	int last_idx = 0;
	for (int i = 0; i < number.size(); ++i)
	{
		if (number[i] == digit)
		{
			last_idx = i;
			int one_past = i + 1;
			if (one_past < number.size() && number[one_past] > number[i])
			{
				//Found a candidate so break.
				break;
			}
		}
	}
	number.erase(last_idx, 1);
	return number;
}
/*
* A priority queue is a bit odd. Think of the ordering being from top to bottom of a vector but the min or max being
* at the end of the vector. If u want a min heap, then the largest items will be at top which means that ur comparator
* should return true for the biggest items.
* Now this solution is elegant but the one I first came up with could avoid insertions if the string is less than the smallest.
* It could also avoid deletions both of which take O(LogN). Lets implement that also.
*/
std::vector<std::string> TopK(const std::string& stream, int k)
{
	std::vector<std::string> res{};
	std::istringstream is{ stream };
	std::string token{};
	const auto cmp = [](const std::string& lhs, const std::string& rhs) {return lhs.size() > rhs.size(); };
	std::priority_queue<std::string, std::vector<std::string>, decltype(cmp)> min_queue{ cmp };
	while (is >> token)
	{
		min_queue.push(token);
		if (min_queue.size() > k)
		{
			min_queue.pop();
		}
	}

	while (!min_queue.empty())
	{
		res.push_back(min_queue.top());
		min_queue.pop();
	}
	return res;
}

/*
* You save on the cost of always inserting and deleting which both take O(LogN)
*/
std::vector<std::string> TopK1(const std::string& stream, int k)
{
	std::vector<std::string> res{};
	std::istringstream is{ stream };
	std::string token{};
	auto cmp = [](const std::string& lhs, const std::string& rhs) {return lhs.size() > rhs.size(); };
	std::priority_queue<std::string, std::vector<std::string>, decltype(cmp)> min_queue{ cmp };
	while (is >> token)
	{
		if (min_queue.size() == k)
		{
			const auto top = min_queue.top();
			if (token.size() <= top.size())
				continue;
			min_queue.pop();
		}
		min_queue.push(token);
	}

	while (!min_queue.empty())
	{
		res.push_back(min_queue.top());
		min_queue.pop();
	}
	return res;
}

/*
* The usual solution is to use a priority_queue.
* The tricky part of the algorithm is to determine the current position we are in each of the vectors
* because they vectors can be different lengths. There are 2 solutions so far
* 1. Store the current iter and the end in an object which is stored in priority queue.
* 2. Store an index that denotes each vectors position in multidim, an index to current and the current value.
* Note that by default a std::priority_queue is a max heap. Use std::greater for a min heap or custom comparator.
* The cost of inserts and deletes are O(Log k) where k is the number of elements in the heap. K here is the number
* of arrays since we would have at must k entries. Now we insert N items where N is the total number of elements in all
* arrays so the runtime is O(N Logk). The space requirement for the priority queue is O(k).
*/
std::vector<int> MergeKSortedArrays(const std::vector<std::vector<int>>& arrays)
{
	std::vector<int> res{};
	using Iter = decltype(arrays[0].begin());
	using IterPair = std::pair<Iter, Iter>;
	const auto comp = [](const IterPair& lhs, const IterPair& rhs) {return *(lhs.first) > *(rhs.first); };
	std::priority_queue<IterPair, std::vector<IterPair>, decltype(comp)> min_q{comp};
	for (const auto& array: arrays)
	{
		if (!array.empty())
		{
			min_q.push({ array.begin(), array.end()});
		}
	}
	while (!min_q.empty())
	{
		const auto top = min_q.top();
		min_q.pop();
		res.push_back(*(top.first));
		auto next = std::next(top.first);
		if (next != top.second)
		{
			min_q.push({ next, top.second });
		}
	}
	return res;
}

auto MergeKSortedArrays(const std::vector<std::shared_ptr<ListNode<int>>>& arrays) -> decltype(arrays[0])
{
	using value_type = std::shared_ptr<ListNode<int>>;

	value_type dummy = std::make_shared<ListNode<int>>();
	auto tail = dummy;

	const auto comp = [](const value_type lhs, const value_type rhs) {return lhs->data > rhs->data; };
	std::priority_queue< value_type, std::vector<value_type>, decltype(comp)> min_queue{comp};
	for (const auto i : arrays)
	{
		if (i)
			min_queue.push(i);
	}

	while (!min_queue.empty())
	{
		auto top = min_queue.top();
		min_queue.pop();
		tail->next = top;
		auto next = top->next;
		if (next)
			min_queue.push(next);
		tail = tail->next;
	}
	return dummy->next;
}

/*
* Extract each subsequence, ascending and descending and insert into a vector. Note that the descending must be reversed.
* We assume we start with ascending. Notice how we use is_sorted_until with comparator.
* Notice how u used std::make_reverse_iterator.
*/
std::vector<int> SortKIncresingDecreasingArray(const std::vector<int>& input)
{
	bool ascending = true;
	auto start = input.begin();
	std::vector<std::vector<int>> sorted_arrays{};
	while (start != input.end())
	{
		if (ascending)
		{
			auto end = std::is_sorted_until(start, input.end());
			std::vector<int> tmp{start, end};
			sorted_arrays.push_back(std::move(tmp));
			ascending = false;
			start = end;
		}
		else
		{
			auto end = std::is_sorted_until(start, input.end(), std::greater<>{});
			std::vector<int> tmp{ std::make_reverse_iterator(end), std::make_reverse_iterator(start) };
			sorted_arrays.push_back(std::move(tmp));
			ascending = true;
			start = end;
		}
	}
	return MergeKSortedArrays(sorted_arrays);
}

/*
* The Brute force solution requires a runtime of O(m + n) and extra space of O(m + n)
* 
*/
double findMedianSortedArrays(const std::vector<int>& nums1, const std::vector<int>& nums2)
{
	std::vector<int> merged{};
	const int total = nums1.size() + nums2.size();
	merged.reserve(total);
	auto iter1 = nums1.cbegin();
	auto iter2 = nums2.cbegin();
	while (iter1 != nums1.cend() && iter2 != nums2.cend())
	{
		if (*iter1 < *iter2)
		{
			merged.push_back(*iter1);
			++iter1;
		}
		else
		{
			merged.push_back(*iter2);
			++iter2;
		}
	}
	std::copy(iter1, nums1.cend(), std::back_inserter(merged));
	std::copy(iter2, nums2.cend(), std::back_inserter(merged));
	
	if (total % 2)
		return merged[total / 2];
	else
		return 0.5 * (merged[total / 2] + merged[total / 2 - 1]);
}

/*
* The binary search option is where you earn your stripes.
* 1. The goal is to find 2 partitions on either vector such that the number of elements on each half are equal.
* 2. All the elements on the left side must be less than elements on the right side.
* 3. The partitioning is done on the smaller vector.
* 4. Time : O(log(min(n, m))
* 5. Space: O(1)
*/
double findMedianSortedArrays1(const std::vector<int>& nums1, const std::vector<int>& nums2)
{
	int const len1 = nums1.size(), len2 = nums2.size();
	if (len1 > len2)
		return findMedianSortedArrays1(nums2, nums1);
	int left = 0, right = len1;
	while (left <= right)
	{
		const int partition1 = left + (right - left) / 2;
		const int partition2 = (len1 + len2 + 1) / 2 - partition1;
		const int left1 = partition1 == 0 ? std::numeric_limits<int>::min() : nums1[partition1 - 1];
		const int left2 = partition2 == 0 ? std::numeric_limits<int>::min() : nums2[partition2 - 1];
		const int right1 = partition1 == len1 ? std::numeric_limits<int>::max() : nums1[partition1];
		const int right2 = partition2 == len2 ? std::numeric_limits<int>::max() : nums2[partition2];

		if (left1 <= right2 && left2 <= right1)
		{
			if ((len1 + len2) % 2)
			{
				return std::max(left1, left2);
			}
			else
			{
				return 0.5 * (std::max(left1, left2) + std::min(right1, right2));
			}
		}
		else if (left1 > right2)
		{
			right = partition1 - 1;
		}
		else
		{
			left = partition1 + 1;
		}
	}
	return -1;
}

std::vector<std::string> topKFrequent(const std::vector<std::string>& words, int k)
{
	std::unordered_map<std::string, int> histogram{};
	for (const auto& item : words)
	{
		++histogram[item];
	}
	using value_type = std::pair<int, std::string>;
	const auto cmp = [](const value_type& lhs, const value_type& rhs)
	{
		return lhs.first > rhs.first ||
			(!(rhs.first > lhs.first) && lhs.second < rhs.second);
	};
	std::priority_queue<value_type, std::vector<value_type>, decltype(cmp)> queue{ cmp };
	for (const auto& i : histogram)
	{
		queue.push({ i.second, i.first });
		if (queue.size() > k)
			queue.pop();
	}
	std::vector<std::string> res{};
	while (!queue.empty())
	{
		const auto item = queue.top();
		queue.pop();
		res.push_back(item.second);
	}
	std::reverse(res.begin(), res.end());
	return res;
}

/*
* The Eclidean distance is the distance between 2 points.
* In this case, one of the points is at the origin.
* For 2 dimensional plane, the distance is sqrt[(x1 - x2)2 + (y1 - y2)2]
* For 3 dimensional plane, the distance is sqrt[(x1 - x2)2 + (y1 - y2)2 + (z1 - z2)2)]
*/
std::vector<std::vector<int>> kClosest(std::vector<std::vector<int>>& points, int k)
{
	const auto cmp = [](const auto& lhs, const auto& rhs)
	{
		const auto d1 = sqrt(pow(lhs[0], 2) + pow(lhs[1], 2));
		const auto d2 = sqrt(pow(rhs[0], 2) + pow(rhs[1], 2));
		return d1 < d2;
	};
	std::priority_queue<std::vector<int>, std::vector<std::vector<int>>, decltype(cmp)> pq{ cmp };
	for (const auto& i : points)
	{
		pq.push(i);
		if (pq.size() > k)
			pq.pop();
	}
	std::vector<std::vector<int>> res{};
	while (!pq.empty())
	{
		res.push_back(pq.top());
		pq.pop();
	}
	return res;
}

double RunningMedian(std::vector<int>::const_iterator begin, std::vector<int>::const_iterator end)
{
	std::priority_queue<int> maxq{};
	std::priority_queue<int, std::vector<int>, std::greater<>> minq{};
	while (begin != end)
	{
		std::cout << "Adding elem " << *begin << std::endl;
		if (maxq.size() == minq.size())
		{
			/*
			* Need to add to lower half aka maxq.
			* We cant just add directly maxq = [1, 15, 31] minq = [50, 64, 100] adding 70 directly to maxq breaks sorted property
			* To solve this we add it to minq to sort then we add the min to the maxq
			*/
			minq.push(*begin);
			maxq.push(minq.top());
			minq.pop();
		}
		else
		{
			/*
			* The maxq has one elem more so add to minq.
			* Again we cant just add direct to minq since it might break sorted property
			* Add to maxq first to sort and then to minq.
			*/
			maxq.push(*begin);
			minq.push(maxq.top());
			maxq.pop();
		}
		std::cout << "Running media = " << (maxq.size() == minq.size() ? 0.5 * (maxq.top() + minq.top()) : maxq.top()) << std::endl;
		++begin;
	}
	return maxq.size() == minq.size() ? 0.5 * (maxq.top() + minq.top()) : maxq.top();
}

/*
* There are few bad ways to get the K largest items in a heap.
* 1. Pop the heap k times....thereby modify the heap irreversibly
* 2. Copy the entire heap and perform (1) on a copy. What if the heap has millions of items?
* A heap represented in an array has some properties that can be exploited
* The left child if any exists at 2i + 1
* The right child if any exists at 2i + 2
* Where i is the index of the parent. The largest value is also at index 0.
*/
std::vector<int> KLargestInBinaryHeap(const std::vector<int>& heap, int k)
{
	std::vector<int> res{};
	return res;
}

/*
* We have some data with a special property that each item is at most k distance away from where it should exist when sorted.
* We can use a priority queue of size k.
*/
std::vector<int> SortApproximatelySortedData(std::vector<int>::const_iterator begin, const std::vector<int>::const_iterator end, int k)
{
	//The default comparator in a std::priority_queue is std::less which creates a max_heap.
	//We need a min_heap since we want to pluck the smallest elements at O(logn) time.
	std::priority_queue<int, std::vector<int>, std::greater<>> min_q{};
	auto tmpk = k;
	while (begin != end && tmpk--)
		min_q.push(*begin++);
	std::vector<int> res{};
	while (begin != end)
	{
		min_q.push(*begin++);
		const auto smallest = min_q.top();
		min_q.pop();
		res.push_back(smallest);
	}
	while (!min_q.empty())
	{
		const auto smallest = min_q.top();
		min_q.pop();
		res.push_back(smallest);
	}
	return res;
}

int BinarySearch(const std::vector<int>& input, int key)
{
	int left = 0;
	int right = input.size() - 1;
	while (left <= right)
	{
		int mid = left + (right - left) / 2;
		if (input[mid] < key)
			left = mid + 1;
		else if (input[mid] == key)
			return mid;
		else
			right = mid - 1;
	}
	return -1;
}

/*
Each record has the same length in bytes and each is on its own line.
Each key is say 4 bytes from the start of the line.
*/
std::string BinarySearchSortedFile(const std::string& fname, const std::string& key)
{
	constexpr size_t bytesPerRecord{ 50 };
	constexpr size_t keySize{ 10 };
	std::string out{};
	std::ifstream file{fname};
	if (!file.is_open())
		return out;
	out.reserve(bytesPerRecord - keySize);
	std::filesystem::path p{ fname };
	const int lines = std::filesystem::file_size(p) / bytesPerRecord;

	int lineStart = 0, lineEnd = lines;
	while (lineStart <= lineEnd)
	{
		const int mid = lineStart + (lineEnd - lineStart) / 2;
		file.seekg(mid*bytesPerRecord);
		std::getline(file, out);
		const int comp = out.compare(0, keySize, key);
		if (comp == 0)
		{
			return out.substr(keySize + 1);
		}
		else if (comp < 0)
			lineStart = mid + 1;
		else
			lineEnd = mid - 1;
	}
	return out;
}

/*
* Is this equivalent to a lower bound then?
* The solution is to record the index of a match and eliminate right hand side. 
* If we find another match it will be towards the left so closer to the first.
* I can merge the if and else branch. Done
*/
int FindFirstOfK(const std::vector<int>& input, int key)
{
	int left = 0;
	int right = input.size() - 1;
	int result = -1;
	while (left <= right)
	{
		int mid = left + (right - left) / 2;
		if (input[mid] >= key)
		{
			if (input[mid] == key)
				result = mid;
			right = mid - 1;
		}
		else 
			left = mid + 1;
	}
	return result;
}

int FindLastOfK(const std::vector<int>& input, int key)
{
	int left = 0;
	int right = input.size() - 1;
	int result = -1;
	while (left <= right)
	{
		int mid = left + (right - left) / 2;
		if (input[mid] <= key)
		{
			if (input[mid] == key)
				result = mid;
			left = mid + 1;
		}
		else
			right = mid - 1;
	}
	return result;
}

/*
* This is an upper_bound.
* We use FindLastOfK and return the last index
*/
int FindFirstGreaterThanK(const std::vector<int>& input, int key)
{
	const auto lastidx = FindLastOfK(input, key);
	if (lastidx == -1)
		return lastidx;
	if (lastidx == input.size() - 1)
		return -1;
	return lastidx + 1;
}

std::pair<int, int> EqualRange(const std::vector<int>& input, int key)
{
	return { FindFirstOfK(input, key), FindLastOfK(input, key) };
}

int FindFirstOfK(const std::vector<std::string>& products, const std::string& searchWord)
{
	int left = 0, right = products.size() - 1, res = -1;
	while (left <= right)
	{
		const int mid = left + (right - left) / 2;
		const int found = products[mid].compare(0, searchWord.size(), searchWord);
		if (found >= 0)
		{
			if (found == 0)
				res = mid;
			right = mid - 1;
		}
		else
			left = mid + 1;
	}
	return res;
}
std::vector<std::vector<std::string>> suggestedProducts(std::vector<std::string>& products, const std::string& searchWord) 
{
	std::vector<std::vector<std::string>> res{};
	std::sort(products.begin(), products.end());
	for (int i = 0; i < searchWord.size(); ++i)
	{
		const auto key = searchWord.substr(0, i + 1);
		const int idx = FindFirstOfK(products, key);
		std::vector<std::string> tmp{};
		if (idx != -1)
		{
			const int maxbound = idx + 3;
			for (int j = idx; j < maxbound && j < products.size(); ++j)
			{
				if (products[j].compare(0, key.size(), key) != 0)
					break;
				tmp.push_back(products[j]);
			}
		}
		res.push_back(tmp);
	}
	return res;
}

bool IsPrefixInSortedStringArray(const std::vector<std::string>& input, const std::string& prefix)
{
	int left = 0, right = input.size() - 1 ;
	while (left <= right)
	{
		int mid = left + (right - left) / 2;
		int cmp = input[mid].compare(0, prefix.size(), prefix);//why not use the string compare
		if (cmp > 0)
			left = mid + 1;
		else if (cmp == 0)
			return true;
		else
			right = mid - 1;
	}
	return false;
}

/*
* In a left or right rotated sequence, one half will be sorted and the other half won't be
* It is easy to see that the smallest will exist in the half that is not sorted. There are 2 nuances though
* 1. if input[mid] > input[right], then smallest is in range [mid + 1, right].
* 2. Otherwise it must be in other half but the mid could be smallest. so range is [left, mid]
* 3. Why do we have left <= right in most? Because of ranges with one element.
*/
int FindSmallestInSortedRotated(const std::vector<int>& input)
{
	int left = 0, right = input.size() - 1;
	//Break early if not rotated
	if (input[left] < input[right])
		return left;

	while (left < right)
	{
		int mid = left + (right - left) / 2;
		if (input[mid] > input[right])
			left = mid + 1;
		else
			right = mid;
	}
	return left;
}

/*
* I initially didnt have the input[left] == input[mid] check
* and realised that while testing mid could be equal to left
* and decremented mid to get right which obviously swept pass the correct answer.
*/
int FindLargestInSortedRotated(const std::vector<int>& input)
{
	int left = 0, right = input.size() - 1;
	while (left < right)
	{
		int mid = left + (right - left) / 2;
		if (input[left] < input[mid])
			left = mid;
		else if (input[left] == input[mid])
			++left;
		else
			right = mid - 1;
	}
	return left;
}
/*
* Its all about finding which half of the sequence to eliminate.
* The general idea is to find a sorted half. 3 months ago I would have maintained that in a rotated sorted, we
* would always have one half sorted and the other non-sorted. It has become clear that we can both have 2 sorted halves
* So we check if right is sorted
* 1. if so, check if our key lies in the boundary and move the left.
* 2. If not then our key must be in other half so move right.
* If the right half is not sorted and therefore the left is
* 1. check if our key is within left and mid. if so move right.
* 2. If not then key must be in right half and move left
*/
int SearchInSortedRotated(const std::vector<int>& nums, int target)
{
	int result = -1;
	if (nums.empty())
		return result;
	int left = 0;
	int right = nums.size() - 1;
	while (left <= right)
	{
		int mid = left + (right - left) / 2;
		if (nums[mid] == target)
		{
			return mid;
		}
		else if (nums[mid] <= nums[right])
		{
			if (target > nums[mid] && target <= nums[right])
				left = mid + 1;
			else
				right = mid - 1;
		}
		else
		{
			if (target >= nums[left] && target < nums[mid])
				right = mid - 1;
			else
				left = mid + 1;
		}
	}
	return result;
}
/*
* 1. if matrix[row][col] == k, return it.
* 2. if k > matrix[row][col] increment row as tthe value cant be on that row.
* 3. if k < matrix[row][col], decrement col as that value cnt be on that col.
*/
bool MatrixSearch(const std::vector<std::vector<int>>& matrix, int k)
{
	int row = 0, col = matrix[0].size() - 1;
	while (row < matrix.size() && col >= 0)
	{
		if (matrix[row][col] == k)
			return true;
		else if (matrix[row][col] < k)
			++row;
		else
			--col;
	}
	return false;
}

/*
* The brute force approach is to test squaring from 0 to i to see if it equals i.
* This solution is linear in i.
* So, we have a key space from [0 - i] and we want to find out which half the result will exist in.
* If mid ^ 2 is greater than i then we eliminate rhs etc.
*/
int SquareRoot(int i)
{
	int left = 0, right = i;
	while (left < right)
	{
		int mid = left + (right - left) / 2;
		int squared = mid * mid;
		if (squared < i)
			left = mid + 1;
		else if (squared == i)
			return mid;
		else
			right = mid - 1;
	}
	return left - 1;
}

bool SearchInSortedArray(const std::vector<std::vector<int>>& input, int k)
{
	int row = 0;
	int col = input[0].size() - 1;
	while (row < input.size() && col >= 0)
	{
		if (input[row][col] == k)
			return true;
		if (input[row][col] < k)
			++row;
		if (row < input.size() && input[row][col] > k)
			--col;
	}
	return false;
}

int GenRandomIntInRange(int left, int right)
{
	if (left >= right)
		throw std::invalid_argument{"low must be less than high"};
	const int diff = right - left;
	return left + (rand() % (diff + 1));
}

//This partition moves greater
int Partition(std::vector<int>& nums, int left, int right)
{
	int partition = left;
	const int pivotidx = GenRandomIntInRange(left, right);
	const int pivot = nums[pivotidx];
	std::swap(nums[pivotidx], nums[right]);
	for (int i = left; i < right; ++i)
	{
		if (nums[i] > pivot)
		{
			std::swap(nums[i], nums[partition++]);
		}
	}
	std::swap(nums[partition], nums[right]);
	return partition;
}

/*
* Should we check size of array is at least k?
* Lets analyse the runtime of this algorithm.
* We start by partitioning the entire array and if subsequent partitions are central the runtime becomes
* N + N/2 + N/4 + N/8 +...+ 1 = 2N.
* This runtime is contigent on the pivot between middle-ish. If the smallest or largest item is selected, then it essentially becomes O(N^2)
*/
int FindKthLargest(std::vector<int>& nums, int k)
{
	int left = 0;
	int right = nums.size() - 1;
	while (left <= right)
	{
		const auto partition = Partition(nums, left, right);
		if (partition == k - 1)
			return nums[partition];
		else if (partition > k - 1)
			right = partition - 1;
		else
			left = partition + 1;
	}
	return -1;
}

/*
* We have a billion ip addresses stored in file and we dont have enough RAM to load the file into main memory. An IP is a 32 bit int.
* 1. Sorting the elements and then traversing the elements looking for gaps comes to mind but this would require 4bytes X 10^9 = 4GB. Merge sort can do it externally though.
* 2. Adding all the elements into a map and then checking for every ip address until we get a miss. map still required at least 4GB.
* 3. Whenever we want to record the presence of something without requiring to retrieve the value, consider a bit map. for a 32 bit 
*    word, we have 2^32 permutations which is about 500 MB. No bueno.
* 4. Each permutation of the MSB has a fixed number of occurences given by the number of bits. it is 2 ^ (N/2) or 1 << (N/2).
*/
int FindMissingElement(std::vector<int>::const_iterator start, const std::vector<int>::const_iterator end)
{
	auto begin_cpy = start;
	const auto buckets = 1 << 16;
	std::vector<size_t> histogram(buckets);

	while (start != end)
	{
		const auto entry = *start++;
		const auto msb = entry >> 16;
		++histogram[msb];
	}
	//One bucket will be less than 1 << 16. Find it
	int short_fall{};
	for (int i = 0; i > histogram.size(); ++i)
	{
		if (histogram[i] < buckets)
		{
			short_fall = i;
			break;
		}
	}
	//Now we have the MSB cohort with one less entry. Time to drill down on it and find the missing ip address
	std::bitset<buckets> candidates{};
	start = begin_cpy;
	while (start != end)
	{
		if (*start >> 16 == short_fall)
		{
			const auto lsb = ((1 << 16) - 1) & *start;
			candidates.set(lsb);
		}
		++start;
	}

	for (int i = 0; i < candidates.size(); ++i)
	{
		if (!candidates[i])
			return (short_fall << 16) | i;
	}
	return -1;
}

/*
* Option 1 is to put the elements in a hashmap and check for the flag returned after insertion. This identifies the dupe item
* We then iterate the map from 1 to N and inspect the return value from find to identify missing element.
* Option 2 trades a slower runtime for O(1) space by sorting the elements and iterating to find the missing and duplicated element. 
* Option 3 uses the xor property that xoring same number is 0. We xor all results from 1 to N and xor all elements in the array then xor both.
* All equal elements in both will be negated leaving a result of x ^ y where x is missing and y is duplicated.
* Next is tricky. We find the lsb that is a 1 in the cummulative xor. This means either missing or dupe has a set bit in same location.
* If the missing item has this bit set, the result of xor will yield the duplicate.
* If the duplicate item has this bit set, the result of xor, will result in missing
*/
std::pair<int, int> FindDuplicateAndMissingElement(const std::vector<int>& input)
{
	int xor_of_dup_and_missing = 0;
	//If the size is N then the natural numbers go from 0 to N-1. You need to be careful when question is asked as it could be from 1 to N
	for (int i = 0; i < input.size(); ++i)
	{
		xor_of_dup_and_missing ^= i ^ input[i];
	}
	const int bit_mask = xor_of_dup_and_missing & (~(xor_of_dup_and_missing - 1));
	int miss_or_dup = 0;
	for (int i = 0; i < input.size(); ++i)
	{
		if (i & bit_mask)
			miss_or_dup ^= i;
		if (input[i] & bit_mask)
			miss_or_dup ^= input[i];
	}

	if (std::find(input.begin(), input.end(), miss_or_dup) != input.end())
	{
		//this must be the dupe
		return { miss_or_dup, xor_of_dup_and_missing ^ miss_or_dup };
	}

	//It must be the missing
	return { xor_of_dup_and_missing ^ miss_or_dup, miss_or_dup };
}

std::vector<std::vector<std::string>> FindAnagrams(const std::vector<std::string>& words)
{
	std::unordered_map<std::string, std::vector<std::string>> anagrams{};
	for (const auto& word : words)
	{
		std::string sorted_word = word;
		std::sort(sorted_word.begin(), sorted_word.end());
		anagrams[sorted_word].push_back(word);
	}
	std::vector<std::vector<std::string>> res{};
	for (const auto& [k, v] : anagrams)
	{
		if (v.size() > 1) //why not !v.empty()
			res.push_back(v);
	}
	return res;
}

/*
* Rather interesting solution. A palindrome has one property. 
* If its even, then every char has an even number of occurences.
* If its odd, then every char has an even number of occurences with only one with an odd number.
* This simplifies to the string having 0 or 1 chars with an odd number of occurences
* Interestingly what about [a,b,a]. Thats fine. 
*/
bool CanFormPalindrome(const std::string& input)
{
	std::unordered_map<char, int> count{};
	for (const auto aChar : input)
	{
		++count[aChar];
	}
	int odd_occurencies = 0;

	for (const auto& [aChar, freq] : count)
	{
		if (freq % 2 != 0)
			++odd_occurencies;
		if (odd_occurencies > 1)
			return false;
	}
	return true;
}

/*
* So the first iteration of this problem tallies up all the frequencies and then counts all the odd occurencies. If our count is > 1, return false.
* This uses 2 iterations. We can do better with a single iteration. store only chars with count of 1.
* If insert indicates that the char already exists, this means it no longer has single occurence. delete it.
*/
bool CanFormPalindrome2(const std::string& input)
{
	std::unordered_set<char> odd_occurences{};
	for (auto aChar : input)
	{
		const auto& [iter, inserted] = odd_occurences.insert(aChar);
		if (!inserted)
		{
			odd_occurences.erase(iter);
		}
	}
	return odd_occurences.size() <= 1;
}

/*
* There are 2 ways of doing this. Both involve iterating through letter and magazine but one 
* potentially involves storing less info.
* 1. We go through magazine and store char frequencies. Next we go through letters and decrement. as far we dont find a zero freq we are good.
* 2. Go through letter and store char frequencies. Next go through magazine and decrement
*/
bool IsLetterConstructibleFromMagazine(const std::string& letter, const std::string& magazine)
{
	std::unordered_map<char, int> count{};
	for (auto aChar : letter)
		++count[aChar];
	for (auto aChar : magazine)
	{
		auto iter = count.find(aChar);
		if (iter != count.end())
		{
			//--iter->second;
			if (--iter->second == 0)
				count.erase(iter);
			if (count.empty())
				break;
		}
	}
	return count.empty();
}

int FindNearestRepetition(const std::vector<std::string>& paragraph)
{
	int res = std::numeric_limits<int>::max();
	std::unordered_map<std::string, int> words_and_idx;
	for (int i = 0; i < paragraph.size(); ++i)
	{
		auto iter = words_and_idx.insert({ paragraph[i], i });
		if (!iter.second)
		{
			const auto distance = (i - iter.first->second);
			res = std::min(res, distance);
			iter.first->second = i;
		}
	}
	return res == std::numeric_limits<int>::max() ? -1 : res;
}

/*
* This one looks very similar to longest unique substring.
* You have 2 pointers to create the window. You move the right until u have included all
* you move the left until you have excluded at least one. A rudimentary solution is to start
* with all the keywords in a set and erase them as we encounter. Once size is 0 then we got all in a window.
* then we rest the set of keywords and move the left until we've erased one of the keywords at which point we go 
* right again to coplete the set. There is an easier way with a hash_map. Lets explore.
*/
std::pair<int, int> FindSmallestSubArrayCoveringSet(const std::vector<std::string>& paragraph, 
	                                                const std::unordered_set<std::string>& keywords)
{
	std::pair<int, int> res{-1, -1};
	std::unordered_map<std::string, int> processed_state{};
	for (const auto& i : keywords)
		processed_state[i] = 1;
	int to_process = processed_state.size();

	for (int right = 0, left = 0; right < paragraph.size(); ++right)
	{
		auto iter = processed_state.find(paragraph[right]);
		if (iter != processed_state.end() && iter->second == 1)
		{
			--iter->second;
			--to_process;
		}
		while (to_process == 0)
		{
			if (right < 0 || right - left < res.second - res.first)
				res = {left, right};
			auto iter2 = processed_state.find(paragraph[left]);
			if (iter2 != processed_state.end())
			{
				++to_process;
				++iter2->second;
			}
			++left;
		}
	}
	return res;
}

/*
* This is way more complex than the variation above.
* the string t can contain duplicates and the window must contain them all.
* 1. Get the counts of each char in string t stored in to_processMap. these are the min counts that must be in the sliding window.
* 2. Once each char reaches its min, decrement to process as we've satisfied the min required count for this char
* 3. Once you reach a to_process of 0, then your seenCount map has the frequencies of all the chars.
* 4. Now we start to move the left pointer and here it gets tricky. Keep on moving it until one of the char count goes below the min
* 5. Once it goes below min we start to move the right pointer
*/
std::string minWindow(const std::string& s, const std::string& t) 
{
	std::unordered_map<char, int> to_processMap{};
	int minBegin = -1, minEnd = -1;
	for (char c : t)
	{
		++to_processMap[c];
	}
	int to_process = to_processMap.size();
	std::unordered_map<char, int> seenCount{};
	for (int left = 0, right = 0; right < s.size(); ++right)
	{
		auto iter = to_processMap.find(s[right]);
		if (iter != to_processMap.end())
		{
			++seenCount[s[right]];
			if (seenCount[s[right]] == to_processMap[s[right]])
			{
				--to_process;
			}
		}
		while (to_process == 0)
		{
			//Work out the window and update min if possible
			if (minBegin == -1 || right - left < minEnd - minBegin)
			{
				minBegin = left;
				minEnd = right;
			}
			auto iter2 = seenCount.find(s[left]);
			if (iter2 != seenCount.end())
			{
				--iter2->second;
				if (iter2->second < to_processMap[s[left]])
					++to_process;
			}
			++left;
		}
	}
	//compute substr
	if (minBegin == -1)
		return "";
	return s.substr(minBegin, minEnd - minBegin + 1);
}

/*
* We have an array of numbers or a string of characters. Each will have a set of unique numbers or characters
* The task is to find the smallest subarray that contains all the unique numbers of characters.
*/
std::pair<int, int> MinSubarrayContainingUniqueElementsInArray(std::vector<int>& input)
{
	std::pair<int, int> res{-1, -1};
	ReturnOnlyUniqueElements(input);
	std::unordered_map<int, int> distinct_map{};
	for (auto i : input)
	{
		++distinct_map[i];
	}
	int to_process = input.size();
	for (int right = 0, left = 0; right < input.size(); ++right)
	{
		auto iter = distinct_map.find(input[right]);
		if (iter != distinct_map.end() && --iter->second == 0)
		{
			--to_process;
		}

		while (!to_process)
		{
			if (res.first < 0 || right - left < res.second - res.first)
				res = {left, right};
			auto iter = distinct_map.find(input[left]);
			if (iter != distinct_map.end())
			{
				++iter->second;
				++to_process;
			}
			++left;
		}
	}
	return res;
}

/*
* So we have a series of integers and the task is to find the longest sub array that has consecutive ints that differ by 1.
* Solution 1 will involve sorting the input. Then we walk through array with 2 pointers and count.
* If current and next differ by 1 increment count.
*/
int MaxSubArrayConsecutiveInteger(std::vector<int>& input)
{
	std::sort(input.begin(), input.end());
	int max = 0;
	int count = 1;
	for (int i = 1; i < input.size(); ++i)
	{
		if (input[i] - input[i - 1] == 1)
		{
			++count;
		}
		else
		{
			max = std::max(max, count);
			count = 1;
		}
	}
	return max;
}

/*
* This one seems a bit nuts. Uses 
*/
int MaxSubArrayConsecutiveInteger1(const std::vector<int>& input)
{
	std::unordered_set<int> unprocessed_set{ input.begin(), input.end() };
	int max = 0;
	while (!unprocessed_set.empty())
	{
		const int number = *unprocessed_set.begin();
		unprocessed_set.erase(number);
		int count = 1;
		int tmp = number;
		while (unprocessed_set.count(--tmp))
		{
			++count;
			unprocessed_set.erase(tmp);
		}
		tmp = number;
		while (unprocessed_set.count(++tmp))
		{
			++count;
			unprocessed_set.erase(tmp);
		}
		max = std::max(max, count);
	}
	return max;
}

/*
* sentence = amanaplanacanal, words = [can, apl, ana]
* All the words have same size. The implication is that no 2 words can ever be a possible match for example
* sentence = bobcat, words = [bob, bobcat]. Here both words match substring. This makes the code more involved.
* In the general case where the words are of different length, there exists a recursive solution and this is for completeness
* 1. Go through the words perhaps starting from longest
*/

bool all_matched(const std::string& sentence, int start_idx, std::unordered_map<std::string, int> wordsfreq, int wordsize)
{
	for (int i = start_idx; !wordsfreq.empty() && start_idx + wordsize - 1 < sentence.size(); i += wordsize)
	{
		const auto fragment = sentence.substr(i, wordsize);
		auto iter = wordsfreq.find(fragment);
		if (iter == wordsfreq.end() || iter->second < 1)
			return false;
		--iter->second;
		if (iter->second == 0)
			wordsfreq.erase(iter);
	}
	return wordsfreq.empty();
}

std::vector<int> FindAllSubstrings(const std::string& sentence, const std::vector<std::string>& words)
{
	std::vector<int> res{};
	if (words.empty())
		res;
	std::unordered_map<std::string, int> wordsfreq{};
	for (const auto& word : words)
	{
		++wordsfreq[word];
	}
	
	const int nwords = words.size();
	const auto wordsize = wordsfreq.begin()->first.size();
	for (int i = 0; sentence.size() - i >= nwords*wordsize; ++i)
	{
		if (all_matched(sentence, i, wordsfreq, wordsize))
			res.push_back(i);
	}
	return res;
}

/*
* I still insist i can do this with a sliding window (probably works only if the array contains natural numbers +ve).
* The solution involves accumulating the items and adding the totals.
* If the total so far equals our target, update our maxLen. If not,
* add the total to map but also check if total - target aleady exists.
* if so, then i - map[total - target] is a range that can be checked if its max.
* What about int overflow?
*/
int LengthOfLongestSubArrayWithSum(const std::vector<int>& input, int target)
{
	int maxLen = 0, sum = 0;
	std::unordered_map<int, int> totals{};
	for (int i = 0; i < input.size(); ++i)
	{
		sum += input[i];
		totals.insert({sum, i});

		if (sum == target)
		{
			maxLen = i + 1;
		}

		const int diff = sum - target;
		const auto iter = totals.find(diff);
		if (iter != totals.end())
		{ 
			maxLen = std::max(i - iter->second, maxLen);
		}
	}
	return maxLen;
}

/*
* Looks like it tries to work out the number of subarrays with a given a sum that equals the target passed in.
*/
int SubArraySum(const std::vector<int>& nums, int target)
{
	int num = 0;
	int sum = 0;
	std::unordered_map<int, int> sum_map{};
	for (int i : nums)
	{
		sum += i;
		if (sum == target)
			++num;
		const int diff = sum - target;
		auto iter = sum_map.find(diff);
		if (iter != sum_map.end())
			num += iter->second;
		++sum_map[sum];
	}
	return num;
}
/*
* Simply counting the occurences until it nets is not correct on its own since the order in which the parenthesis appear
* i.e )( is not the same as (). This is why we check the char is ). Lets battle test this.
* )()((). It appears to work. Updating the i for already seen counts via pos[count] = i seems odd to me but this test case failed
* "()(()". code was expecting 2 but got 4.
*/
int longestValidParentheses2(std::string s)
{
	int max = 0, count = 0;
	std::unordered_map<int, int> pos;
	pos[0] = -1;
	for (int i = 0; i < s.size(); ++i)
	{
		count += (s[i] == '(' ? 1 : -1);
		const auto iter = pos.find(count);

		if (iter != pos.end() && s[i] == ')')
		{
			max = std::max(max, i - iter->second);
		}
		else
		{
			//This is where this differs in the max 0's and 1's. From empirical experiments, its no guarantee that the entire range is well formed.
			pos[count] = i;
		}
	}
	return max;
}

std::vector<int> IntersectTwoSortedArrayNoDupes(const std::vector<int>& one, const std::vector<int>& two)
{
	int idx_1 = 0, idx_2 = 0;
	std::vector<int> res{};
	while (idx_1 < one.size() && idx_2 < two.size())
	{
		if (one[idx_1] == two[idx_2])
		{
			if (res.empty() || res.back() != one[idx_1])
				res.push_back(one[idx_1]);
			++idx_1;
			++idx_2;
		}
		else if (one[idx_1] < two[idx_2])
		{
			++idx_1;
		}
		else
		{
			++idx_2;
		}
	}
	return res;
}

/*
* The merged array is to be stored in the first. The first array has enough room.
* Now for the algo. We need 3 pointers
* 1. Pointing to end of data in first.
* 2. Pointing to end of data in second.
* 3. Pointing to end of array in first.
* The solution in EPI seems to assume that the first iter will be exhausted before the second. odd.
*/
void MergeTwoSortedArraysIntoFirst(std::vector<int>& first, int m, const std::vector<int>& second, int n)
{
	int a = m - 1;
	int b = n - 1;
	int writeidx = m + n - 1;
	//How did u make this error? if first is bigger than all elements in second, first will go to 0 first.
	while (a >= 0 && b >= 0)
	{
		first[writeidx--] = first[a] > second[b] ?  first[a--] : second[b--];
	}
	//first is exhaused (and possibly last too)
	while (a >= 0)
	{
		first[writeidx--] = second[a--];
	}
	while (b >= 0)
	{
		first[writeidx--] = second[b--];
	}
}

/*
* I am a professor who has written a number of papers that get cited from time to time.
* What is the highest number of papers h that have been cited h times or more?
*/
int HIndex(std::vector<int>& citations)
{
	std::sort(citations.begin(), citations.end());
	const int len = citations.size();
	for (int i = 0; i < len; ++i)
	{
		if (citations[i] >= len - i)
			return len - i;
	}
	return 0;
}

/*
* 1. An interval could be non-overlapping, so we add these. The end will be less than the start of newInterval
* 2. The newInterval occurs before current and is non-overlapping
* 3. The interval is overlapping so extend.
*/
std::vector<std::vector<int>> insertInterval(std::vector<std::vector<int>>& intervals, std::vector<int>& newInterval)
{
	std::vector<std::vector<int>> res{};
	for (const auto& interval : intervals)
	{
		if (interval[1] < newInterval[0])
		{
			res.push_back(interval);
		}
		else if (newInterval[1] < interval[0])
		{
			res.push_back(newInterval);
			newInterval = interval;
		}
		else
		{
			newInterval[0] = std::min(interval[0], newInterval[0]);
			newInterval[1] = std::max(interval[1], newInterval[1]);
		}
	}
	res.push_back(newInterval);
	return res;
}

/*
* The more intuitive solution but longer solution is as follows.
* 1. add all the non-overlapping intervals that occur before the new interval first.
* 2. extend the interval for as long as is possible. If the new interval doesnt overlap, we wont enter the while loop here so kill 2 birds.
*/
std::vector<std::vector<int>> insertInterval1(std::vector<std::vector<int>>& intervals, std::vector<int>& newInterval)
{
	std::vector<std::vector<int>> res{};

	int i = 0;
	const int len = intervals.size();

	while (i < len && intervals[i][1] < newInterval[0])
	{
		res.push_back(intervals[i++]);
	}

	while (i < len && intervals[i][0] <= newInterval[1])
	{
		newInterval[0] = std::min(intervals[i][0], newInterval[0]);
		newInterval[1] = std::max(intervals[i][1], newInterval[1]);
		++i;
	}
	res.push_back(newInterval);
	std::copy(intervals.begin() + i, intervals.end(), std::back_inserter(res));
	return res;
}

std::vector<std::vector<int>> mergeIntervals(std::vector<std::vector<int>>& intervals)
{
	std::vector<std::vector<int>> res{};

	const auto cmp = [](const std::vector<int>& a, const std::vector<int>& b) {
		return a[0] < b[0] || (a[0] == b[0] && a[1] < b[1]);
	};
	std::sort(intervals.begin(), intervals.end(), cmp);

	for (int i = 0; i < intervals.size(); ++i)
	{
		if (res.empty() || res.back()[1] < intervals[i][0])
		{
			res.push_back(intervals[i]);
		}
		else
		{
			res.back()[0] = std::min(res.back()[0], intervals[i][0]);
			res.back()[1] = std::max(res.back()[1], intervals[i][1]);
		}
	}
	return res;
}
/*
* Giving a set of coins with a denomination, what is the smallest amount that we cant make from the coins?
* We sort the coins first [1, 3, 5]. If the max so far + 1 is less than next coin, then we've found the smallest amount we can make from coins.
*/
int SmallestNonConstructibleValue(std::vector<int>& in)
{
	std::sort(in.begin(), in.end());
	int max_constructible_so_far = 0;
	for (const auto coin : in)
	{
		if (max_constructible_so_far + 1 < coin)
		{
			break;
		}
		max_constructible_so_far += coin;
	}
	return max_constructible_so_far + 1;
}

void ReturnOnlyUniqueElements(std::vector<int>& input)
{
	int tail = 0;
	std::sort(input.begin(), input.end());

	for (int i = 0; i < input.size(); ++i)
	{
		if ((i == 0 || input[i] != input[i - 1]) && (i + 1 == input.size() || input[i] != input[i + 1]))
			std::swap(input[i], input[tail++]);
	}
	input.erase(input.begin() + tail, input.end());
}

int FindMaxSimultaneousEvents(const std::vector<std::pair<int, int>>& events)
{
	std::vector<std::pair<int, bool>> end_points{};
	end_points.reserve(events.size() * 2);
	for (const auto& event : events)
	{
		end_points.push_back({ event.first, true });
		end_points.push_back({ event.second, false });
	}

	const auto cmp = [](const auto& one, const auto& two)
	{
		return one.first < two.first || (one.first == two.first && (one.second && !two.second));
	};

	std::sort(end_points.begin(), end_points.end(), cmp);

	int max_simultaneous_events = 0;
	int num_simultaneous_events = 0;
	for (const auto& end_point : end_points)
	{
		if (end_point.first)
		{
			++num_simultaneous_events;
			max_simultaneous_events = std::max(num_simultaneous_events, max_simultaneous_events);
		}
		else
		{
			--num_simultaneous_events;
		}
	}
	return max_simultaneous_events;
}

std::string MergeStrings(const std::vector<std::string>& words)
{
	int charCount[26] = { 0 };
	for (const auto& word : words)
	{
		for (char eachChar : word)
		{
			++charCount[eachChar - 'a'];
		}
	}

	std::string res;
	for (int i = 0; i < 26; ++i)
	{
		if (charCount[i] > 0)
		{
			res.append(charCount[i], i + 'a');
		}
	}
	return res;
}

/*
* You got the concept for this one. kudos.
* salaries_so_far + (cap * employees_remaining) = target.
*/
double FindMaxCap(double target, std::vector<double>& salaries)
{
	std::sort(salaries.begin(), salaries.end());
	double salaries_so_far = 0.0;
	const int len = salaries.size();
	for (int i = 0; i < len; ++i)
	{
		const int employees_remaining = len - i;

		if ((salaries_so_far + (salaries[i] * employees_remaining)) >= target)
		{
			return (target - salaries_so_far) / employees_remaining;

		}
		salaries_so_far += salaries[i];
	}
	return -1.0;
}

void InsertSort(std::vector<int>& in)
{
	for (int i = 1; i < in.size(); ++i)
	{
		int j = i;
		while (j > 0 && in[j] < in[j - 1])
		{
			std::swap(in[j], in[j - 1]);
			--j;
		}
	}
}

void SelectionSort(std::vector<int>& in)
{
	for (int i = 0; i < in.size(); ++i)
	{
		int min = i;
		for (int j = i + 1; j < in.size(); ++j)
		{
			if (in[j] < in[min])
				min = j;
		}
		std::swap(in[i], in[min]);
	}
}
/*
* 1. Find the schedule with the earliest ending time.
* 2. Put it in the result
* 3. Remove other schedules that overlap
* 4. Go back to 1 if its not empty.
*/
auto OptimalScheduling(std::vector<std::tuple<std::string, int, int>> schedules) -> decltype(schedules)
{
	decltype(schedules) res{};
	while (!schedules.empty())
	{
		int min_idx = 0;
		for (int i = 1; i < schedules.size(); ++i)
		{
			if (std::get<2>(schedules[i]) < std::get<2>(schedules[min_idx]))
				min_idx = i;
		}
		res.push_back(schedules[min_idx]);
		const auto endTime = std::get<2>(res.back());
		schedules.erase(schedules.begin() + min_idx);
		//Lets use remove_if to find all the overlaps.
		auto iter = std::remove_if(schedules.begin(), schedules.end(), [endTime](const auto& i) {return endTime > std::get<1>(i); });
		schedules.erase(iter, schedules.end());
	}
	return res;
}

/*
* An alternative to above wich involves sorting the schedules by endtime first.
*/
struct Schedule
{
	std::string name;
	int start;
	int end;
};

//I believe sorting can help the overall algorithm.
std::vector<Schedule> OptimalScheduling2(std::vector<Schedule>& schedules)
{
	std::vector<Schedule> optimumSchedules{};

	const auto cmp = [](const Schedule& one, const Schedule& two)
	{
		return one.end < two.end;
	};
	std::sort(schedules.begin(), schedules.end(), cmp);
	auto iter = schedules.begin();
	while (iter != schedules.end())
	{
		optimumSchedules.push_back(*iter++);
		while (iter != schedules.end() && !(optimumSchedules.back().end < iter->start))
		{
			++iter;
		}
	}
	return optimumSchedules;
}

std::vector<std::vector<int>> getSkyline(const std::vector<std::vector<int>>& buildings) 
{
	std::vector<std::vector<int>> res{};
	if (buildings.empty())
		return res;
	std::vector<std::pair<int, int>> building_points{};
	building_points.reserve(buildings.size() * 2);
	for (const auto& building : buildings)
	{
		building_points.push_back({ building[0], -building[2] });
		building_points.push_back({ building[1], building[2] });
	}
	std::sort(building_points.begin(), building_points.end());
	std::multiset<int> next_biggest{ 0 };
	int current_highest = 0;
	for (const auto& point : building_points)
	{
		if (point.second < 0)
		{
			next_biggest.insert(-point.second);
		}
		else
		{
			next_biggest.erase(next_biggest.find(point.second));
		}
		const auto biggest = *next_biggest.rbegin();
		if (biggest != current_highest)
		{
			res.push_back({ point.first, biggest });
			current_highest = biggest;
		}
	}
	return res;
}

/*
* The BST property
* All keys in the left subtree of a node are less than or equal to the value of the node
* All keys in the right subtree of a node are greater than or equal to the value of the node
*/
std::shared_ptr<BinarySearchTree> Search(std::shared_ptr<BinarySearchTree> root, int k)
{
	if (!root)
		return root;

	if (root->data == k)
		return root;
	return  k < root->data ? Search(root->left, k) : Search(root->right, k);
}

bool IsNodeInRange(std::shared_ptr<BinarySearchTree> node, int min, int max)
{
	if (!node)
		return true;
	if (node->data < min || node->data > max)
		return false;
	return IsNodeInRange(node->left, min, node->data) && IsNodeInRange(node->right, node->data, max);
}

/*
* Given some node, we can test its value against those of its children but this is wrong.
* There is an important property that u overlooked. ALL elements to the left of node must be smaller 
* and ALL the elements to the right must be larger. This means it is not enough to simply test that the 
* left child is smaller and the right child is greater. The implementation in EPI fails if the value of the root node is max() or min()
*/
bool IsBinaryTreeBST(std::shared_ptr<BinarySearchTree> root)
{
	return IsNodeInRange(root, std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
}

/*
* The naive impl that comes to mind is to do an Inorder Traversal and return the first node that is greater.
* This is linear and doesnt take advantage of the fact that the tree is sorted.
* If the current node is greater than the target, go left else go right. The solution doesnt terminate until we 
* arrive at a leaf. Can this be improved?
*/
std::shared_ptr<BinarySearchTree> FindFirstGreaterThanK(std::shared_ptr<BinarySearchTree> root, int k)
{
	std::shared_ptr<BinarySearchTree> greatest_seen_so_far = nullptr;
	auto iter = root;
	while (iter)
	{
		if (iter->data > k)
		{
			greatest_seen_so_far = iter;
			iter = iter->left;
		}
		else
			iter = iter->right;
	}
	return greatest_seen_so_far;
}

std::shared_ptr<BinarySearchTree> FindFirstEqualToK(std::shared_ptr<BinarySearchTree> root, int k)
{
	std::shared_ptr<BinarySearchTree> equal_seen_so_far = nullptr;
	auto iter = root;
	while (iter)
	{
		if (iter->data >= k)
		{
			if (iter->data == k)
				equal_seen_so_far = iter;
			iter = iter->left;
		}
		else
			iter = iter->right;
	}
	return equal_seen_so_far;
}
/*
* The impl from book what i think is a superflous check on the vector size b4 we call right.
*/
void FindKLargestInBSTHelper(std::shared_ptr<BinarySearchTree> node, std::vector<int>& kLargest, int k)
{
	if (!node)
		return;
	//go right
	FindKLargestInBSTHelper(node->right, kLargest, k);
	if (kLargest.size() < k)
	{
		kLargest.push_back(node->data);
		if (kLargest.size() < k)
			FindKLargestInBSTHelper(node->left, kLargest, k);
	}
}

/*
* The solution that comes to mind is to do an inorder traversal while maintaining a queue. The Q will have at most K elements
* and we would be popping older elements to make way for newer ones.
* A potentially more efficient solution is to walk the tree in reverse which is achieved by going right and then going left
* and adding to a vector until the size is k.
*/
std::vector<int> FindKLargestInBST(std::shared_ptr<BinarySearchTree> root, int k)
{
	std::vector<int> kLargest{};
	kLargest.reserve(k);
	FindKLargestInBSTHelper(root, kLargest, k);
	return kLargest;
}

std::pair<int, bool> kthSmallestHelper(std::shared_ptr<BinarySearchTree> root, int& k)
{
	if (!root)
		return { -1, false };

	const auto& res = kthSmallestHelper(root->left, k);
	if (res.second)
		return res;
	--k;
	if (k == 0)
	{
		return { root->data, true };
	}
	return kthSmallestHelper(root->right, k);
}
int kthSmallest(std::shared_ptr<BinarySearchTree> root, int k)
{
	return kthSmallestHelper(root, k).first;
}

int kthSmallest2(std::shared_ptr<BinarySearchTree> root, int k)
{
	std::stack<std::pair<std::shared_ptr<BinarySearchTree>, bool>> to_process{};
	to_process.push({ root, false });
	while (!to_process.empty())
	{
		const auto [node, processed] = to_process.top();
		to_process.pop();
		if (node)
		{
			if (processed)
			{
				--k;
				if (k == 0)
					return node->data;
			}
			else
			{
				to_process.push({ node->right, false });
				to_process.push({ node, true });
				to_process.push({ node->left, false });
			}
		}
	}
	return -1;
}

/*
* EPI uses an iterative solution. I have written a recursive solution in the notebook but one should always use iterative over recursion if possible.
* There are 4 possibilities that make this easy to reason about.
* 1. If either of the nodes equal the root, then the root is the LCA.
* 2. if one node is less than root and the other is greater than root, the root is the LCA.
* 3. If both nodes are less than root, then LCA is in left subtree.
* 4. If both nodes are greater than root, then LCA is in right subtree.
*/
std::shared_ptr<const BinarySearchTree> LowestCommonAncestor(const std::shared_ptr<const BinarySearchTree> root,
	const std::shared_ptr<const BinarySearchTree> one,
	const std::shared_ptr<const BinarySearchTree> two)
{
	const auto small = one->data < two->data ? one : two;
	const auto big = small == one ? two : one;
	auto r = root;

	while (r->data > big->data || r->data < small->data)
	{
		while (r->data > big->data)
			r = r->left;
		while (r->data < small->data)
			r = r->right;
	}
	return r;
}

/*
* Not too dissimilar to merge k arrays but this one needs to compute the diff between the largest and smallest.
* A heap wont do as u can only get the min and max. We would need to get both min and max efficiently.
* The solution is to start by adding an element each from each array. Compute the diff then you pop the smallest and replace
* with next element from same array. Whatever u erase needs to have a current iter and and end. 
* Also needs a custom compaater to dereference the current iters.
*/
int FindClosestElementInSortedArrays(const std::vector<std::vector<int>>& arrays)
{
	using Iter = std::vector<int>::const_iterator;
	using IterPair = std::pair<Iter, Iter>;
	const auto cmp = [](const IterPair& one, const IterPair& two) {
		return *(one.first) < *(two.first);
	};
	std::multiset<IterPair, decltype(cmp)> tuple{cmp};
	for (const auto& arr : arrays)
	{
		tuple.insert({ arr.begin(), arr.end() });
	}

	int min = 0;
	auto smallest = tuple.begin();
	while (smallest->first != smallest->second)
	{
		min = std::min(min, *(tuple.rbegin()->first) - *(smallest->first));
		IterPair next{std::next(smallest->first), smallest->second };
		tuple.erase(smallest);
		tuple.insert(next);
		smallest = tuple.begin();
	}
	return min;
}

std::shared_ptr<BinarySearchTree> BuildMinHeightFromSortedArrayHelper(const std::vector<int>& sorted, int left, int right)
{
	if (left > right)
		return nullptr;
	const int mid = left + (right - left) / 2;
	return std::make_shared<BinarySearchTree>(sorted[mid],
		BuildMinHeightFromSortedArrayHelper(sorted, left, mid - 1),
		BuildMinHeightFromSortedArrayHelper(sorted, mid + 1, right));
}

std::shared_ptr<BinarySearchTree> BuildMinHeightFromSortedArray(const std::vector<int>& sorted)
{
	return BuildMinHeightFromSortedArrayHelper(sorted, 0, sorted.size() - 1);
}

/*
* On the face of it, this is not that complex. U start with first param and search the BST way for middle
* If u find a match, search for the second param from middle.
* If u dont find a match, start with second param and search for the middle the BST way.
* If u find a match, search the first param from middle.
* An optimisation would be search both 1st and 2nd param for middle in same iter. what are the possibilities?
* 1. I find middle from both which means both are ancestors of middle and return false.
* 2. I find none which means middle is not in any family tree and return false.
* 3. I find middle is in atleast one the family tree...perform a search on other starting from middle
*/
bool PairIsAncestorAndDescendant(const std::shared_ptr<const BinarySearchTree> possible_anc_or_desc_0,
	const std::shared_ptr<const BinarySearchTree> possible_anc_or_desc_1,
	const std::shared_ptr<const BinarySearchTree> middle)
{
	auto iter0 = possible_anc_or_desc_0;
	auto iter1 = possible_anc_or_desc_1;
	std::shared_ptr<const BinarySearchTree> anc;

	while (iter0 || iter1)
	{
		if (iter0)
		{
			if (iter0 == middle)
			{
				anc = iter0;
				break;
			}
			iter0 = iter0->data < middle->data ? iter0->right : iter0->left;
		}

		if (iter1)
		{
			if (iter1 == middle)
			{
				anc = iter1;
				break;
			}
			iter1 = iter1->data < middle->data ? iter1->right : iter1->left;
		}
	}
	if (!anc)
		return false;

	auto dec = iter0 == middle ? possible_anc_or_desc_1 : possible_anc_or_desc_0;
	iter0 = middle;
	while (iter0 && iter0 != dec)
	{
		iter0 = iter0->data < dec->data ? iter0->right : iter0;
	}
	return iter0.get();
}

void RangeLookupBSTHelper(const std::shared_ptr<const BinarySearchTree> node, const std::pair<int, int>& range, std::vector<int>& matches)
{
	if (!node)
		return;
	if (node->data >= range.first && node->data <= range.second)
	{
		RangeLookupBSTHelper(node->left, range, matches);
		matches.push_back(node->data);
		RangeLookupBSTHelper(node->right, range, matches);
	}
	else if (node->data < range.first)
	{
		RangeLookupBSTHelper(node->right, range, matches);
	}
	else
	{
		RangeLookupBSTHelper(node->left, range, matches);
	}
}
/*
* 1. If the current node is between the range, add to vector and process both left and right subtrees.
* 1. If the current node is less than low end of the range, then go right as it cant be in left.
* 2. If the current node is greater than high end of the range, then go left as it cant be in right.
*/
std::vector<int> RangeLookupBST(const std::shared_ptr<const BinarySearchTree> root, const std::pair<int, int>& range)
{
	std::vector<int> matches{};
	RangeLookupBSTHelper(root, range, matches);
	return matches;
}

//Iterative solution
/*
* x is the larger of the 2 quantities
* ensure it stays that way in your loop
*/
int gcd(int x, int y)
{
	auto p = std::minmax(y, x);
	while (y)
	{
		auto tmp = y;
		y = x % y;
		x = tmp;
	}
	return x;
}

int gcd1(int x, int y)
{
	return y == 0 ? x : gcd1(y, x % y);
}

/*
* Tail recurssion is an interesting type of recursion that doesnt require the stack of the previous function invocation
* The results from the last invocation is passed to current invocation in a such a way that local variables from prev
* call do not need to be held on the stack. The example below is for non-tail recursion.
* recsum(5)
5 + recsum(4)
5 + (4 + recsum(3))
5 + (4 + (3 + recsum(2)))
5 + (4 + (3 + (2 + recsum(1))))
5 + (4 + (3 + (2 + (1 + recsum(0)))))
5 + (4 + (3 + (2 + (1 + 0))))
5 + (4 + (3 + (2 + 1)))
5 + (4 + (3 + 3))
5 + (4 + 6)
5 + 10
15

Tail recursion on the other hand is as follows

tailrecsum(5, 0)
tailrecsum(4, 5)
tailrecsum(3, 9)
tailrecsum(2, 12)
tailrecsum(1, 14)
tailrecsum(0, 15)
15
*/

int SumN(int N)
{
	if (N == 0)
		return 0;
	return N + SumN(N - 1);
}

int SumN1(int N, int total)
{
	if (N == 0)
		return total;
	return SumN1(N - 1, total + N);
}

/*
* The easiest way to understand the solution is to note that there are 3 actions that need to happen
* 1. Move all but one ring from to_peg to aux_peg.
* 2. Move last ring from to_peg to from_peg.
* 3. Move all rings from aux_peg to from_peg.
*/
void TowerOfHanoiHelper(int n, int from_peg, int to_peg, int aux_peg, std::vector<std::pair<int, int>>& results)
{
	if (n == 1)
	{
		results.push_back({from_peg, to_peg});
		return;
	}
	TowerOfHanoiHelper(n - 1, from_peg, aux_peg, to_peg, results);
	results.push_back({ from_peg, to_peg });
	TowerOfHanoiHelper(n - 1, aux_peg, to_peg, from_peg, results);

}

std::vector<std::pair<int, int>> TowerOfHanoi(int n)
{
	std::vector<std::pair<int, int>> res{};
	TowerOfHanoiHelper(n, 0, 1, 2, res);
	return res;
}

void PhoneMnemonicHelper(int digit, 
	const std::string& number, 
	std::vector<std::string>& mnemonics, 
	std::string& match, 
	const std::array<std::string, 10>& mappings)
{
	if (digit == number.size())
	{
		mnemonics.push_back(match);
		return;
	}

	const auto alphaset = mappings[number[digit] - '0'];
	for (char c : alphaset)
	{
		match[digit] = c;
		PhoneMnemonicHelper(digit + 1, number, mnemonics, match, mappings);
	}
}

std::vector<std::string> PhoneMnemonic(const std::string& number)
{
	std::vector<std::string> mnemonics{};
	const std::array<std::string, 10> mappings{ {"0", "1", "ABC", "DEF", "GHI", "JKL", "MNO", "PQRS", "TUV", "WXYZ"}};
	std::string aMatch(number.size(), '0');
	PhoneMnemonicHelper(0, number, mnemonics, aMatch, mappings);
	return mnemonics;
}

std::vector<std::string> PhoneMnemonic1(const std::string& number)
{
	std::vector<std::string> mnemonics{};
	const std::array<std::string, 10> mappings{ {"0", "1", "ABC", "DEF", "GHI", "JKL", "MNO", "PQRS", "TUV", "WXYZ"} };
	std::string aMatch(number.size(), '0');
	std::stack<std::pair<int, int>> call_stack{};
	call_stack.push({ 0, 0 });
	while (!call_stack.empty())
	{
		auto& item = call_stack.top();

		if (item.first + 1 == number.size())
		{
			for (char c : mappings[number[item.first] - '0'])
			{
				aMatch[item.first] = c;
				mnemonics.push_back(aMatch);
			}
			call_stack.pop();
		}
		else
		{
			const auto mapping = mappings[number[item.first] - '0'];
			if (item.second < mapping.size())
			{
				aMatch[item.first] = mapping[item.second++];
				call_stack.push({ item.first + 1, 0 });
			}
			else
			{
				call_stack.pop();
			}
		}
	}
	return mnemonics;
}

bool isValid(const std::vector<int>& partial)
{
	const int row_id = partial.size() - 1;
	for (int i = 0; i < row_id; ++i)
	{
		const int col_diff = std::abs(partial[i] - partial[row_id]);
		//If diff is 0 then its on same column as another to fails.
		//If diff is same as row_id then its on same diagonal.
		if (col_diff == 0 || col_diff == row_id - i)
			return false;
	}
	return true;
}
void solveNQueensHelper(int n, std::vector<int>& partial, std::vector<std::vector<std::string>>& res)
{
	if (partial.size() == n)
	{
		std::vector<std::string> placements{};
		for (int i = 0; i < partial.size(); ++i)
		{
			std::string placement(partial.size(), '.');
			placement[partial[i]] = 'Q';
			placements.push_back(placement);
		}
		res.push_back(placements);
		return;
	}
	for (int col = 0; col < n; ++col)
	{
		partial.push_back(col);
		if (isValid(partial))
			solveNQueensHelper(n, partial, res);
		partial.pop_back();
	}
}

/*
* Where to start from? 
*/
std::vector<std::vector<std::string>> solveNQueens(int n) 
{
	std::vector<std::vector<std::string>> res{};
	std::vector<int> partial{};
	solveNQueensHelper(n, partial, res);
	return res;
}


void PermutationHelper(int i, std::vector<int>& in, std::vector<std::vector<int>>& res)
{
	if (i == in.size() - 1)
	{
		res.push_back(in);
		return;
	}

	for (int j = i; j < in.size(); ++j)
	{
		std::swap(in[i], in[j]);
		PermutationHelper(i + 1, in, res);
		std::swap(in[i], in[j]);
	}
}

/*
* For an array A with size n. The following applies
* 1. All the permutations must start with A[0], A[1],..., A[n-1]
* 2. The Permutation starting from A[0] == A[0] + permutation(A[1], A[N]).
* 3. The Permutation starting from A[1] is similar to above but after A[1] is swapped with A[0] 
*/
std::vector<std::vector<int>> Permutations(std::vector<int>& in)
{
	std::vector<std::vector<int>> res{};
	PermutationHelper(0, in, res);
	return res;
}

std::vector<std::vector<int>> Permutations1(std::vector<int>& in)
{
	std::sort(in.begin(), in.end());
	std::vector<std::vector<int>> res{};
	do
	{
		res.push_back(in);
	} while (std::next_permutation(in.begin(), in.end()));
	return res;
}

/*
* {apple, banana, mango}. The members of the power set are
* {} => [0,0,0]
* {apple} => [1,0,0]
* {banana} => [0,1,0]
* {mango} => [0,0,1]
* {apple, bababa} => [1,1,0]
* {apple, mango} => [1,0,1]
* {banana, mango} => [0,1,1]
* {apple, banana, mango} => [1,1,1]
* 1. Work out how many subsets there...this is 2^n and achieved b 1 << n in code.
* 2. For each number iterate through the set bits and map back to original input.
*/
std::vector<std::vector<std::string>> PowerSet(const std::vector<std::string>& in)
{
	std::vector<std::vector<std::string>> res{};
	const int subsets = 1 << in.size();
	res.reserve(subsets);
	for (int i = 0; i < subsets; ++i)
	{
		int bitset = i;
		int count = 0;
		std::vector<std::string> subset{};
		while (bitset)
		{
			if (bitset & 1)
			{
				subset.push_back(in[count]);
			}
			++count;
			bitset >>= 1;
		}
		res.push_back(std::move(subset));
	}
	return res;
}

void PowerSetHelper(int offset, const std::vector<int>& in, std::vector<std::vector<int>> res, std::vector<int>& partial)
{
	res.push_back(partial);
	for (int i = offset; i < in.size(); ++i)
	{
		partial.push_back(in[i]);
		PowerSetHelper(i + 1, in, res, partial);
		partial.pop_back();
	}
}

std::vector<std::vector<int>> PowerSet(const std::vector<int>& in)
{
	std::vector<int> partial{};
	std::vector<std::vector<int>> res{};
	PowerSetHelper(0, in, res, partial);
	return res;
}

void KCombinationsHelper(int n, int k, int offset, std::vector<std::vector<int>>& res, std::vector<int>& partial)
{
	if (partial.size() == k)
	{
		res.push_back(partial);
		return;
	}
	const int remaining = k - partial.size();
	for (int i = offset; i <= n; ++i)
	{
		partial.push_back(i);
		KCombinationsHelper(n, k, i + 1, res, partial);
		partial.pop_back();
	}
}

/*
* Very similar to the Permutation solution. The PowerSet solution can also be used albeit inefficiently.
* If k is much smaller than n or k is almost equal to n, lots of subsets will be generated that dont meet the size k.
*/
std::vector<std::vector<int>> KCombinations(int n, int k)
{
	std::vector<std::vector<int>> res{};
	if (k > n)
		return res;
	std::vector<int> partial{};
	partial.reserve(k);
	return res;
}

void GenerateBalancedParenthesisHelper(int left_remaining, int right_remaining, const std::string& partial, std::vector<std::string>& res)
{
	if (!right_remaining)
	{
		res.push_back(partial);
		return;
	}
	if (left_remaining > 0)
	{
		GenerateBalancedParenthesisHelper(left_remaining - 1, right_remaining, partial + "(", res);
	}
	//We could also do right_remaining > left_remaining
	if (left_remaining < right_remaining)
	{
		GenerateBalancedParenthesisHelper(left_remaining, right_remaining - 1, partial + ")", res);
	}
}

/*
* Aside from the usual arcane situation with some recursive functions, there are however 2 important factors that make sense.
* These factors are considered in the context of if i have a string which is less than 2k that has the probability of being
* balanced, what can I add, either ( or ), to keep it in this same likelihood.
* 1. If the no of left parens left is greater than 0, we can add left parens.
* 2. A right parens can only be added if we have already added some left parens i.e right > left.
*/
std::vector<std::string> GenerateBalancedParenthesis(int k)
{
	std::vector<std::string> res{};
	GenerateBalancedParenthesisHelper(k, k, "", res);
	return res;
}
//Brute force solution to generate all permutations and check if balanced.
void GenerateBalancedParenthesisHelper2(int i, std::string& partial, std::vector<std::string>& res)
{
	if (i == partial.size())
	{
		if (is_balanced(partial))
			res.push_back(partial);
		return;
	}
	//can u see how u can get away with push and pop routine in the backtracking algo?
	for (char c : {'(', ')'})
	{
		partial[i] = c;
		GenerateBalancedParenthesisHelper2(i + 1, partial, res);
	}
}
std::vector<std::string> GenerateBalancedParenthesis2(int i)
{
	std::vector<std::string> res{};
	std::string partial(i * 2, 0);
	GenerateBalancedParenthesisHelper2(0, partial, res);
	return res;
}

void PalindromeDecompositionsHelper(int offset,
	const std::string& s,
	std::vector<std::string>& partition,
	std::vector<std::vector<std::string>>& res)
{
	if (offset == s.size())
	{
		res.push_back(partition);
		return;
	}

	for (int i = offset + 1; i <= s.size(); ++i)
	{
		auto part = s.substr(offset, i - offset);
		if (IsPalindrome(part))
		{
			partition.push_back(std::move(part));
			PalindromeDecompositionsHelper(i, s, partition, res);
			partition.pop_back();
		}
	}
}

std::vector<std::vector<std::string>> PalindromeDecompositions(const std::string& s)
{
	std::vector<std::vector<std::string>> res{};
	std::vector<std::string> partition{};
	PalindromeDecompositionsHelper(0, s, partition, res);
	return res;
}

void IpHelper(int i,
	const std::string& input,
	std::vector<std::string>& parts,
	std::vector<std::string>& res)
{
	if (parts.size() == 4)
	{
		if (is_valid_ip(parts[0]) &&
			is_valid_ip(parts[1]) &&
			is_valid_ip(parts[2]) &&
			is_valid_ip(parts[3]))
		{
			res.push_back(parts[0] + "." + parts[1] + "." + parts[2] + "." + parts[3]);
		}
		return;
	}
	for (int j = i; j < input.size() && j < i + 3; ++j)
	{
		const int len = parts.size() < 3 ? j - i + 1 : std::string::npos;
		parts.push_back(input.substr(i, len));
		IpHelper(j + 1, input, parts, res);
		parts.pop_back();
		if (parts.size() == 3)
			break;
	}
}

std::vector<std::string> GetValidIpAddress2(const std::string& input)
{
	std::vector<std::string> res{};
	std::vector<std::string> parts{};
	IpHelper(0, input, parts, res);
	return res;
}

/* 0, 1, 1, 2, 3, 5, 8, 13, .....
* Zero based so passing 3 will return 2
*/ 
int fib1(int n)
{
	if (n < 2)
		return n;
	int fib_prev = 0, fib_curr = 1;
	for (int i = 2; i <= n; ++i)
	{
		int tmp = fib_curr + fib_prev;
		fib_prev = fib_curr;
		fib_curr = tmp;
	}
	return fib_curr;
}

/*
* The number of subarrays is n(n + 1)/2. The brute force is to enumerate all subarrays and find the one with greatest sum.
* The solution will take approx n^2 to enumerate the subarrays and another n for accummulate the items which is about n^3.
* There is an elegant dynamic programming solution. The max so far is the max of either
* 1. The sum of previous max so far and ith element
* 2. The ith element
*/
int FindMaximumSubarray(const std::vector<int>& nums)
{
	int currentSubarray = nums[0];
	int maxSubarray = nums[0];

	for (int i = 1; i < nums.size(); ++i) {
		currentSubarray = std::max(nums[i], currentSubarray + nums[i]);
		maxSubarray = std::max(maxSubarray, currentSubarray);
	}

	return maxSubarray;
}

/*
* Of course there will be a time it will be difficult to recall the above.
* The brute force solution is O(N^2) and is similar to the code that computes subarrays. The key is that all subarrays from
* i is the sum[i to j] where we are at j+1
*/

int FindMaximumSubarray2(const std::vector<int>& input)
{
	int max = std::numeric_limits<int>::min();
	for (int i = 0; i < input.size(); ++i)
	{
		int sum_so_far = 0;
		for (int j = i; j < input.size(); ++j)
		{
			sum_so_far += input[j];
			max = std::max(max, sum_so_far);
		}
	}
	return max;
}

/*
* Its a miracle that you understand this. the key is that if u can make a final score in n ways with just 1 score 
* then you can make the final score with 2 scores by adding following
* 1. Number of combinations to make final score +
* 2. Number of combinations to make final score - new score.
* For example, assuming we have a set of scores [2, 3]
*/
int NumCombinationForFinalScore(int final_score, const std::vector<int>& individual_scores)
{
	std::vector<std::vector<int>> combos(individual_scores.size(), std::vector<int>(final_score + 1, 0));
	for (int i = 0; i < individual_scores.size(); ++i)
	{
		combos[i][0] = 1;
		for (int j = 1; j <= final_score; ++j)
		{
			const int without_current = i > 0 ? combos[i - 1][j] : 0;
			const int with_current = j - individual_scores[i] >= 0 ? combos[i][j - individual_scores[i]] : 0;
			combos[i][j] = without_current + with_current;
		}
	}
	return combos.back().back();
}

/*
* Appears this can be done with m space rather than nm space.
*/
int NumCombinationForFinalScore2(int final_score, const std::vector<int>& individual_scores)
{
	std::vector<int> combos(final_score + 1, 0);
	combos[0] = 1;
	for (int i = 0; i <= final_score; ++i)
	{
		for (int score : individual_scores)
		{
			if (i - score >= 0)
				combos[i] += combos[i - score];
		}
	}
	return combos.back();
}

int coinChangeHelper(const std::vector<int>& coins, int remamount)
{
	if (remamount == 0)
		return 0;
	if (remamount < 0)
		return -1;
	int minCoins = std::numeric_limits<int>::max();
	for (int coin : coins)
	{
		const int min = coinChangeHelper(coins, remamount - coin);
		if (min == -1)
			continue;
		minCoins = std::min(minCoins, min);
	}
	return minCoins == std::numeric_limits<int>::max() ? -1 : 1 + minCoins;
}
/*
* Brute force solution with an issue of solving the same subproblem over and over again
* Time complexity is O(2^N)
* Space complexity is O(N)
*/
int coinChange(std::vector<int>& coins, int amount) 
{
	return coinChangeHelper(coins, amount);
}

int coinChangeHelper2(const std::vector<int>& coins, int remamount, std::vector<int>& memo)
{
	if (remamount == 0)
		return 0;
	if (remamount < 0)
		return -1;

	if (memo[remamount] != -1)
		return memo[remamount];

	int minCoins = std::numeric_limits<int>::max();
	for (int coin : coins)
	{
		int min = coinChangeHelper2(coins, remamount - coin, memo);
		if (min == -1)
			continue;
		minCoins = std::min(minCoins, min);
	}
	memo[remamount] = minCoins == std::numeric_limits<int>::max() ? -1 : 1 + minCoins;
	return memo[remamount];
}
/*
* Added a memo table to avoid recalculation
*/
int coinChange2(std::vector<int>& coins, int amount) 
{
	std::vector<int> memo(amount + 1, -1);
	return coinChangeHelper2(coins, amount, memo);
}

/*
* The premise is still the same. The number of coins to generate an amount x is the (amount - coins) + 1 
*/
int coinChange3(std::vector<int>& coins, int amount) {
	std::vector<int> memo(amount + 1, amount + 1);
	memo[0] = 0;
	for (int i = 1; i < memo.size(); ++i)
	{
		for (int coin : coins)
		{
			if (i - coin < 0)
				continue;
			memo[i] = std::min(memo[i], memo[i - coin] + 1);
		}
	}
	return memo[amount] == amount + 1 ? -1 : memo[amount];
}

/*
* This returns just one decomposition but if we wanted all decompositions we would have to store every possible
* j in inner loop that yields a decomposition.
* The premise behind this solution is as follows. We need to mark the boundries where we have either of the following
* 1. Have found an entry in the dictionary from 0 - i.
* 2. Not found an entry from 0-i but there exist a j < i where 0-j has been marked previously and j - i exist in dictionary
* Time Complexity: O(n^3) because the substr is a linear operation.
* Space Complexity: O(n)
*/
std::vector<std::string> DecomposeWordIntoDictionaryTerms(const std::string& word, const std::unordered_set<std::string>& dict)
{
	std::vector<int> last_length(word.size(), -1);
	for (int i = 0; i < word.size(); ++i)
	{
		if (dict.count(word.substr(0, i)))
		{
			last_length[i] = i + 1;
			continue;
		}
		for (int j = 0; j < i; ++j)
		{
			if (last_length[j] != -1 && dict.count(word.substr(j + 1, i - j)))
			{
				last_length[i] = i - j;
				break;
			}
		}
	}
	std::vector<std::string> decompositions{};
	if (last_length.back() != -1)
	{
		int idx = last_length.size() - 1;
		while (idx >= 0)
		{
			decompositions.push_back(word.substr(idx - last_length[idx] + 1, last_length[idx]));
			idx -= last_length[idx];
		}
	}
	std::reverse(decompositions.begin(), decompositions.end());
	return decompositions;
}

/*
* The idea is not dissimilar to the solution on the grid.
* The minimum cost at a certain point is the min cost from the 2 paths that feed into it.
* The paths at the front and back only have one path that feed into them.
* You have to decide if u want to modify the input the vector or use extra memory
*/
int MinPathWeight(const std::vector<std::vector<int>>& triangle)
{
	if (triangle.empty())
		return 0;
	std::vector<int> prev_row{triangle.front()};
	for (int i = 1; i < triangle.size(); ++i)
	{
		std::vector<int> curr_row{ triangle[i] };
		curr_row.front() += prev_row.front();
		for (int j = 1; j < curr_row.size() - 1; ++j)
		{
			curr_row[j] += std::min(prev_row[j - 1], prev_row[j]);
		}
		curr_row.back() += prev_row.back();
		prev_row.swap(curr_row); //Is move better?
	}
	return *std::min_element(prev_row.begin(), prev_row.end());
}

int NumberOfWaysToHHelper(int no_of_steps, int max_steps, std::vector<int>& no_of_ways_to_h)
{
	if (no_of_steps <= 1)
		return 1;
	if (no_of_ways_to_h[no_of_steps] == 0)
	{
		for (int i = 1; i <= max_steps && no_of_steps - i >= 0; ++i)
		{
			no_of_ways_to_h[no_of_steps] += NumberOfWaysToHHelper(no_of_steps - i, max_steps, no_of_ways_to_h);
		}
	}
	return no_of_ways_to_h[no_of_steps];
}

/*
* The key to understanding this solution is to recognise that you can take from 1 to max_steps as your first step.
* The total number of ways you can climb is therefore the sum of number of ways you can climb after taking
* 1, 2,....max_steps as your first step. F(no_of_steps, max_steps) = SIGMA F(no_of_steps - i, max_steps)
*/
int NumberOfWaysToH(int no_of_steps, int max_steps)
{
	std::vector<int> no_of_ways_to_h(no_of_steps + 1); //should default each element to 0.
	return NumberOfWaysToHHelper(no_of_steps, max_steps, no_of_ways_to_h);
}

int LongestNonDecreasingSubsequenceLength(const std::vector<int>& input)
{
	std::vector<int> ithNDSL(input.size(), 1);
	for (int i = 1; i < input.size(); ++i)
	{
		for (int j = 0; j < i; ++j)
		{
			if (input[j] <= input[i])
			{
				ithNDSL[i] = std::max(ithNDSL[i], input[j] + 1);
			}
		}
	}
	return *std::max_element(ithNDSL.begin(), ithNDSL.end());
}

//Greedy algorithm is only optimal for cents due to a special property
int CoinChange(int cents)
{
	constexpr std::array<int, 7> denominations = {100, 50, 25, 10, 5, 1};
	int no_of_coins = 0;
	for (int denom : denominations)
	{
		no_of_coins += cents / denom;
		cents %= denom;
	}
	return no_of_coins;
}

std::vector<std::pair<int, int>> OptimumTaskAssignment(std::vector<int>& tasks)
{
	std::sort(tasks.begin(), tasks.end());
	std::vector<std::pair<int, int>> res{};
	for (int i = 0, j = tasks.size() - 1; i > j; ++i, --j)
	{
		res.push_back({ tasks[i], tasks[j] });
	}
	return res;
}

/*
* We have a bunch of service times for SQL queries. How can we minimise the total time expended in running them all?
* Running the minimum service times as opposed to running the maximum service times is the correct solution.
* If u run longest queries first, every waiting query will be extended by atleast the longest
*/
int MinimumTotalWaitingTime(std::vector<int>& serviceTimes)
{
	std::sort(serviceTimes.begin(), serviceTimes.end());
	int res = 0;
	for (int i = 1; i < serviceTimes.size(); ++i)
	{
		res += serviceTimes[i - 1];
	}
	return res;
}

void dfs(int row, int col, std::vector<std::vector<char>>& grid) 
{
	if (row < 0 ||
		row >= grid.size() ||
		col < 0 ||
		col >= grid[0].size() ||
		grid[row][col] == '0')
		return;

	grid[row][col] = '0';
	dfs(row - 1, col, grid);
	dfs(row + 1, col, grid);
	dfs(row, col - 1, grid);
	dfs(row, col + 1, grid);
}

int numIslands(std::vector<std::vector<char>>& grid)
{
	int nums = 0;
	if (grid.empty())
		return nums;
	for (int row = 0; row < grid.size(); ++row)
	{
		for (int col = 0; col < grid[0].size(); ++col)
		{
			if (grid[row][col] == '1')
			{
				++nums;
				dfs(row, col, grid);
			}
		}
	}
	return nums;
}

int numIslands2(std::vector<std::vector<char>>& grid)
{
	int nums = 0;
	if (grid.empty())
		return nums;
	for (int row = 0; row < grid.size(); ++row)
	{
		for (int col = 0; col < grid[0].size(); ++col)
		{
			if (grid[row][col] == '1')
			{
				++nums;
				grid[row][col] = '0';
				std::queue<std::pair<int, int>> neighbours{};
				neighbours.push({ row, col });
				while (!neighbours.empty())
				{
					const auto [r, c] = neighbours.front();
					neighbours.pop();
					if (r - 1 >= 0 && grid[r - 1][c] == '1')
					{
						neighbours.push({ r - 1, c });
						grid[r - 1][c] = '0';
					}
					if (r + 1 < grid.size() && grid[r + 1][c] == '1')
					{
						neighbours.push({ r + 1, c });
						grid[r + 1][c] = '0';
					}
					if (c - 1 >= 0 && grid[r][c - 1] == '1')
					{
						neighbours.push({ r, c - 1 });
						grid[r][c - 1] = '0';
					}
					if (c + 1 < grid[0].size() && grid[r][c + 1] == '1')
					{
						neighbours.push({ r, c + 1 });
						grid[r][c + 1] = '0';
					}
				}
			}
		}
	}
	return nums;
}

int minPathSum(std::vector<std::vector<int>>& grid) 
{
	const int nrows = grid.size();
	const int ncols = grid[0].size();
	std::vector<std::vector<int>> memo(nrows, std::vector<int>(ncols));
	memo[nrows - 1][ncols - 1] = grid[nrows - 1][ncols - 1];
	for (int row = nrows - 1; row >= 0; --row)
	{
		for (int col = ncols - 1; col >= 0; --col)
		{
			if (row == nrows - 1 && col == ncols - 1)
				continue;
			if (row == nrows - 1)
			{
				memo[row][col] = grid[row][col] + memo[row][col + 1];
			}
			else if (col == ncols - 1)
			{
				memo[row][col] = grid[row][col] + memo[row + 1][col];
			}
			else
			{
				const int min = std::min(memo[row + 1][col], memo[row][col + 1]);
				memo[row][col] = grid[row][col] + min;
			}
		}
	}
	return memo[0][0];
}

int NumberPaths(int nrows, int ncols)
{
	std::vector<std::vector<int>> memo(nrows, std::vector<int>(ncols, 1));
	//memo[nrows - 1][ncols - 1] = 1;
	for (int row = nrows - 1; row >= 0; --row)
	{
		for (int col = ncols - 1; col >= 0; --col)
		{
			if (row == nrows - 1 && col == ncols - 1)
				continue;
			if (row == nrows - 1)
			{
				memo[row][col] = 1;
			}
			else if (col == ncols - 1)
			{
				memo[row][col] = 1;
			}
			else
			{
				memo[row][col] = memo[row + 1][col] + memo[row][col + 1];;
			}
		}
	}
	return memo[0][0];
}

bool existHelper(int row, int col, std::vector<std::vector<char>>& board, std::string& word, int idx)
{
	if (idx == word.size())
		return true;
	if (row < 0 ||
		row >= board.size() ||
		col < 0 ||
		col >= board[0].size() ||
		board[row][col] != word[idx])
	{
		return false;
	}
	std::array<int, 4> rowoffset{ {1, -1, 0, 0} };
	std::array<int, 4> coloffset{ {0, 0, 1, -1} };

	board[row][col] = '@';
	bool ret = false;

	for (int j = 0; j < 4; ++j)
	{
		ret = existHelper(row + rowoffset[j],
			col + coloffset[j],
			board,
			word,
			idx + 1);
		if (ret)
			break;
	}
	board[row][col] = word[idx];
	return ret;
}
bool exist(std::vector<std::vector<char>>& board, std::string word) 
{
	for (int row = 0; row < board.size(); ++row)
	{
		for (int col = 0; col < board[0].size(); ++col)
		{
			const bool exist = existHelper(row, col, board, word, 0);
			if (exist)
				return true;
		}
	}
	return false;
}

int maxAreaOfIslandHelper(int row, int col, std::vector<std::vector<int>>& grid)
{
	if (row < 0 ||
		row >= grid.size() ||
		col < 0 ||
		col >= grid[0].size() ||
		grid[row][col] == 0)
		return 0;
	grid[row][col] = 0;
	int size = 1;
	size += maxAreaOfIslandHelper(row - 1, col, grid);
	size += maxAreaOfIslandHelper(row + 1, col, grid);
	size += maxAreaOfIslandHelper(row, col - 1, grid);
	size += maxAreaOfIslandHelper(row, col + 1, grid);
	return size;
}
int maxAreaOfIsland(std::vector<std::vector<int>>& grid) 
{
	int maxArea = 0;
	for (int row = 0; row < grid.size(); ++row)
	{
		for (int col = 0; col < grid[0].size(); ++col)
		{
			if (grid[row][col] == 1)
			{
				maxArea = std::max(maxArea, maxAreaOfIslandHelper(row, col, grid));
			}
		}
	}
	return maxArea;
}

int maxAreaOfIsland2(std::vector<std::vector<int>>& grid) 
{
	int maxArea = 0;
	for (int row = 0; row < grid.size(); ++row)
	{
		for (int col = 0; col < grid[0].size(); ++col)
		{
			if (grid[row][col] == 1)
			{
				int size = 0;
				std::queue<std::pair<int, int>> neighbours{};
				neighbours.push({ row, col });
				grid[row][col] = 0;
				while (!neighbours.empty())
				{
					++size;
					const auto [r, c] = neighbours.front();
					neighbours.pop();
					if (r - 1 >= 0 && grid[r - 1][c] == 1)
					{
						neighbours.push({ r - 1, c });
						grid[r - 1][c] = 0;
					}
					if (r + 1 < grid.size() && grid[r + 1][c] == 1)
					{
						neighbours.push({ r + 1, c });
						grid[r + 1][c] = 0;
					}
					if (c - 1 >= 0 && grid[r][c - 1] == 1)
					{
						neighbours.push({ r, c - 1 });
						grid[r][c - 1] = 0;
					}
					if (c + 1 < grid[0].size() && grid[r][c + 1] == 1)
					{
						neighbours.push({ r, c + 1 });
						grid[r][c + 1] = 0;
					}
				}
				maxArea = std::max(maxArea, size);
			}
		}
	}
	return maxArea;
}

/*
* Brute force is to use the algo of subarray. We look at a subarray of heights and the min X distance
* The N^3 solution iterates the sublist looking for min height. This can be improved to N^2 by noting that
* the min at i can be computed at constant time if the min at i-1 is known.
*/
int largestRectangleArea2(std::vector<int>& heights)
{
	int max = 0;
	for (int i = 0; i < heights.size(); ++i)
	{
		int min = std::numeric_limits<int>::max();
		for (int j = i; j < heights.size(); ++j)
		{
			min = std::min(min, heights[j]);
			const int local_area = min * (j - i + 1);
			max = std::max(max, local_area);
		}
	}
	return max;
}
/*
* I do understand this now. The second brute force solution provides a clue.
* The max area from a bar is computed from the distance to the right up to first bar less than us and the distance to the left down to the first less than us.
* The algo below with the stacks is triggered when we find a smaller bar than us...this signals we can that we should sweep leftwards to the bar below us.
* Note that this is not computed off the bar before the one that is lower.
*/
int largestRectangleArea(std::vector<int>& heights)
{
	int max = 0;
	heights.push_back(0);
	std::stack<int> stk{};
	for (int i = 0; i < heights.size(); ++i)
	{
		while (!stk.empty() && heights[i] < heights[stk.top()])
		{
			const auto tp = stk.top();
			stk.pop();
			const int left_boundary = stk.empty() ? -1 : stk.top();
			max = std::max(max, (i - left_boundary -1) * heights[tp]);
		}
		stk.push(i);
	}
	return max;
}

/*
* Semi optimised
*/
int trap(std::vector<int>& height) {
	//Get the max to the left of me and the max to the right of me
	//The water held at i is the min(maxleft, maxright) - height[i]
	int trapped = 0;
	if (height.empty())
		return trapped;
	std::vector<int> left_max(height.size());
	std::vector<int> right_max(height.size());
	left_max[0] = height[0];
	for (int i = 1; i < left_max.size(); ++i)
	{
		left_max[i] = std::max(left_max[i - 1], height[i]);
	}
	right_max[height.size() - 1] = height.back();
	for (int i = right_max.size() - 2; i >= 0; --i)
	{
		right_max[i] = std::max(right_max[i + 1], height[i]);
	}

	for (int i = 0; i < height.size(); ++i)
	{
		trapped += std::min(left_max[i], right_max[i]) - height[i];
	}
	return trapped;
}

/*
* Optimised
*/
int trap2(std::vector<int>& height)
{
	int trapped = 0;
	if (height.empty())
		return trapped;
	std::stack<int> stk{};
	for (int i = 0; i < height.size(); ++i)
	{
		while (!stk.empty() && height[i] > height[stk.top()])
		{
			const auto top = stk.top();
			stk.pop();
			if (stk.empty())
				break;
			const int dist = i - stk.top() - 1;
			const int bounded_height = std::min(height[i], height[stk.top()]) - height[top];
			trapped += dist * bounded_height;
		}
		stk.push(i);
	}
	return trapped;
}

std::vector<std::vector<std::string>> GetGroups(const std::vector<std::string>& words, int maxWidth)
{
	std::vector<std::string> group{};
	std::vector<std::vector<std::string>> groups{};
	size_t remaining = maxWidth;
	for (const auto& aword : words)
	{
		size_t minWidth = !group.empty() + aword.size();

		if (minWidth > remaining)
		{
			groups.push_back(std::move(group));
			group.clear();
			remaining = maxWidth;
			minWidth = aword.size();
		}
		group.push_back(aword);
		remaining -= minWidth;
	}
	if (!group.empty())
		groups.push_back(group);
	return groups;
}