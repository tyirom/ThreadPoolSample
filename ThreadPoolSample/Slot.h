// --------------------------------------------------------------------------------------------------------------------
// <copyright file="Slot.h" company="Grass Valley K.K.">
//   Copyright (C) 2016 Grass Valley K.K. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------
#pragma once
#include <functional>
#include <vector>
#include <winerror.h>

namespace Slot
{
#if 0 // うまくいかなかった。
	template <class T>
	class Slot
	{
	public:
		using Type = std::function<T>;
		Type m_slot;

		void Connect(Type slot)
		{
			m_slot = slot;
		}
	};

	template <class T, class... Rest>
	class Slot2
	{
	public:
		using Type = T;
		using Func = std::function<T>;
		Func m_slot;

		void Connect(Func slot)
		{
			m_slot = slot;
		}
	};

	template <class... Types>
	class Slot3
	{
	public:
		using Type = typename Slot2<Types...>::Type;
		using Func = typename Slot2<Types...>::Func;
		Func m_slot;

		void Connect(Func slot)
		{
			m_slot = slot;
		}
	};

	template <class... Ts>
	class Slot
	{
		std::tuple<std::function<Ts>...> m_slots;

		template <class T, class... Rest>
		void Connect(std::function<T> slot)
		{

		}
	};
#endif

	template <class T1, class T2>
	struct SameType
	{
		static const bool value = false;
	};

	template <class T>
	struct SameType<T, T>
	{
		static const bool value = true;
	};

	template <typename... Types>
	class MyClass
	{
	public:
		typedef std::tuple<std::vector<Types>...> vtype;
		vtype vectors;

		template <int N, typename T>
		struct VectorOfType : SameType<T, typename std::tuple_element<N, vtype>::type::value_type>
		{ };

		template <int N, class T, class Tuple, bool Match = false>
		struct MatchingField
		{
			static std::vector<T>& get(Tuple& tp)
			{
				return MatchingField<N + 1, T, Tuple, VectorOfType<N + 1, T>::value>::get(tp);
			}
		};

		template <int N, class T, class Tuple>
		struct MatchingField<N, T, Tuple, true>
		{
			static std::vector<T>& get(Tuple& tp)
			{
				return std::get<N>(tp);
			}
		};

		template <typename T>
		std::vector<T>& access()
		{
			return MatchingField<0, T, vtype, VectorOfType<0, T>::value>::get(vectors);
		}
	};

	template <typename... Ts>
	class Slot2
	{
	public:
		typedef std::tuple<Ts...> SlotType;
		SlotType m_slots;

		template <int N, typename T>
		struct Checker : SameType<T, typename std::tuple_element<N, SlotType>::type>
		{
		};

		template <int N, class T, class Tuple, bool Match = false>
		struct Find
		{
			static T& Get(Tuple& tp)
			{
				return Find<N + 1, T, Tuple, Checker<N + 1, T>::value>::Get(tp);
			}
		};

		template <int N, class T, class Tuple>
		struct Find<N, T, Tuple, true>
		{
			static T& Get(Tuple& tp)
			{
				return std::get<N>(tp);
			}
		};

		template <typename T>
		T& Get()
		{
			return Find<0, T, SlotType, Checker<0, T>::value>::Get(m_slots);
		}

		template <typename T>
		void Set(T& value)
		{
			Get<T>() = value;
		}

		template <typename T>
		void Set(T value)
		{
			Get<T>() = value;
		}
	};

	template <typename... Ts>
	class Slot3
	{
	public:
		typedef std::tuple<Ts...> Types;
		Types m_slots;

		template <int N, typename T>
		struct Checker : std::is_same<T, typename std::tuple_element<N, Types>::type>
		{
		};

		template <int N, class T, class Tuple, bool Match = false>
		struct Find
		{
			static T& Get(Tuple& tp)
			{
				return Find<N + 1, T, Tuple, Checker<N + 1, T>::value>::Get(tp);
			}
		};

		template <int N, class T, class Tuple>
		struct Find<N, T, Tuple, true>
		{
			static T& Get(Tuple& tp)
			{
				return std::get<N>(tp);
			}
		};

		template <typename T>
		T& Get()
		{
			return Find<0, T, Types, Checker<0, T>::value>::Get(m_slots);
		}

		template <typename T>
		void Set(T& value)
		{
			Get<T>() = value;
		}

		template <typename T>
		void Set(T value)
		{
			Get<T>() = value;
		}
#if 0
		template <typename T>
		void Set(const T& value)
		{
			Get<T>() = value;
		}
#endif
	};

	template <typename... Ts>
	class Slot4
	{
	protected:
		typedef std::tuple<std::function<void(Ts)>...> Types;
		Types m_slots;

		template <int N, typename T>
		struct Checker : std::is_same<T, typename std::tuple_element<N, Types>::type>
		{
		};

		template <int N, class T, class Tuple, bool Match = false>
		struct Find
		{
			static T& Get(Tuple& tp)
			{
				return Find<N + 1, T, Tuple, Checker<N + 1, T>::value>::Get(tp);
			}
		};

		template <int N, class T, class Tuple>
		struct Find<N, T, Tuple, true>
		{
			static T& Get(Tuple& tp)
			{
				return std::get<N>(tp);
			}
		};

		template <typename T>
		T& Get()
		{
			return Find<0, T, Types, Checker<0, T>::value>::Get(m_slots);
		}
	};

	class ErrorSlot : Slot4<const std::exception&, HRESULT>
	{
	public:
#if 0
		template <class T>
		void ErrorHandler(std::function<void(T)> func)
		{
			Get<std::function<void(T)>>() = std::move(func);
		}

		template <class T>
		void OnError(T value)
		{
			Get<std::function<void(T)>>()(value);
		}
#else
		void ErrorHandler(std::function<void(const std::exception&)> func)
		{
			Get<std::function<void(const std::exception&)>>() = func;
		}

		void ErrorHandler(std::function<void(HRESULT)> func)
		{
			Get<std::function<void(HRESULT)>>() = func;
		}

		void OnError(const std::exception& ex)
		{
			Get<std::function<void(const std::exception&)>>()(ex);
		}

		void OnError(HRESULT hr)
		{
			Get<std::function<void(HRESULT)>>()(hr);
		}
#endif
	};
}
