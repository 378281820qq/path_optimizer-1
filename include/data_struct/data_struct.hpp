//
// Created by ljn on 20-1-28.
//

#ifndef PATH_OPTIMIZER_INCLUDE_DATA_STRUCT_DATA_STRUCT_HPP_
#define PATH_OPTIMIZER_INCLUDE_DATA_STRUCT_DATA_STRUCT_HPP_
#include <vector>
#include <tools/spline.h>
#include <opt_utils/opt_utils.hpp>

namespace PathOptimizationNS {

struct ReferencePath {
    ReferencePath() = default;
    // Copy some elements from another one.
    ReferencePath(const ReferencePath &divided_segments_, size_t target_index) :
        x_s_(divided_segments_.x_s_),
        y_s_(divided_segments_.y_s_),
        max_s_(divided_segments_.max_s_) {
        assert(target_index <= divided_segments_.seg_angle_list_.size());
        seg_s_list_.assign(divided_segments_.seg_s_list_.begin(), divided_segments_.seg_s_list_.begin() + target_index);
        seg_angle_list_.assign(divided_segments_.seg_angle_list_.begin(),
                               divided_segments_.seg_angle_list_.begin() + target_index);
        seg_k_list_.assign(divided_segments_.seg_k_list_.begin(), divided_segments_.seg_k_list_.begin() + target_index);
        seg_clearance_list_.assign(divided_segments_.seg_clearance_list_.begin(),
                                   divided_segments_.seg_clearance_list_.begin() + target_index);
        seg_x_list_.assign(divided_segments_.seg_x_list_.begin(), divided_segments_.seg_x_list_.begin() + target_index);
        seg_y_list_.assign(divided_segments_.seg_y_list_.begin(), divided_segments_.seg_y_list_.begin() + target_index);
    }
    // Reference path representation.
    tk::spline x_s_;
    tk::spline y_s_;
    double max_s_ = 0;
    // Divided smoothed path info.
    std::vector<double> seg_s_list_;
    std::vector<double> seg_k_list_;
    std::vector<double> seg_x_list_;
    std::vector<double> seg_y_list_;
    std::vector<double> seg_angle_list_;
    std::vector<std::vector<double> > seg_clearance_list_;
};

struct VehicleState {
    VehicleState() = default;
    VehicleState(const hmpl::State &start_state,
                 const hmpl::State &end_state,
                 double offset = 0,
                 double heading_error = 0) :
        start_state_(start_state),
        end_state_(end_state),
        initial_offset_(offset),
        initial_heading_error_(heading_error) {}
    // Initial state.
    hmpl::State start_state_;
    // Target state.
    hmpl::State end_state_;
    // Initial error with reference line.
    double initial_offset_ = 0;
    double initial_heading_error_ = 0;
};

// Standard point struct.
struct State {
    State() = default;
    State(double x, double y, double z = 0, double k = 0) :
        x(x),
        y(y),
        z(z),
        k(k) {}
    double x = 0;
    double y = 0;
    double z = 0; // Heading.
    double k = 0; // Curvature.
};

// Point for A* search.
struct APoint {
    double x = 0;
    double y = 0;
    double s = 0;
    double l = 0;
    double g = 0;
    double h = 0;
    // Layer denotes the index of the longitudinal layer that the point lies on.
    int layer = -1;
    double offset = 0;
    bool is_in_open_set = false;
    APoint *parent = nullptr;
    inline double f() {
        return g + h;
    }
};

class PointComparator {
public:
    bool operator()(APoint *a, APoint *b) {
        return a->f() > b->f();
    }
};

}
#endif //PATH_OPTIMIZER_INCLUDE_DATA_STRUCT_DATA_STRUCT_HPP_
