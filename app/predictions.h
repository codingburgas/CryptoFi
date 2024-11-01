#pragma once
#include "pch.h"
#include "mainScreen.h"
namespace prediction {
    class TransactionPredictor {
    public:
        // Function to analyze and train model on transactions
        void analyzeTransactions(const std::vector<mainScreen::Transaction*>& transactions);

        // Function to make a prediction based on a sample transaction
        std::string predictNextTransaction(const std::vector<mainScreen::Transaction*>& transactions);


    private:
        // Helper functions for data preparation and training
        std::vector<std::vector<double>> prepareFeatureMatrix(const std::vector<mainScreen::Transaction*>& transactions);
        std::vector<double> prepareTargetVector(const std::vector<mainScreen::Transaction*>& transactions);
        void setupAndTrainModel(const std::vector<std::vector<double>>& features, const std::vector<double>& targets);

        // OpenNN neural network
        opennn::NeuralNetwork neural_network;

    };
}

