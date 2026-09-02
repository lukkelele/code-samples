#pragma once

#ifdef PROFILER_ENABLED
#include <tracy/Tracy.hpp>

#ifndef PROFILER_SCOPED_TIMERS
#define PROFILER_SCOPED(...) ZoneScoped##__VA_OPT__(N(__VA_ARGS__))
#else
/* Scoped timers are used instead of tracy. */
#include "timer.h"
#define PROFILER_TIMER_CONCAT_INNER(a, b) a##b
#define PROFILER_TIMER_CONCAT(a, b)       PROFILER_TIMER_CONCAT_INNER(a, b)
#define PROFILER_TIMER(...)                                                     \
	const ::core::ScopedTimer PROFILER_TIMER_CONCAT(_profiler_timer_, __LINE__) \
	{                                                                           \
		__func__ __VA_OPT__(, ) __VA_ARGS__                                     \
	}
#define PROFILER_SCOPED(...) PROFILER_TIMER(__VA_ARGS__)
#endif /* PROFILER_SCOPED_TIMERS */

#define PROFILER_MARK_FRAME()                                    FrameMark
#define PROFILER_MARK_FRAME_BEGIN(_name)                         FrameMarkStart(_name)
#define PROFILER_MARK_FRAME_END(_name)                           FrameMarkEnd(_name)
#define PROFILER_THREAD(...)                                     tracy::SetThreadName(__VA_ARGS__)
#define PROFILER_LOCKABLE(_type, _var)                           TracyLockable(_type, _var)
#define PROFILER_LOCKABLE_N(_type, _var, _desc)                  TracyLockableN(_type, _var, _desc)
#define PROFILER_LOCKABLE_BASE(_type)                            LockableBase(_type)
#define PROFILER_PLOT(_name, _value)                             TracyPlot(_name, _value)
#define PROFILER_PLOT_CONFIG(_name, _type, _step, _fill, _color) TracyPlotConfig(_name, _type, _step, _fill, _color)
#define PROFILER_MESSAGE(_txt, _size)                            TracyMessage(_txt, _size)
#define PROFILER_MESSAGE_L(_txt)                                 TracyMessageL(_txt)
#else
#define PROFILER_SCOPED(...)
#define PROFILER_MARK_FRAME()
#define PROFILER_MARK_FRAME_BEGIN(_name)
#define PROFILER_MARK_FRAME_END(_name)
#define PROFILER_THREAD(...)
#define PROFILER_LOCKABLE(_type, _var)          _type _var
#define PROFILER_LOCKABLE_N(_type, _var, _desc) _type _var
#define PROFILER_LOCKABLE_BASE(_type)           _type
#define PROFILER_PLOT(_name, _value)
#define PROFILER_PLOT_CONFIG(_name, _type, _step, _fill, _color)
#define PROFILER_MESSAGE(_txt, _size)
#define PROFILER_MESSAGE_L(_txt)
#endif /* PROFILER_ENABLED */
