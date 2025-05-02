#include "Model.h"

#include <set>
#include <assert.h>

#include "DirectedGraph.h"

namespace pulmtln {

using namespace mfem;

std::vector<const Element*> getElementsWithAttribute(const Mesh& mesh, int attr)
{
	std::vector<const Element*> res;
	for (auto e{ 0 }; e < mesh.GetNBE(); ++e) {
		const auto elemPtr{ mesh.GetBdrElement(e) };
		if (elemPtr->GetAttribute() == attr) {
			res.push_back(elemPtr);
		}
	}
	return res;
}

std::multimap<int, const Element*> determineClosedLoops(const std::vector<const Element*>& elems)
{
	DirectedGraph g;
	std::map<int, const Element*> vToE;
	for (const auto& e : elems) {
		assert(e->GetType() == Element::Type::SEGMENT);
		int v0{ e->GetVertices()[0] };
		int v1{ e->GetVertices()[1] };
		g.addEdge(v0, v1);
		vToE[v0] = e;
	}

	std::multimap<int, const Element*> res;
	int cycleCount{ 0 };
	for (const auto& cycle : g.findCycles()) {
		for (const auto& v : cycle) {
			res.emplace(cycleCount, vToE.at(v));
		}
		cycleCount++;
	}

	return res;
}

Materials filterOutMaterialsNotPresentInMesh(
	const Materials& materials, 
	const Mesh& mesh)
{
	NameToAttrMap matInMesh;
	NameToAttrMap allMats{ materials.buildNameToAttrMap() };
	for (const auto& [name, attr] : allMats) {
		for (auto e{ 0 }; e < mesh.GetNBE(); e++) {
			if (mesh.GetBdrElement(e)->GetAttribute() == attr) {
				matInMesh.emplace(name, attr);
				break;
			}
		}
	}
	for (const auto& [name, attr] : allMats) {
		for (auto e{ 0 }; e < mesh.GetNE(); e++) {
			if (mesh.GetElement(e)->GetAttribute() == attr) {
				matInMesh.emplace(name, attr);
				break;
			}
		}
	}

	Materials res{ materials };
	res.removeMaterialsNotInList(matInMesh);
	return res;
}

Model::Model(
	Mesh& mesh,  
	const Materials& materials) :
	mesh_{ std::make_unique<mfem::Mesh>(std::move(mesh)) }
{
	materials_ = filterOutMaterialsNotPresentInMesh(materials, *mesh_);
}

bool elementsFormOpenLoops(const std::vector<const Element*>& elems)
{
	return determineClosedLoops(elems).size() != elems.size();
}

std::size_t Model::numberOfConductors() const
{
	return materials_.buildNameToAttrMapFor<PEC>().size();
}

Model::OpennessType Model::determineOpenness() const
{
	assert(materials_.openBoundaries.size() <= 1);

	if (materials_.openBoundaries.size() == 0) {
		return OpennessType::closed;
	}

	for (const auto& m : materials_.openBoundaries) {
		if (!elementsFormOpenLoops(getElementsWithAttribute(*mesh_, m.attribute))) {
			return OpennessType::open;
		}
	}
	return OpennessType::semiopen;
}


}