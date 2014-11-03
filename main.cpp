#include "query.h"
#include <iostream>
#include <vector>
#include <iostream>
#include <string>

struct Foo
{
    int a;
};

bool sort_pairs(
        const std::pair<char, int>& lhs,
        const std::pair<char, int>& rhs
)
{
    return lhs.second < rhs.second;
}

int main() {
    std::cout << "Hello, World!" << std::endl;

    std::string hello = "Hello, World!";


	auto q =
		lift(hello.begin(), hello.end())
		>> zip_with(from_range_infinite(0))
		>> orderby(sort_pairs, false)
		>> select([](std::pair<char, int> item){return item.first; });
//		>> select([](char i){return i; });

	auto it = q.begin();
	auto v = *it;
	auto sit = ++it;




    for(auto i : q)
        std::cout << i << std::endl;

	std::string rev(q.begin(), q.end());
	std::cout << rev << std::endl;

    return 0;
}
