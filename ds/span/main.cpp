#include <format>
#include <iostream>
#include <vector>

#include "span.hpp"

auto main() -> int {
	std::vector<float> vec{1, 2, 3, 4, 5};
	stdImpl::span<float> s(vec);

	for (auto elem : s) {
		std::cout << std::format("{}", elem) << "\n";
	}

	std::cout << std::format("{}", s[2]) << "\n";

	return 0;
}
