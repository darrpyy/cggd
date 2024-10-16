#pragma once

#include "utils/error_handler.h"

#include <algorithm>
#include <linalg.h>
#include <vector>


using namespace linalg::aliases;

namespace cg
{
	template<typename T>
	class resource
	{
	public:
		resource(size_t size);
		resource(size_t x_size, size_t y_size);
		~resource();

		const T* get_data();
		T& item(size_t item);
		T& item(size_t x, size_t y);

		size_t size_bytes() const;
		size_t count() const;
		size_t get_stride() const;

	private:
		std::vector<T> data;
		size_t item_size = sizeof(T);
		size_t stride;
	};

	template<typename T>
	inline resource<T>::resource(size_t size)
	{
		data.resize(size);
		stride = 0;
	}
	template<typename T>
	inline resource<T>::resource(size_t x_size, size_t y_size)
	{
		data.resize(y_size * x_size);
		stride = x_size;
	}
	template<typename T>
	inline resource<T>::~resource()
	{
	}
	template<typename T>
	inline const T* resource<T>::get_data()
	{
		return data.data();
	}
	template<typename T>
	inline T& resource<T>::item(size_t item)
	{
		return data.at(item);
	}
	template<typename T>
	inline T& resource<T>::item(size_t x, size_t y)
	{
		return data.at(y * stride + x);
	}
	template<typename T>
	inline size_t resource<T>::size_bytes() const
	{
		return item_size * data.size();
	}
	template<typename T>
	inline size_t resource<T>::count() const
	{
		return data.size();
	}

	template<typename T>
	inline size_t resource<T>::get_stride() const
	{
		return stride;
	}

	struct color
	{
		static color from_float3(const float3& in)
		{
			color clr{};
			clr.r = in.x;
			clr.g = in.y;
			clr.b = in.z;
			return clr;
		};
		float3 to_float3() const
		{
			return float3(r, g, b);
		}
		float r;
		float g;
		float b;
	};

	struct unsigned_color
	{
		static unsigned_color from_color(const color& color)
		{
			unsigned_color clr{};
			clr.r = std::clamp(static_cast<int>(color.r * 255.0f), 0, 255);
			clr.g = std::clamp(static_cast<int>(color.g * 255.0f), 0, 255);
			clr.b = std::clamp(static_cast<int>(color.b * 255.0f), 0, 255);
			return clr;
		};

		static unsigned_color from_float3(const float3& color)
		{
			float3 preprocessed = clamp(255.0f * color, 0.0f, 255.0f);
			unsigned_color clr{};
			clr.r = static_cast<uint8_t>(preprocessed.x);
			clr.g = static_cast<uint8_t>(preprocessed.y);
			clr.b = static_cast<uint8_t>(preprocessed.z);
			return clr;
		};

		float3 to_float3() const
		{
			return float3( static_cast<float>(r), static_cast<float>(g), static_cast<float>(b)) / 255.0f;
		};
		uint8_t r;
		uint8_t g;
		uint8_t b;
	};


	struct vertex
	{
		float x, y, z;
		float nx, ny, nz;
		float u, v;
		float ambient_r, ambient_g, ambient_b;
		float diffuse_r, diffuse_g, diffuse_b;
		float emissive_r, emissive_g, emissive_b;
	};

}// namespace cg