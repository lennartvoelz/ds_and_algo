#pragma once
#include <cstddef>
#include <iterator>
#include <span>
#include <type_traits>

namespace stdImpl {

// Forward Declaration
template <typename T>
class SpanIterator;

template <typename T, std::size_t Extent = std::dynamic_extent>
class span {
public:
	using type = T;
	using value_type = std::remove_cv_t<T>;
	using size_type = std::size_t;

	// Constructors

	// Iterators
	constexpr auto begin() { return SpanIterator<type>(ptr_); }
	constexpr auto end() { return SpanIterator<type>(ptr_ + N); }

private:
	type* ptr_;
	size_type size_{0};
};

template <typename T>
class SpanIterator {
public:
	using type = T;
	using reference = T&;
	using pointer = T*;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using iterator_category = std::random_access_iterator_tag;

	// Constructors
	constexpr SpanIterator() = default;
	constexpr explicit SpanIterator(type* ptr) : currentPtr_(ptr) {}

	// Dereferencing
	constexpr auto operator*() -> reference { return *currentPtr_; }
	constexpr auto operator->() -> pointer { return currentPtr_; }

	// Iterating
	constexpr auto operator++() -> SpanIterator& {
		currentPtr_++;
		return *this;
	}
	constexpr auto operator--() -> SpanIterator& {
		currentPtr_--;
		return *this;
	}

	// Difference Types
	constexpr auto operator+=(difference_type n) -> SpanIterator& {
		currentPtr_ += n;
		return *this;
	}
	constexpr auto operator+(difference_type n) const -> SpanIterator {
		auto tempIt = *this;
		tempIt += n;
		return tempIt;
	}
	constexpr auto operator-=(difference_type n) -> SpanIterator& {
		currentPtr_ -= n;
		return *this;
	}
	constexpr auto operator-(difference_type n) const -> SpanIterator {
		auto tempIt = *this;
		tempIt -= n;
		return tempIt;
	}

	// Difference between two Iterators
	constexpr auto operator-(const SpanIterator& other) const -> difference_type {
		return currentPtr_ - other.currentPtr_;
	}

	// Random Access
	constexpr auto operator[](difference_type n) const -> reference { return *(currentPtr_ + n); }

	// Comparison Operators
	constexpr auto operator==(const SpanIterator& other) const -> bool { return currentPtr_ == other.currentPtr_; }
	constexpr auto operator!=(const SpanIterator& other) const -> bool { return !(currentPtr_ == other.currentPtr_); }

private:
	type* currentPtr_{nullptr};
};
}  // namespace stdImpl