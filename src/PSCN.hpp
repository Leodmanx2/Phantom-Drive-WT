#ifndef PSCN_H
#define PSCN_H

#include <cmath>
#include <cstdint>
#include <fstream>
#include <limits>
#include <vector>

namespace PSCN {

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

	struct Actor {
		std::string name;
		Vec3f       position;
		Vec3f       orientation; // Euler angles: Roll, pitch, yaw

		template <typename Archive>
		void serialize(Archive& archive) {
			archive(name, position, orientation);
		}
	};

	struct Camera {
		Vec3f position;
		Vec3f orientation; // Euler angles: Roll, pitch, yaw

		template <typename Archive>
		void serialize(Archive& archive) {
			archive(position, orientation);
		}
	};

	struct PointLight final {
		Vec3f   position;
		Vec3f   color;
		float32 intensity;
		float32 radius;

		template <typename Archive>
		void serialize(Archive& archive) {
			archive(position, color, intensity, radius);
		}
	};

	struct SpotLight final {
		Vec3f   position;
		Vec3f   direction;
		Vec3f   color;
		float32 intensity;
		float32 angle;
		float32 radius;

		template <typename Archive>
		void serialize(Archive& archive) {
			archive(position, direction, color, intensity, angle, radius);
		}
	};

	struct DirectionLight final {
		Vec3f   direction;
		Vec3f   color;
		float32 intensity;

		template <typename Archive>
		void serialize(Archive& archive) {
			archive(direction, color, intensity);
		}
	};

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
		std::vector<Camera>         cameras;
		std::vector<Actor>          actors;
		std::vector<PointLight>     pointLights;
		std::vector<SpotLight>      spotLights;
		std::vector<DirectionLight> directionLights;
		float32 ambience;

		Body()
		  : cameras(), actors(), pointLights(), spotLights(), directionLights(){};
		Body(const std::vector<Camera>&         cam,
		     const std::vector<Actor>&          act,
		     const std::vector<PointLight>&     point,
		     const std::vector<SpotLight>&      spot,
		     const std::vector<DirectionLight>& direc)
		  : cameras(cam)
		  , actors(act)
		  , pointLights(point)
		  , spotLights(spot)
		  , directionLights(direc) {}

		template <typename Archive>
		void serialize(Archive& archive) {
			archive(cameras, actors, pointLights, spotLights, directionLights);
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
}

#endif
