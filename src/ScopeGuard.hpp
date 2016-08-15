#pragma once

#ifndef TDF_SCOPE_GUARD_HPP
#define TDF_SCOPE_GUARD_HPP

#include <functional>

class ScopeGuard
{
private:
	std::function<void()> f_;

public:
	template<typename F>
	ScopeGuard(F && f) : f_(std::forward<F>(f)) {}

	template<typename F>
	ScopeGuard(ScopeGuard && g) : f_(std::move(g.f_))
	{
		g.f_ = nullptr;
	}

	~ScopeGuard()
	{
		if (f_)
			f_();
	}

	void dismiss() throw()
	{
		f_ = nullptr;
	}
};

#endif // TDF_SCOPE_GUARD_HPP
