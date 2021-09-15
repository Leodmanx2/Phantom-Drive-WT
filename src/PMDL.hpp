#ifndef PD_PMDL_HPP
#define PD_PMDL_HPP

#include <cmath>
#include <cstdint>
#include <fstream>
#include <limits>
#include <vector>

namespace PMDL {

	// -----------------------------------------------------------------------------
	//  Basic types
	// -----------------------------------------------------------------------------

	using uint8 = std::uint8_t;

	using uint32 = std::uint32_t;
	using int32  = std::int32_t;

	using float32 = float;

	// Using basic floating point types relies on them having fixed precision
	// (32, 64 bits) and following a single standard (IEE 754).
	// Cereal also does this check internally, but we'll do them anyway.
	static_assert(std::numeric_limits<float>::is_iec559,
	              "Float type is not IEEE 754");

	// -----------------------------------------------------------------------------
	//  Vectors
	// -----------------------------------------------------------------------------

	struct Vec2f {
		float32 u;
		float32 v;

		template <typename Archive>
		void serialize(Archive& archive) {
			archive(u, v);
		}
	};

	struct Vec3f {
		float32 x;
		float32 y;
		float32 z;

		template <typename Archive>
		void serialize(Archive& archive) {
			archive(x, y, z);
		}
	};

	// -----------------------------------------------------------------------------
	//  Model types
	// -----------------------------------------------------------------------------

	struct Vertex {
		Vec3f position;
		Vec3f normal;
		Vec2f texCoord;

		Vertex() : position(), normal(), texCoord(){};
		Vertex(Vec3f pos, Vec3f norm, Vec2f uv)
		  : position(pos), normal(norm), texCoord(uv) {}

		template <typename Archive>
		void serialize(Archive& archive) {
			archive(position, normal, texCoord);
		}
	};

	using Index = uint32;

	// -----------------------------------------------------------------------------
	//  File structures
	// -----------------------------------------------------------------------------

	struct Header {
		uint32 signature;
		uint32 version;

		Header() : signature(), version(){};
		Header(const uint32 sig, const uint8 vers)
		  : signature(sig), version(vers) {}

		template <typename Archive>
		void serialize(Archive& archive) {
			archive(signature, version);
		}
	};

	struct Body {
		std::vector<Vertex> vertices;
		std::vector<Index>  indices;

		Body() : vertices(), indices(){};
		Body(const std::vector<Vertex>& vert, const std::vector<Index>& ind)
		  : vertices(vert), indices(ind) {}

		template <typename Archive>
		void serialize(Archive& archive) {
			archive(vertices, indices);
		}
	};

	struct File {
		Header header;
		Body   body;

		File() : header(), body(){};
		File(const Header& head, const Body& bod) : header(head), body(bod) {}

		void write(const std::string& filename);

		static File parse(std::istream& fileContents);

		template <typename Archive>
		void serialize(Archive& archive) {
			archive(header, body);
		}
	};
} // namespace PMDL

#endif
