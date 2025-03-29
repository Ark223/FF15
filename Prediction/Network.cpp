
#include "Network.h"

namespace IPrediction
{
    Network* Network::m_instance = nullptr;

    void Network::Destroy()
    {
        delete m_instance;
        m_instance = nullptr;
    }

    Network* Network::Get()
    {
        if (m_instance == nullptr)
        {
            m_instance = new Network();
        }
        return m_instance;
    }

    // Activation functions

    double Linear::Calculate(double x) const
    {
        return x;
    }

    double LeakyReLU::Calculate(double x) const
    {
        return x < 0 ? this->m_alpha * x : x;
    }

    double ReLU::Calculate(double x) const
    {
        return x < 0 ? 0.0 : x;
    }

    double Mish::Calculate(double x) const
    {
        double ex = std::exp(x);
        return x - 2.0 * x / (ex * (ex + 2.0) + 2.0);
    }

    double Swish::Calculate(double x) const
    {
        return x * (1.0 / (1.0 + std::exp(-x)));
    }

    double Sigmoid::Calculate(double x) const
    {
        return 1.0 / (1.0 + std::exp(-x));
    }

    // Core functions

    void Network::InitWeights(const std::vector<double>& weights)
    {
        size_t index = 0;
        for (Layer& layer : this->network)
        {
            // First initialize the flattened weight matrix
            for (size_t i = 0; i < layer.NodesCount; ++i)
            {
                layer.Weights[i] = weights[index++];
            }

            // Then initialize biases for the current layer
            for (size_t i = 0; i < layer.OutputCount; ++i)
            {
                layer.Biases[i] = weights[index++];
            }
        }
    }

    void Network::DefineLayout(const std::vector<size_t>& layout,
        const std::vector<IActivation*>& activations)
    {
        this->network.clear();

        // Create layers to build the neural network
        for (size_t id = 1; id < layout.size(); ++id)
        {
            Layer layer = Layer();

            layer.OutputCount = layout[id];
            layer.InputCount = layout[id - 1];
            layer.NodesCount = layer.OutputCount;
            layer.NodesCount *= layer.InputCount;

            layer.Activation = activations[id - 1];
            layer.Weights.resize(layer.NodesCount);
            layer.Biases.resize(layer.OutputCount);

            this->network.push_back(layer);
        }
    }

    std::vector<double> Network::Predict(std::vector<double> input)
    {
        // Propagate the input through each layer
        std::vector<double> current = input;
        for (const Layer& layer : this->network)
        {
            std::vector<double> next(layer.OutputCount, 0.0);
            for (size_t i = 0; i < layer.OutputCount; ++i)
            {
                double sum = layer.Biases[i];
                for (size_t j = 0; j < layer.InputCount; ++j)
                {
                    // For each neuron, compute weighted sum from the previous layer
                    sum += current[j] * layer.Weights[i + layer.OutputCount * j];
                }
                // Apply the activation function for the layer
                next[i] = layer.Activation->Calculate(sum);
            }
            current = next;
        }
        return current;
    }
}
