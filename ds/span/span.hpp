#pragma once
#include <concepts>
#include <cstddef>
#include <iterator>
#include <memory>
#include <span>
#include <type_traits>

namespace stdImpl {

template <typename Range>
concept begin_end_iterators = requires(Range range) {
	{ range.end() };
	{ range.begin() } -> std::same_as<decltype(range.end())>;
};

template <typename Iterator>
concept contiguous_iterator = requires(Iterator iterator) {
	{ iterator + 1 } -> std::same_as<Iterator>;
	{ iterator - 1 } -> std::same_as<Iterator>;
	{ *iterator } -> std::same_as<typename std::iterator_traits<Iterator>::reference>;
};

template <typename Range>
concept data_and_size_member = requires(Range range) {
	{ range.size() } -> std::same_as<std::size_t>;
	{ range.data() } -> std::same_as<typename std::remove_reference_t<Range>::value_type*>;
};

template <typename Range>
concept contiguous_range =
    begin_end_iterators<Range> && contiguous_iterator<decltype(std::declval<Range>().begin())> &&
    contiguous_iterator<decltype(std::declval<Range>().end())> && data_and_size_member<Range> && requires(Range range) {
	    { std::data(range) } -> std::same_as<typename std::remove_reference_t<Range>::value_type*>;
    };

// Forward Declaration
template <typename T>
class SpanIterator;

template <typename T, std::size_t Extent = std::dynamic_extent>
class span {
public:
	using type = T;
	using value_type = std::remove_cv_t<T>;
	using size_type = std::size_t;
	using pointer = T*;
	using reference = T&;
	using const_pointer = const T*;
	using const_reference = const T&;
	using difference_type = std::ptrdiff_t;
	static constexpr std::size_t extent = Extent;

	// Constructors
	/*
	I only want to support three constructors here:
	    1.) Constructed from start iterator and count
	    2.) Constructed from two iterators that mark begin/end
	    3.) Constructed from a range container
	*/
	template <contiguous_iterator It>
	constexpr explicit(extent == std::dynamic_extent) span(It first, size_type count) noexcept
	    : ptr_(std::to_address(first)), size_(count) {}
	template <contiguous_iterator It>
	constexpr explicit(extent == std::dynamic_extent) span(It first, It end) noexcept
	    : ptr_(std::to_address(first)), size_(end - first) {}
	template <contiguous_iterator It>
	constexpr explicit(extent != std::dynamic_extent) span(It first) noexcept : ptr_(std::to_address(first)) {}

	template <contiguous_range Range>
	constexpr explicit span(Range&& range) : ptr_(range.data()) {
		if constexpr (extent != std::dynamic_extent) {
			if (extent > range.size()) {
				throw std::length_error("Range size is smaller than static extent -> UB");
			}
		} else {
			size_ = range.size();
		}
	}

	// Assignment and Copy
	/*
	    Skip for now
	*/

	// Accessors
	constexpr auto operator[](size_type index) const -> reference { return *(ptr_ + index); }
	constexpr auto front() const -> reference { return *(ptr_); }
	constexpr auto back() const -> reference { return *(ptr_ + size() - 1); }
	constexpr auto data() const noexcept -> pointer { return ptr_; }

	// Attributes
	[[nodiscard]] constexpr auto size() const noexcept -> size_type {
		if constexpr (extent != std::dynamic_extent) {
			return extent;
		} else {
			return size_;
		}
	}

	// Iterators
	constexpr auto begin() noexcept { return SpanIterator<type>(ptr_); }
	constexpr auto end() noexcept { return SpanIterator<type>(ptr_ + size()); }

private:
	pointer ptr_{nullptr};
	std::conditional_t<(extent == std::dynamic_extent), size_type, std::integral_constant<size_type, extent>> size_;
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