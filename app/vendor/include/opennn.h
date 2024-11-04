//   OpenNN: Open Neural Networks Library
//   www.opennn.net
//
//   O P E N   N E U R A L   N E T W O R K S   L I B R A R Y
//
//   Artificial Intelligence Techniques SL
//   artelnics@artelnics.com

#ifndef OPENNN_H
#define OPENNN_H

#include "opennn/config.h"
#include "opennn/half.hpp"

// Data set

#include "opennn/data_set.h"

// Neural network

#include "opennn/config.h"
#include "opennn/layer.h"
#include "opennn/pooling_layer.h"
#include "opennn/convolutional_layer.h"
#include "opennn/bounding_layer.h"
#include "opennn/perceptron_layer.h"
#include "opennn/long_short_term_memory_layer.h"
#include "opennn/recurrent_layer.h"
#include "opennn/probabilistic_layer.h"
#include "opennn/scaling_layer.h"
#include "opennn/region_proposal_layer.h"
#include "opennn/non_max_suppression_layer.h"
#include "opennn/unscaling_layer.h"
#include "opennn/flatten_layer.h"
#include "opennn/neural_network.h"

// Training strategy

#include "opennn/loss_index.h"

#include "opennn/cross_entropy_error.h"
#include "opennn/mean_squared_error.h"
#include "opennn/minkowski_error.h"
#include "opennn/normalized_squared_error.h"
#include "opennn/sum_squared_error.h"
#include "opennn/weighted_squared_error.h"

#include "opennn/conjugate_gradient.h"
#include "opennn/gradient_descent.h"
#include "opennn/levenberg_marquardt_algorithm.h"
#include "opennn/quasi_newton_method.h"
#include "opennn/optimization_algorithm.h"
#include "opennn/learning_rate_algorithm.h"

// Model selection

#include "opennn/model_selection.h"
#include "opennn/neurons_selection.h"
#include "opennn/growing_neurons.h"
#include "opennn/inputs_selection.h"
#include "opennn/growing_inputs.h"
#include "opennn/genetic_algorithm.h"

// Testing analysis

#include "opennn/testing_analysis.h"

// Utilities

#include "opennn/numerical_differentiation.h"
#include "opennn/correlations.h"
#include "opennn/response_optimization.h"
#include "opennn/opennn_strings.h"
#include "opennn/opennn_images.h"
#include "opennn/tensor_utilities.h"
#include "opennn/statistics.h"
#include "opennn/scaling.h"
#include "opennn/region_based_object_detector.h"
#include "opennn/json_to_xml.h"
#include "opennn/text_analytics.h"
#include "opennn/codification.h"

#endif

// OpenNN: Open Neural Networks Library.
// Copyright(C) 2005-2022 Artificial Intelligence Techniques, SL.
//
// This library is free software; you can redistribute it and/or
// modify it under the s of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
