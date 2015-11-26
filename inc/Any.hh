#pragma once
#include <memory>
#include <typeindex>
#include <exception>
#include <iostream>

/**
 * \brief [API] Any类，可储存任何copyable的类型.
 * \example
 *      Any a = "const char*";
 *      if(a.is<const char*>())
 *          std::cout << a.get<const char*>() << std::endl;
 */
struct Any
{
	Any(void) : tp_index_(std::type_index(typeid(void))) {}

	Any(const Any& that) : ptr_(that.clone()), tp_index_(that.tp_index_) {}

	Any(Any && that) : ptr_(std::move(that.ptr_)), tp_index_(that.tp_index_) {}

	/** 创建智能指针时，对于一般的类型，通过std::decay来移除引用和cv符，从而获取原始类型 */
	template<typename U, class = typename std::enable_if<!std::is_same<typename std::decay<U>::type, Any>::value, U>::type> 
    Any(U && value) : ptr_(new Derived<typename std::decay<U>::type>(std::forward<U>(value))),
		tp_index_(std::type_index(typeid(typename std::decay<U>::type))) {}

	bool isNull() const { return !bool(ptr_); }

	template<class U> bool is() const
	{
		return tp_index_ == std::type_index(typeid(U));
	}

	/* 将Any转换为实际的类型 */
	template<class U>
	U& get()
	{
		if (!is<U>())
		{
			std::cout << "can not cast " << typeid(U).name() << " to " << tp_index_.name() << std::endl;
			throw std::bad_cast{};
		}

		auto derived = dynamic_cast<Derived<U>*> (ptr_.get());
		return derived->value;
	}

	Any& operator=(const Any& a)
	{
		if (ptr_ == a.ptr_)
			return *this;

		ptr_ = a.clone();
		tp_index_ = a.tp_index_;
		return *this;
	}

private:
	struct Base_;
	typedef std::unique_ptr<Base_> BasePtr_;

	struct Base_
	{
		virtual ~Base_() {}
		virtual BasePtr_ clone() const = 0;
	};

	template<typename T>
	struct Derived : Base_
	{
		template<typename U>
		Derived(U && value) : value(std::forward<U>(value)) { }

		BasePtr_ clone() const
		{
			return BasePtr_(new Derived<T>(value));
		}

		T value;
	};

	BasePtr_ clone() const
	{
		if (ptr_ != nullptr)
			return ptr_->clone();

		return nullptr;
	}

	BasePtr_ ptr_;
	std::type_index tp_index_;
};
