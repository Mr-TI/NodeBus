/*
 * Copyright (C) 2012-2014 Emeric Verschuur <emericv@mbedsys.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef NODEBUS_POINTER_H
#define NODEBUS_POINTER_H

#include <nodebus/core/shareddata.h>

namespace NodeBus {

/**
 * @brief NodeBus : Shared pointer
 * 
 * @author <a href="mailto:emericv@mbedsys.org">Emeric Verschuur</a>
 * @copyright Copyright (C) 2012-2014 MBEDSYS SAS
 * This library is released under the GNU Lesser General Public version 2.1
 */
template <typename T> class SharedPtr {
	T *m_data;
public:
	/**
	 * @brief Default shared pointer constructor
	 */
	SharedPtr();
	
	/**
	 * @brief Shared pointer constructor from a data address
	 * 
	 * @param data address to a data
	 */
	SharedPtr(T *data);
	
	/**
	 * @brief Shared pointer constructor from a equivalent type pointer
	 * 
	 * @param data other pointer
	 */
	SharedPtr(const SharedPtr<T> &other);
	
	/**
	 * @brief Shared pointer constructor from a other type pointer
	 * 
	 * @param other other pointer
	 */
	template<class X>
	SharedPtr(const SharedPtr<X> &other);
	
	/**
	 * @brief Shared pointer destructor
	 */
	~SharedPtr();
	
	/**
	 * @brief Get the internal data address
	 * 
	 * @return the internal data address
	 */
	T *data();
	
	/**
	 * @brief Get the internal data address
	 * 
	 * @return the internal data address
	 */
	const T *data() const;
	
	/**
	 * @brief Update the internal pointer to a given data address
	 * 
	 * @param data data address
	 * @return a reference to the pointer
	 */
	SharedPtr<T> &operator= (const T *data);
	
	/**
	 * @brief Affectation operator
	 * 
	 * @param other pointer
	 * @return a reference to the pointer
	 */
	SharedPtr<T> &operator= (const SharedPtr<T>& other);
	
	/**
	 * @brief Affectation operator
	 * 
	 * @param other pointer
	 * @return a reference to the pointer
	 */
	template<class X>
	SharedPtr<T> &operator= (const SharedPtr<X>& other);
	
	/**
	 * @brief Dereference pointer operator
	 * 
	 * @return a reference to the pointed data
	 */
	T &operator*() const;
	
	/**
	 * @brief Dereference pointer operator
	 * 
	 * @return a reference to the pointed data
	 */
	T *operator->();
	
	
	T *operator->() const;
	
	/**
	 * @brief Equality test operator between this pointer and a data address
	 * 
	 * @return true if the internal pointer and a given data address are equal, otherwise false
	 */
	bool operator== (const void *data) const;
	
	/**
	 * @brief Equality test operator between this pointer and a an other given pointer
	 * 
	 * @return true if this pointer and an other given pointer have the same data address, otherwise false
	 */
	bool operator== (const SharedPtr<T>& other) const;
	
	/**
	 * @brief Equality test operator between this pointer and a an other given pointer
	 * 
	 * @return true if this pointer and an other given pointer have the same data address, otherwise false
	 */
	template<class X>
	bool operator== (const SharedPtr<X>& other) const;
	
	/**
	 * @brief Difference test operator between this pointer and a data address
	 * 
	 * @return true if the internal pointer and a given data address are different, otherwise false
	 */
	bool operator!= (const void *data) const;
	
	/**
	 * @brief Difference test operator between this pointer and a an other given pointer
	 * 
	 * @return true if this pointer and an other given pointer don't have the same data address, otherwise false
	 */
	bool operator!= (const SharedPtr<T>& other) const;
	
	/**
	 * @brief Difference test operator between this pointer and a an other given pointer
	 * 
	 * @return true if this pointer and an other given pointer don't have the same data address, otherwise false
	 */
	template<class X>
	bool operator!= (const SharedPtr<X>& other) const;
	
	/**
	 * @brief Test if a class instance of a given type
	 * 
	 * @return true if the class instance of a given type
	 */
	template<class X>
	bool instanceof ();
	
	/**
	 * @brief Cast this pointer to a given type
	 * 
	 * @return a pointer as given type
	 */
	template<class X>
	X* cast ();
};

/// @brief Generic pointer type
typedef SharedPtr<SharedData> GenericPtr;

extern void __raise_InvalidClassException();
extern void __raise_NullPointerException();
void __log_data_ref_init(void* data);
void __log_data_ref_delete(void* data);

#ifdef NODEBUS_SHAREDPTR_DEBUG
#define __NODEBUS_SHAREDPTR_DEBUG_NEW() if (m_data->ptrNbRef == 0) __log_data_ref_init(m_data)
#define __NODEBUS_SHAREDPTR_DEBUG_DEL() __log_data_ref_delete(m_data)
#else
#define __NODEBUS_SHAREDPTR_DEBUG_NEW()
#define __NODEBUS_SHAREDPTR_DEBUG_DEL()
#endif

template <typename T>
inline SharedPtr<T>::SharedPtr(): m_data(nullptr) {}
template <typename T>
inline SharedPtr<T>::~SharedPtr() {
	if (m_data != nullptr) {
		if (m_data->ptrNbRef.fetch_sub(1) == 1) {
			__NODEBUS_SHAREDPTR_DEBUG_DEL();
			delete m_data;
		}
	}
}
template <typename T>
inline SharedPtr<T>::SharedPtr(T* data): m_data(data) {
	if (m_data != nullptr) {
		if (data != nullptr && dynamic_cast<const T*>(data) == nullptr) {
			__raise_InvalidClassException();
		}
		__NODEBUS_SHAREDPTR_DEBUG_NEW();
		m_data->ptrNbRef++;
	}
}
template <typename T>
inline SharedPtr<T>::SharedPtr(const SharedPtr< T >& other): m_data(other.m_data) {
	if (m_data != nullptr) {
		if (other.data() != nullptr && dynamic_cast<const T*>(other.data()) == nullptr) {
			__raise_InvalidClassException();
		}
		__NODEBUS_SHAREDPTR_DEBUG_NEW();
		m_data->ptrNbRef++;
	}
}
template <typename T>
template <typename X>
inline SharedPtr<T>::SharedPtr(const SharedPtr< X >& other): m_data((T*)(other.data())) {
	if (m_data != nullptr) {
		if (other.data() != nullptr && dynamic_cast<const T*>(other.data()) == nullptr) {
			__raise_InvalidClassException();
		}
		__NODEBUS_SHAREDPTR_DEBUG_NEW();
		m_data->ptrNbRef++;
	}
}
template <typename T>
inline T* SharedPtr<T>::data() {
	return m_data;
}
template <typename T>
inline const T* SharedPtr<T>::data() const {
	return m_data;
}
template <typename T>
inline SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<T>& other) {
	return operator=(other.m_data);
}
template <typename T>
template <typename X>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<X>& other) {
	if (m_data == (T*)(other.data())) {
		return *this;
	} else if (m_data != nullptr) {
		if (m_data->ptrNbRef.fetch_sub(1) == 1) {
			__NODEBUS_SHAREDPTR_DEBUG_DEL();
			delete m_data;
		}
	}
	if (other.data() != nullptr && dynamic_cast<const T*>(other.data()) == nullptr) {
		__raise_InvalidClassException();
	}
	m_data = (T*)(other.data());
	if (m_data != nullptr) {
		__NODEBUS_SHAREDPTR_DEBUG_NEW();
		m_data->ptrNbRef++;
	}
	return *this;
}
template <typename T>
SharedPtr<T>& SharedPtr<T>::operator=(const T *data) {
	if (m_data == data) {
		return *this;
	} else if (m_data != nullptr) {
		if (m_data->ptrNbRef.fetch_sub(1) == 1) {
			__NODEBUS_SHAREDPTR_DEBUG_DEL();
			delete m_data;
		}
	}
	if (data != nullptr && dynamic_cast<const T*>(data) == nullptr) {
		__raise_InvalidClassException();
	}
	m_data = const_cast<T*>(data);
	if (m_data != nullptr) {
		__NODEBUS_SHAREDPTR_DEBUG_NEW();
		m_data->ptrNbRef++;
	}
	return *this;
}
template <typename T>
inline bool SharedPtr<T>::operator==(const SharedPtr<T>& other) const {
	return m_data == other.m_data;
}
template <typename T>
template <typename X>
inline bool SharedPtr<T>::operator==(const SharedPtr<X>& other) const {
	return m_data == other.data();
}
template <typename T>
inline bool SharedPtr<T>::operator==(const void *data) const {
	return m_data == data;
}
template <typename T>
inline bool SharedPtr<T>::operator!=(const SharedPtr<T>& other) const {
	return m_data != other.m_data;
}
template <typename T>
template <typename X>
inline bool SharedPtr<T>::operator!=(const SharedPtr<X>& other) const {
	return m_data != other.data();
}
template <typename T>
inline bool SharedPtr<T>::operator!=(const void *data) const {
	return m_data != data;
}
template <typename T>
inline T &SharedPtr<T>::operator*() const {
	if (m_data == nullptr) {
		__raise_NullPointerException();
	}
	return *m_data;
}
template <typename T>
inline T *SharedPtr<T>::operator->() {
	if (m_data == nullptr) {
		__raise_NullPointerException();
	}
	return m_data;
}
template <typename T>
inline T *SharedPtr<T>::operator->() const {
	if (m_data == nullptr) {
		__raise_NullPointerException();
	}
	return m_data;
}

template<class T>
template<class X>
inline bool SharedPtr<T>::instanceof () {
	return dynamic_cast<X*>(m_data) != nullptr;
}

template<class T>
template<class X>
inline X *SharedPtr<T>::cast () {
	X *x = dynamic_cast<X*>(m_data);
	if (x == nullptr) {
		__raise_InvalidClassException();
	}
	return x;
}

}

#endif // NODEBUS_POINTER_H
