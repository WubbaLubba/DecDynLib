#pragma once

#include <tuple>

namespace ddl
{
	template <typename T>
	struct export_id : T
	{};

	template <typename TA, typename TB >
	constexpr bool operator== (export_id<TA>, export_id<TB>)
	{
		return std::is_same_v<TA, TB>;
	}

	template <typename TA, typename TB >
	constexpr bool operator!= (export_id<TA> a, export_id<TB> b)
	{
		return !(a == b);
	}

	template <typename Id, typename Export>
	struct export_info_t {};

	template <typename T>
	struct type_t 
	{
		using type = T;
	};

	template <typename TA, typename TB>
	constexpr bool operator== (type_t<TA>, type_t<TB>)
	{
		return std::is_same_v<TA, TB>;
	}
	template <typename TA, typename TB>
	constexpr bool operator!= (type_t<TA> a, type_t<TB> b)
	{
		return !(a==b);
	}

	struct export_not_found {};

	template <typename... Exports>
	struct exports_map;

	template <>
	struct exports_map<>
	{

		template <typename ExportId>
		constexpr static auto find_export(ExportId)
		{
			return type_t<export_not_found>{};
		}
	};

	template <typename Id, typename TExport, typename... Ids, typename... TExports>
	struct exports_map<export_info_t<Id, TExport>, export_info_t<Ids, TExports>...>
	{
		template <typename ExportId>
		constexpr static auto find_export(ExportId eid)
		{
			if constexpr(eid == Id{}) return type_t<TExport>{};
			else return exports_map<export_info_t<Ids, TExports>...>::find_export(eid);
		}
	};

	template <typename Export, typename Id>
	constexpr export_info_t<Id, Export> export_info(Id)
	{
		static_assert(!std::is_reference_v<Export>, "Library cant export an reference type");
		return {};
	}

	template <typename... Ids, typename... TExports>
	constexpr exports_map<export_info_t<Ids, TExports>...> make_exports_map(export_info_t<Ids, TExports>...)
	{
		return {};
	}
}