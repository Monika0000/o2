#pragma once

#include "o2/Utils/Reflection/Type.h"

namespace o2
{
	class IAbstractValueProxy
	{
	public:
		virtual ~IAbstractValueProxy() {}

		virtual void SetValuePtr(void* value) = 0;
		virtual void GetValuePtr(void* value) const = 0;
		virtual const Type& GetType() const = 0;
	};

	template<typename _type>
	class IValueProxy: public IAbstractValueProxy
	{
	public:
		virtual void SetValue(const _type& value) = 0;
		virtual _type GetValue() const = 0;

		void SetValuePtr(void* value) override { SetValue(*(_type*)value); }
		void GetValuePtr(void* value) const override { *(_type*)value = GetValue(); }
		const Type& GetType() const override { return TypeOf(_type); }
	};

	class IPointerValueProxy
	{
	public:
		virtual void* GetValueVoidPointer() const = 0;
	};

	template<typename _type>
	class PointerValueProxy: public IValueProxy<_type>, public IPointerValueProxy
	{
	protected:
		_type* mValuePtr = nullptr;

	public:
		PointerValueProxy() {}
		PointerValueProxy(_type* valuePtr):mValuePtr(valuePtr) {}

		void SetValue(const _type& value) override { *mValuePtr = value; }
		_type GetValue() const override { return *mValuePtr; }

		_type* GetValuePointer() const { return mValuePtr; }
		void* GetValueVoidPointer() const override { return (void*)mValuePtr; }
	};
}
