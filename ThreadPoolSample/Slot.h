// --------------------------------------------------------------------------------------------------------------------
// <copyright file="Slot.h" company="Grass Valley K.K.">
//   Copyright (C) 2016 Grass Valley K.K. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------
#pragma once
#include <functional>
#include <vector>

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
}
