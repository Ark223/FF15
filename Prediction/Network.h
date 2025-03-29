
#ifndef NETWORK_H
#define NETWORK_H

#include <cmath>
#include <vector>

namespace IPrediction
{
    class IActivation
    {
        public:
            virtual ~IActivation() {}
            virtual double Calculate(double x) const = 0;
    };

    class Linear : public IActivation
    {
        public:
            double Calculate(double x) const override;
    };

    class LeakyReLU : public IActivation
    {
        private:
            double m_alpha;

        public:
            double Calculate(double x) const override;
            explicit LeakyReLU(double alpha = 0.01) : m_alpha(alpha) {}
    };

    class ReLU : public IActivation
    {
        public:
            double Calculate(double x) const override;
    };

    class Mish : public IActivation
    {
        public:
            double Calculate(double x) const override;
    };

    class Swish : public IActivation
    {
        public:
            double Calculate(double x) const override;
    };

    class Sigmoid : public IActivation
    {
        public:
            double Calculate(double x) const override;
    };

    struct Layer
    {
        size_t InputCount = 0;
        size_t OutputCount = 0;
        size_t NodesCount = 0;
        IActivation* Activation;
        std::vector<double> Weights;
        std::vector<double> Biases;
    };

    class Network
    {
        private:
            std::vector<Layer> network;
            static Network* m_instance;

        public:
            static void Destroy();
            static Network* Get();

            void InitWeights(const std::vector<double>& weights);
            void DefineLayout(const std::vector<size_t>& layout,
                const std::vector<IActivation*>& activations);
            std::vector<double> Predict(std::vector<double> input);
    };
}

#endif // NETWORK_H
