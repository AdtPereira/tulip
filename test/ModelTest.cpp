#include <gtest/gtest.h>

#include "Model.h"
#include "Parser.h"
#include "TestUtils.h"

using namespace pulmtln;

class ModelTest : public ::testing::Test {};

TEST_F(ModelTest, empty_coax_is_closed)
{
	EXPECT_EQ(
		Parser{ inputCase("empty_coax") }.readModel().determineOpenness(),
		Model::OpennessType::closed
	);
}

TEST_F(ModelTest, agrawal1981_is_semiopen)
{
	EXPECT_EQ(
		Parser{ inputCase("agrawal1981") }.readModel().determineOpenness(),
		Model::OpennessType::semiopen
	);
}

TEST_F(ModelTest, three_wires_ribbon_is_open)
{
	EXPECT_EQ(
		Parser{ inputCase("three_wires_ribbon") }.readModel().determineOpenness(),
		Model::OpennessType::open
	);
}

TEST_F(ModelTest, agrawal1981_conductors_in_mesh)
{
	auto m{ Parser{ inputCase("agrawal1981") }.readModel() };
	
	ASSERT_EQ(4, m.getMaterials().pecs.size());
}