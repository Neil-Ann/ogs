/**
 * \copyright
 * Copyright (c) 2012-2019, OpenGeoSys Community (http://www.opengeosys.org)
 *            Distributed under a Modified BSD License.
 *              See accompanying file LICENSE.txt or
 *              http://www.opengeosys.org/project/license
 *
 */

#pragma once

#include <utility>

#include "BaseLib/Error.h"
#include "MeshLib/PropertyVector.h"

#include "Parameter.h"


namespace MeshLib
{
template <typename T>
class PropertyVector;
}  // MeshLib

namespace ProcessLib
{

/// A parameter class looking for values from indices in a property vector.
/// This class can be used for material ID dependent parameters.
template <typename T, MeshLib::MeshItemType MeshItemType>
struct GroupBasedParameter final
    : public Parameter<T>
{
    /**
     * Constructing from a property vector of index and corresponding values
     *
     * @param name_       the parameter's name
     * @param mesh        the parameter's domain of definition.
     * @param property    a property vector of index for mesh items
     * @param vec_values  a vector of values for each index
     */
    GroupBasedParameter(std::string const& name_,
                        MeshLib::Mesh const& mesh,
                        MeshLib::PropertyVector<int> const& property,
                        std::vector<std::vector<double>>
                            vec_values)
        : Parameter<T>(name_, &mesh),
          _property_index(property),
          _vec_values(std::move(vec_values))
    {
    }

    bool isTimeDependent() const override { return false; }

    int getNumberOfComponents() const override
    {
        return _vec_values.empty()
                   ? 0
                   : static_cast<int>(_vec_values.front().size());
    }

    std::vector<T> operator()(double const /*t*/,
                                     SpatialPosition const& pos) const override
    {
        auto const item_id = getMeshItemID(pos, type<MeshItemType>());
        assert(item_id);
        int const index = _property_index[item_id.get()];
        auto const& values = _vec_values[index];
        if (values.empty())
        {
            OGS_FATAL("No data found for the group index %d", index);
        }
         return values;
    }

private:
    template <MeshLib::MeshItemType ITEM_TYPE> struct type {};

    static boost::optional<std::size_t>
    getMeshItemID(SpatialPosition const& pos, type<MeshLib::MeshItemType::Cell>)
    {
        return pos.getElementID();
    }

    static boost::optional<std::size_t>
    getMeshItemID(SpatialPosition const& pos, type<MeshLib::MeshItemType::Node>)
    {
        return pos.getNodeID();
    }

    MeshLib::PropertyVector<int> const& _property_index;
    std::vector<std::vector<T>> const _vec_values;
};

std::unique_ptr<ParameterBase> createGroupBasedParameter(
    std::string const& name,
    BaseLib::ConfigTree const& config,
    MeshLib::Mesh const& mesh);

}  // namespace ProcessLib
