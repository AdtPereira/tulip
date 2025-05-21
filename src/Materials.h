#pragma once

#include "AttrToValueMap.h"

namespace pulmtln {

using MaterialId = int;
using Attribute = int;

struct Material {
	std::string name;
	Attribute attribute;
};

struct PEC : public Material {
	double area{ 0.0 };
};

struct OpenBoundary : public Material {
};

struct Dielectric : public Material {
	double relativePermittivity{ 1.0 };
};

struct Materials { 
	std::vector<PEC> pecs;
	std::vector<OpenBoundary> openBoundaries;
	std::vector<Dielectric> dielectrics;

	template <class T>
	NameToAttrMap buildNameToAttrMapFor() const
	{
		NameToAttrMap res;
		if constexpr (std::is_same<T, PEC>()) {
			for (const auto& m : pecs) {
				res[m.name] = m.attribute;
			}
		}
		if constexpr (std::is_same<T, OpenBoundary>()) {
			for (const auto& m : openBoundaries) {
				res[m.name] = m.attribute;
			}
		}
		else if constexpr (std::is_same<T, Dielectric>()) {
			for (const auto& m : dielectrics) {
				res[m.name] = m.attribute;
			}
		}
		return res;
	}

	template <class T>
	const T& get(const std::string name) const
	{
		if constexpr (std::is_same<T, PEC>()) {
			for (const auto& m : pecs) {
				if (m.name == name) {
					return m;
				}
			}
		}
		if constexpr (std::is_same<T, OpenBoundary>()) {
			for (const auto& m : openBoundaries) {
				if (m.name == name) {
					return m;
				}
			}
		}
		else if constexpr (std::is_same<T, Dielectric>()) {
			for (const auto& m : dielectrics) {
				if (m.name == name) {
					return m;
				}
			}
		}
	}

	NameToAttrMap buildNameToAttrMap() const;
	void removeMaterialsNotInList(const NameToAttrMap allowedMaterials);
	bool isDomainMaterial(const std::string& name) const;
	
	static MaterialId getMaterialIdFromName(const std::string& name);	
};

}