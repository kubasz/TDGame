#pragma once

#ifndef TDF_MAKE_UNIQUE_HPP
#define TDF_MAKE_UNIQUE_HPP

#include <memory>

#if defined(__cplusplus) && __cplusplus < 201402L

// A Herb Sutter's placeholder implementation of make_unique

namespace std {

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&& ...args)
{
	return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

}

#endif // defined(__cplusplus) && __cplusplus >= 201402L

#endif // TDF_MAKE_UNIQUE_HPP
