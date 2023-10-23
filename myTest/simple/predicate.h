#include <iostream>
#include <algorithm>
#include <vector>

/** 
 * extracted from cpp primer 5 chap. 14 newcount-size.cc
*/

// return the plural version of word if ctr is greater than 1
inline std::string make_plural(size_t ctr,
                               const std::string& word, 
                               const std::string& ending)
{
	return (ctr > 1) ? word + ending : word;
}

class SizeComp
{
  size_t sz;  // a data member for each variable captured by value
public:
  SizeComp() = delete;  // no default constructor
  SizeComp& operator=(const SizeComp&) = delete;  // no assignment
  ~SizeComp() = default;

  // constructor with a parameter for each captured variable
  SizeComp(size_t n) : sz(n) {}

  // call operator with the same return type,
  // parameters, and body as the lambda
  bool operator()(const std::string& s) const
  {
    return s.size();
  }
};

class PrintString
{
  std::ostream& os;

public:
  PrintString() = delete;   // no default constructor
  PrintString(std::ostream& o) : os(o) {}
  void operator()(const std::string& s) const
  {
    os << s << " ";
  }
};

class ShorterString
{
public:
  bool operator()(const std::string& s1, const std::string& s2) const
  {
    return s1.size() < s2.size();
  }
};

void elim_dups(std::vector<std::string>& words)
{
  // sort words alphabetically so we can find the duplicates
  std::sort(words.begin(), words.end());

  // print the sorted contents
  std::for_each(words.begin(), words.end(), PrintString(std::cerr));
  std::cerr << std::endl;

  // unique reorders the input so that each word appears once in the
  // front part of the range
  // returns an iterator one past the unique range
  auto endUnique = std::unique(words.begin(), words.end());

  // erase uses a vector operation to remove the nonunique elements
  words.erase(endUnique, words.end());

  // print the reduced vector
  std::for_each(words.begin(), words.end(), PrintString(std::cerr));
  std::cerr << std::endl;
}

void biggies(std::vector<std::string>& words, std::vector<std::string>::size_type sz)
{
  // puts words in alphabetic order and removes duplicates
  elim_dups(words);
  
  // sort words by size using object of type ShorterString
  // maintaining alphabetic order for words of the same size
  std::stable_sort(words.begin(), words.end(), ShorterString());

  // use object of type SizeComp to find
  // the first element whose size() is >= sz
  auto wc = std::find_if(words.begin(), words.end(), SizeComp(sz));

  // compute the number of elements with size >= sz
  auto count = words.end() - wc;

  // print results
  std::cout << count << " " << make_plural(count, "word", "s")
            << " " << sz << " characters or longer" << std::endl;
  
  // use object of type PringString
  // to print the contents of words, each one followed by a space
  std::for_each(wc, words.end(), PrintString(std::cout));
  std::cout << std::endl;
}
