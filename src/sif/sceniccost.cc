#include "sif/dynamiccost.h"
#include "sif/pedestriancost.h"

namespace valhalla {
namespace sif {

class ScenicPedestrianCost : public PedestrianCost {
 public:
  ScenicPedestrianCost(const CostingOptions& options)
      : PedestrianCost(options) {}

  virtual ~ScenicPedestrianCost() {}

  // Override the EdgeCost method to include the scenic value in the cost calculation
  virtual Cost EdgeCost(const baldr::DirectedEdge* edge,
                        const baldr::GraphTile* tile,
                        const uint32_t seconds) const override {
    auto cost = PedestrianCost::EdgeCost(edge, tile, seconds);

    // Get the scenic value from the OSM data
    uint32_t scenic = 0;
    if (tile->GetTag(edge->edgeinfo_offset(), "scenic", scenic)) {
      // Use the scenic value to adjust the cost
      cost.cost *= (1.0f + scenic / 100.0f);
    }

    return cost;
  }
};

// Register the custom costing option
cost_ptr_t CreateScenicPedestrianCost(const CostingOptions& options) {
  return std::make_shared<ScenicPedestrianCost>(options);
}
cost_ptr_t CreateScenicPedestrianCost(const boost::property_tree::ptree& config) {
  return std::make_shared<ScenicPedestrianCost>(config);
}

}