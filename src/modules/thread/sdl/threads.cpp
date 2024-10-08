/**
 * Copyright (c) 2006-2024 LOVE Development Team
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 **/

#include "threads.h"
#include "Thread.h"

namespace love
{
namespace thread
{
namespace sdl
{

Mutex::Mutex()
{
	mutex = SDL_CreateMutex();
}

Mutex::~Mutex()
{
	SDL_DestroyMutex(mutex);
}

void Mutex::lock()
{
	SDL_LockMutex(mutex);
}

void Mutex::unlock()
{
	SDL_UnlockMutex(mutex);
}

Conditional::Conditional()
{
#if SDL_VERSION_ATLEAST(3, 0, 0)
	cond = SDL_CreateCondition();
#else
	cond = SDL_CreateCond();
#endif
}

Conditional::~Conditional()
{
#if SDL_VERSION_ATLEAST(3, 0, 0)
	SDL_DestroyCondition(cond);
#else
	SDL_DestroyCond(cond);
#endif
}

void Conditional::signal()
{
#if SDL_VERSION_ATLEAST(3, 0, 0)
	SDL_SignalCondition(cond);
#else
	SDL_CondSignal(cond);
#endif
}

void Conditional::broadcast()
{
#if SDL_VERSION_ATLEAST(3, 0, 0)
	SDL_BroadcastCondition(cond);
#else
	SDL_CondBroadcast(cond);
#endif
}

bool Conditional::wait(thread::Mutex *_mutex, int timeout)
{
	// Yes, I realise this can be dangerous,
	// however, you're asking for it if you're
	// mixing thread implementations.
	Mutex *mutex = (Mutex *) _mutex;
#if SDL_VERSION_ATLEAST(3, 0, 0)
	if (timeout < 0)
	{
		SDL_WaitCondition(cond, mutex->mutex);
		return true;
	}
	else
		return SDL_WaitConditionTimeout(cond, mutex->mutex, timeout);
#else
	if (timeout < 0)
		return !SDL_CondWait(cond, mutex->mutex);
	else
		return (SDL_CondWaitTimeout(cond, mutex->mutex, timeout) == 0);
#endif
}

} // sdl


/**
 * Implementations of the functions declared in src/modules/threads.h.
 **/

thread::Mutex *newMutex()
{
	return new sdl::Mutex();
}

thread::Conditional *newConditional()
{
	return new sdl::Conditional();
}

thread::Thread *newThread(Threadable *t)
{
	return new sdl::Thread(t);
}

} // thread
} // love
