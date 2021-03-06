/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2014 Boone "Bea" Adkins
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
/**
 * @file
 *
 * Describes a single feature, which a state represents an arrangement.
 *
 * @date Aug 22, 2013
 * @author Bea Adkins
 */

#ifndef FEATURE_H
#define FEATURE_H

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include "pretty.h"
// Written for 0.2.7-5 API
#include <yaml-cpp/yaml.h>


using boost::shared_ptr;

namespace pomdp
{

/**
 * Describes an MDP sub-state.
 */
// @todo Refactor into DiscreteFeature and ContinuousFeature.
struct Feature
{
  unsigned int num_states_; /**< Number of possible state values in this feature. */
  std::string name_; /**< Name of this feature. */
  shared_ptr<const Pretty> state_name_mapping_; /**< Maps name:number for states in this feature. */

  /** Empty. */
  Feature() : name_(""), num_states_(0), state_name_mapping_(Pretty::empty){};

  /** Without named states. */
  Feature(const std::string& name, unsigned int num_states)
    : name_(name), num_states_(num_states), state_name_mapping_(Pretty::empty){}

  /** With named states. */
//  Feature(const std::string& name, const std::vector<std::string>& state_names)
//    : name_(name), num_states_(state_names.size()), state_name_mapping_(new Pretty(state_names)){}
  /** With named states. */
  Feature(const std::string& name, shared_ptr<const Pretty> state_names) : name_(name)
  {
    // If mapping exists, use it.
    if(state_names)
    {
      num_states_ = state_names->size();
      state_name_mapping_ = state_names; //shared_ptr<const Pretty>(state_names);
    }
    // Doesn't exist, use empty Feature.
    else
    {
      num_states_ = 0;
      state_name_mapping_ = Pretty::empty;
    }
  }

  bool valid() const
  {
    if(num_states_ < 2) // A state with 0 values makes no sense, with 1 value isn't very informative!
      return false;

    if(state_name_mapping_->size() > num_states_) // Less is okay - some states don't have names.
      return false;

    return true;
  }

  friend std::ostream& operator<<(std::ostream& os, const Feature& f)
  {
    if(!f.valid())
      os << "Invalid ";
    os << "Feature \"" << f.name_ << "\", size: " << f.num_states_ << ", name mapping: ";

    if(f.state_name_mapping_)
      os << *f.state_name_mapping_;
    else
      os << "none";

    return os;
  }

  friend bool operator==(const Feature& a, const Feature& b)
  {
    return (a.name_ == b.name_ && a.num_states_ == b.num_states_) // Fields match
      && (// And...
          // Either mapping pointers match (both empty is ok)
          (a.state_name_mapping_ == b.state_name_mapping_)
          // Or pointed mappings match
          || (a.state_name_mapping_ && b.state_name_mapping_
              && *a.state_name_mapping_ == *b.state_name_mapping_)
         );
  }

  friend void operator >>(const YAML::Node& node, Feature& f);
};

} /* namespace pomdp */

#endif /* FEATURE_H */
