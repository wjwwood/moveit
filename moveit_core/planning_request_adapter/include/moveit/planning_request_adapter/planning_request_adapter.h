/*********************************************************************
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2012, Willow Garage, Inc.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of Willow Garage nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************/

/* Author: Ioan Sucan */

#ifndef MOVEIT_PLANNING_REQUEST_ADAPTER_PLANNING_REQUEST_ADAPTER_
#define MOVEIT_PLANNING_REQUEST_ADAPTER_PLANNING_REQUEST_ADAPTER_

#include <moveit/planning_interface/planning_interface.h>
#include <moveit/planning_scene/planning_scene.h>
#include <boost/function.hpp>

/** \brief Generic interface to adapting motion planning requests */
namespace planning_request_adapter
{

class PlanningRequestAdapter
{
public:

  typedef boost::function<bool(const planning_scene::PlanningSceneConstPtr& planning_scene,
                               const planning_interface::MotionPlanRequest &req, planning_interface::MotionPlanResponse &res)> PlannerFn;

  PlanningRequestAdapter()
  {
  }

  virtual ~PlanningRequestAdapter()
  {
  }

  /// Get a short string that identifies the planning request adapter
  virtual std::string getDescription() const { return ""; }

  bool adaptAndPlan(const planning_interface::PlannerManagerPtr &planner,
                    const planning_scene::PlanningSceneConstPtr& planning_scene,
                    const planning_interface::MotionPlanRequest &req,
                    planning_interface::MotionPlanResponse &res) const;

  bool adaptAndPlan(const planning_interface::PlannerManagerPtr &planner,
                    const planning_scene::PlanningSceneConstPtr& planning_scene,
                    const planning_interface::MotionPlanRequest &req,
                    planning_interface::MotionPlanResponse &res,
                    std::vector<std::size_t> &added_path_index) const;

  /** \brief Adapt the planning request if needed, call the planner
      function \e planner and update the planning response if
      needed. If the response is changed, the index values of the
      states added without planning are added to \e
      added_path_index */
  virtual bool adaptAndPlan(const PlannerFn &planner,
                            const planning_scene::PlanningSceneConstPtr& planning_scene,
                            const planning_interface::MotionPlanRequest &req,
                            planning_interface::MotionPlanResponse &res,
                            std::vector<std::size_t> &added_path_index) const = 0;

};

typedef boost::shared_ptr<PlanningRequestAdapter> PlanningRequestAdapterPtr;
typedef boost::shared_ptr<const PlanningRequestAdapter> PlanningRequestAdapterConstPtr;

/// Apply a sequence of adapters to a motion plan
class PlanningRequestAdapterChain
{
public:
  PlanningRequestAdapterChain()
  {
  }

  void addAdapter(const PlanningRequestAdapterConstPtr &adapter)
  {
    adapters_.push_back(adapter);
  }

  bool adaptAndPlan(const planning_interface::PlannerManagerPtr &planner,
                    const planning_scene::PlanningSceneConstPtr& planning_scene,
                    const planning_interface::MotionPlanRequest &req,
                    planning_interface::MotionPlanResponse &res) const;

  bool adaptAndPlan(const planning_interface::PlannerManagerPtr &planner,
                    const planning_scene::PlanningSceneConstPtr& planning_scene,
                    const planning_interface::MotionPlanRequest &req,
                    planning_interface::MotionPlanResponse &res,
                    std::vector<std::size_t> &added_path_index) const;

private:
  std::vector<PlanningRequestAdapterConstPtr> adapters_;
};


}

#endif
