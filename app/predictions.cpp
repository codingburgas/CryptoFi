#include "predictions.h"

namespace prediction {

    std::vector<double> dateToFeatures(const std::chrono::system_clock::time_point& date) {
        auto epoch_time = std::chrono::system_clock::to_time_t(date);
        struct tm* time_info = localtime(&epoch_time);
        return {static_cast<double>(time_info->tm_year + 1900),
                static_cast<double>(time_info->tm_mon + 1),
                static_cast<double>(time_info->tm_mday)};
    }

    double encodeType(const std::string& type) {
        return type == "expense" ? 1.0 : 0.0;
    }

    std::vector<std::vector<double>> TransactionPredictor::prepareFeatureMatrix(const std::vector<mainScreen::Transaction*>& transactions) {
        std::vector<std::vector<double>> features;

        for (const auto* transaction : transactions) {
            std::vector<double> feature = dateToFeatures(transaction->date);
            feature.push_back(encodeType(transaction->type));
            features.push_back(feature);
        }
        return features;
    }

    std::vector<double> TransactionPredictor::prepareTargetVector(const std::vector<mainScreen::Transaction*>& transactions) {
        std::vector<double> targets;
        for (const auto* transaction : transactions) {
            targets.push_back(transaction->difference);
        }
        return targets;
    }

    void TransactionPredictor::setupAndTrainModel(const std::vector<std::vector<double>>& features, const std::vector<double>& targets) {

    opennn::Tensor<opennn::type, 2> features_tensor(features.size(), features[0].size());
    for (size_t i = 0; i < features.size(); i++) {
        for (size_t j = 0; j < features[i].size(); j++) {
            features_tensor(i, j) = features[i][j];
        }
    }

    opennn::Tensor<opennn::type, 1> targets_tensor(targets.size());
    for (size_t i = 0; i < targets.size(); i++) {
        targets_tensor(i) = targets[i];
    }

    opennn::DataSet dataset;
    dataset.set_data(features_tensor);
    dataset.set_target();

    opennn::Tensor<opennn::Index, 1> architecture(3);
    architecture(0) = features[0].size();
    architecture(1) = 10;
    architecture(2) = 1;

    try {
        neural_network.set(opennn::NeuralNetwork::ProjectType::Forecasting, architecture);
    } catch (const std::exception& e) {
        return;
    }

    auto* layer_ptr = neural_network.get_layer_pointer(0);
    if (layer_ptr == nullptr) {
        return;
    }

    layer_ptr->set_inputs_number(features[0].size());

    opennn::PerceptronLayer* hidden_layer = new opennn::PerceptronLayer();
    hidden_layer->set_neurons_number(10);
    hidden_layer->set_activation_function(opennn::PerceptronLayer::ActivationFunction::Logistic);

    opennn::PerceptronLayer* output_layer = new opennn::PerceptronLayer();
    output_layer->set_neurons_number(1);

    try {
        neural_network.add_layer(hidden_layer);
        neural_network.add_layer(output_layer);
    } catch (const std::exception& e) {
        delete hidden_layer;
        delete output_layer;
        return;
    }

    opennn::TrainingStrategy training_strategy(&neural_network, &dataset);
    training_strategy.perform_training();

    delete hidden_layer;
    delete output_layer;

}

void TransactionPredictor::analyzeTransactions(const std::vector<mainScreen::Transaction*>& transactions) {
    auto features = prepareFeatureMatrix(transactions);
    auto targets = prepareTargetVector(transactions);
    setupAndTrainModel(features, targets);
}

    std::string TransactionPredictor::predictNextTransaction(const std::vector<mainScreen::Transaction*>& transactions) {
        std::vector<std::vector<double>> features = prepareFeatureMatrix(transactions);
        std::vector<double> targets = prepareTargetVector(transactions);

        setupAndTrainModel(features, targets);

        std::vector<double> input_vector = features.back();
        opennn::Tensor<opennn::type, 2> input_tensor(1, input_vector.size());
        for (size_t i = 0; i < input_vector.size(); i++) {
            input_tensor(0, i) = input_vector[i];
        }

        opennn::Tensor<opennn::type, 2> output = neural_network.calculate_outputs(input_tensor);

        double predicted_difference = output(0, 0);
        std::string prediction_type = predicted_difference < 0 ? "expense" : "revenue";

        return "Predicted next transaction: Type = " + prediction_type + ", Amount = " + std::to_string(std::abs(predicted_difference));
    }
}