#include "predictions.h"

namespace prediction {

    // Converts a date to a vector containing year, month, and day.
    std::vector<double> dateToFeatures(const std::chrono::system_clock::time_point& date) {
        auto epoch_time = std::chrono::system_clock::to_time_t(date);
        struct tm* time_info = localtime(&epoch_time);
        return {static_cast<double>(time_info->tm_year + 1900),
                static_cast<double>(time_info->tm_mon + 1),
                static_cast<double>(time_info->tm_mday)};
    }

    // Encodes the transaction type as a double (1.0 for "expense", 0.0 for revenue).
    double encodeType(const std::string& type) {
        return type == "expense" ? 1.0 : 0.0;
    }

    // Prepares the feature matrix from a list of transactions.
    std::vector<std::vector<double>> TransactionPredictor::prepareFeatureMatrix(const std::vector<mainScreen::Transaction*>& transactions) {
        std::vector<std::vector<double>> features;

        for (const auto* transaction : transactions) {
            std::vector<double> feature = dateToFeatures(transaction->date);
            feature.push_back(encodeType(transaction->type));
            features.push_back(feature);
        }
        return features;
    }

    // Prepares the target vector from a list of transactions.
    std::vector<double> TransactionPredictor::prepareTargetVector(const std::vector<mainScreen::Transaction*>& transactions) {
        std::vector<double> targets;
        for (const auto* transaction : transactions) {
            targets.push_back(transaction->difference);
        }
        return targets;
    }

    // Sets up and trains the neural network model using the provided features and targets.
    void TransactionPredictor::setupAndTrainModel(const std::vector<std::vector<double>>& features, const std::vector<double>& targets) {

        // Convert features and targets to OpenNN tensors.
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

        // Create and configure the dataset.
        opennn::DataSet dataset;
        dataset.set_data(features_tensor);
        dataset.set_target();

        // Define the architecture of the neural network.
        opennn::Tensor<opennn::Index, 1> architecture(3);
        architecture(0) = features[0].size();
        architecture(1) = 10;
        architecture(2) = 1;

        try {
            neural_network.set(opennn::NeuralNetwork::ProjectType::Forecasting, architecture);
        } catch (const std::exception& e) {
            return;
        }

        // Set the number of inputs for the first layer.
        auto* layer_ptr = neural_network.get_layer_pointer(0);
        if (layer_ptr == nullptr) {
            return;
        }

        layer_ptr->set_inputs_number(features[0].size());

        // Create and configure the hidden and output layers.
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

        // Train the neural network.
        opennn::TrainingStrategy training_strategy(&neural_network, &dataset);
        training_strategy.perform_training();

        // Clean up.
        delete hidden_layer;
        delete output_layer;
    }

    // Analyzes the transactions by preparing the features and targets and training the model.
    void TransactionPredictor::analyzeTransactions(const std::vector<mainScreen::Transaction*>& transactions) {
        auto features = prepareFeatureMatrix(transactions);
        auto targets = prepareTargetVector(transactions);
        setupAndTrainModel(features, targets);
    }

    // Predicts the next transaction based on the provided transactions.
    std::string TransactionPredictor::predictNextTransaction(const std::vector<mainScreen::Transaction*>& transactions) {
        std::vector<std::vector<double>> features = prepareFeatureMatrix(transactions);
        std::vector<double> targets = prepareTargetVector(transactions);

        setupAndTrainModel(features, targets);

        // Prepare the input tensor for prediction.
        std::vector<double> input_vector = features.back();
        opennn::Tensor<opennn::type, 2> input_tensor(1, input_vector.size());
        for (size_t i = 0; i < input_vector.size(); i++) {
            input_tensor(0, i) = input_vector[i];
        }

        // Calculate the output using the neural network.
        opennn::Tensor<opennn::type, 2> output = neural_network.calculate_outputs(input_tensor);

        // Determine the predicted transaction type and amount.
        double predicted_difference = output(0, 0);
        std::string prediction_type = predicted_difference < 0 ? "expense" : "revenue";

        return "Predicted next transaction: Type = " + prediction_type + ", Amount = " + std::to_string(std::abs(predicted_difference));
    }
}