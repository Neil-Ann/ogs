/**
 * \copyright
 * Copyright (c) 2013, OpenGeoSys Community (http://www.opengeosys.org)
 *            Distributed under a Modified BSD License.
 *              See accompanying file LICENSE.txt or
 *              http://www.opengeosys.org/LICENSE.txt
 */

#include <ctime>
#include "gtest/gtest.h"

#include "GeoLib/PointVec.h"

typedef std::vector<GeoLib::Point*> VectorOfPoints;

// Testing nullptr input vector.
TEST(GeoLib, TestPointVecCtorNullptr)
{
	ASSERT_ANY_THROW(GeoLib::PointVec("JustAName", nullptr));
}

// Testing empty input vector.
TEST(GeoLib, TestPointVecCtorEmpty)
{
	VectorOfPoints* ps_ptr = new VectorOfPoints;
	ASSERT_ANY_THROW(GeoLib::PointVec("JustAName", ps_ptr));
}

// Testing input vector with single point.
TEST(GeoLib, TestPointVecCtorSinglePoint)
{
	VectorOfPoints* ps_ptr = new VectorOfPoints;
	ps_ptr->push_back(new GeoLib::Point(0,0,0));
	GeoLib::PointVec* point_vec = nullptr;
	ASSERT_NO_THROW(point_vec = new GeoLib::PointVec("JustAName", ps_ptr));
	ASSERT_EQ(1, point_vec->size());

	delete point_vec;
}

// Testing input vector with two different points.
TEST(GeoLib, TestPointVecCtorTwoPoints)
{
	VectorOfPoints* ps_ptr = new VectorOfPoints;
	ps_ptr->push_back(new GeoLib::Point(0,0,0));
	ps_ptr->push_back(new GeoLib::Point(1,0,0));

	GeoLib::PointVec* point_vec = nullptr;
	ASSERT_NO_THROW(point_vec = new GeoLib::PointVec("JustAName", ps_ptr));
	ASSERT_EQ(2, point_vec->size());

	delete point_vec;
}
