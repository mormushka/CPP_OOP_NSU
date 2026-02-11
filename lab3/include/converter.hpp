#pragma once

#include <map>
#include <memory>
#include <string>
#include <map>
#include <functional>

#include "wav_file.hpp"
#include "exceptions.hpp"

namespace Converters
{
    class IConverter
    {
    public:
        virtual ~IConverter() = default;

        virtual void Process(std::vector<int16_t> &mutableSamples, const std::size_t &currentSecond) = 0;
        virtual void SetParameters(const std::vector<std::string> &params) = 0;
        virtual std::string GetName() const = 0;
        virtual std::string GetDescription() const = 0;
    };

    class Mute : public IConverter
    {
    private:
        std::size_t startTime_;
        std::size_t endTime_;

    public:
        void Process(std::vector<int16_t> &input, const std::size_t &currentSecond) override
        {
            if (currentSecond >= startTime_ && currentSecond < endTime_)
                for (auto &i : input)
                    i = 0;
        }

        void SetParameters(const std::vector<std::string> &params) override
        {
            if (params.size() < 2)
            {
                throw std::invalid_argument("1");
            }

            startTime_ = std::stoul(params[0]);
            endTime_ = std::stoul(params[1]);

            if (startTime_ > endTime_)
            {
                throw std::invalid_argument("1");
            }
        }

        std::string GetName() const override { return "mute"; }

        std::string GetDescription() const override
        {
            return "11";
        }
    };

    class Mix : public IConverter
    {
    private:
        std::shared_ptr<std::vector<int16_t>> additionalSamples_;
        std::size_t startTime_;

    public:
        void SetAdditionalSamples(std::shared_ptr<std::vector<int16_t>> samples)
        {
            additionalSamples_ = samples;
        }

        void Process(std::vector<int16_t> &mutableSamples, const size_t &currentSecond) override
        {
            if (currentSecond < startTime_)
                return;

            size_t minSize = std::min<size_t>(mutableSamples.size(), additionalSamples_->size());

            for (size_t i = 0; i < minSize; i++)
            {
                int32_t mixed = static_cast<int32_t>(mutableSamples[i]) +
                                static_cast<int32_t>((*additionalSamples_)[i]);
                mixed /= 2;

                if (mixed > INT16_MAX)
                    mixed = INT16_MAX;
                if (mixed < INT16_MIN)
                    mixed = INT16_MIN;

                mutableSamples[i] = static_cast<int16_t>(mixed);
            }
        }

        void SetParameters(const std::vector<std::string> &params) override
        {
            if (params.size() < 2)
            {
                throw std::invalid_argument("2");
            }

            startTime_ = std::stoul(params[1]);
        }

        std::string GetName() const override { return "mix"; }

        std::string GetDescription() const override
        {
            return "22";
        }
    };
}

namespace Converters
{
    class Factory
    {
    private:
        std::map<std::string, std::function<std::unique_ptr<IConverter>()>> creators_;

        Factory()
        {
            RegisterConverters();
        }

        void RegisterConverters()
        {
            creators_["mute"] = []()
            { return std::make_unique<Mute>(); };
            creators_["mix"] = []()
            { return std::make_unique<Mix>(); };
        }

    public:
        static Factory &Instance()
        {
            static Factory instance;
            return instance;
        }
        
        std::unique_ptr<IConverter> CreateConverter(const std::string &name)
        {
            auto it = creators_.find(name);
            if (it != creators_.end())
            {
                return it->second();
            }
            return nullptr;
        }
    };
}