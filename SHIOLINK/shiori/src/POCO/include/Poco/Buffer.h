//
// Buffer.h
//
// $Id: //poco/1.3/Foundation/include/Poco/Buffer.h#1 $
//
// Library: Foundation
// Package: Core
// Module:  Buffer
//
// Definition of the Buffer class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#ifndef Foundation_Buffer_INCLUDED
#define Foundation_Buffer_INCLUDED


#include "Poco/Foundation.h"
#include <cstddef>


namespace Poco {


template <class T>
class Buffer
	/// A very simple buffer class that allocates a buffer of
	/// a given type and size in the constructor and
	/// deallocates the buffer in the destructor.
	///
	/// This class is useful everywhere where a temporary buffer
	/// is needed.
{
public:
	Buffer(std::size_t size):
		_size(size),
		_ptr(new T[size])
		/// Creates and allocates the Buffer.
	{
	}
	
	~Buffer()
		/// Destroys the Buffer.
	{
		delete [] _ptr;
	}
	
	std::size_t size() const
		/// Returns the size of the buffer.
	{
		return _size;
	}
	
	T* begin()
		/// Returns a pointer to the beginning of the buffer.
	{
		return _ptr;
	}
	
	const T* begin() const
		/// Returns a pointer to the beginning of the buffer.
	{
		return _ptr;
	}

	T* end()
		/// Returns a pointer to end of the buffer.
	{
		return _ptr + _size;
	}
	
	const T* end() const
		/// Returns a pointer to the end of the buffer.
	{
		return _ptr + _size;
	}
	
	T& operator [] (std::size_t index)
	{
		poco_assert (index >= 0 && index < _size);
		
		return _ptr[index];
	}

	const T& operator [] (std::size_t index) const
	{
		poco_assert (index >= 0 && index < _size);
		
		return _ptr[index];
	}

private:
	Buffer();
	Buffer(const Buffer&);
	Buffer& operator = (const Buffer&);

	std::size_t _size;
	T* _ptr;
};


} // namespace Poco


#endif // Foundation_Buffer_INCLUDED
