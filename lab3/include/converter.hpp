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

        virtual void Process(std::vector<int16_t> &mutableSamples,
                             const std::shared_ptr<std::vector<std::vector<int16_t>>> additionalSamples_,
                             const std::size_t &currentSecond) = 0;
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
        void Process(std::vector<int16_t> &mutableSamples,
                     const std::shared_ptr<std::vector<std::vector<int16_t>>> additionalSamples_,
                     const std::size_t &currentSecond) override
        {
            if (currentSecond >= startTime_ && currentSecond < endTime_)
                for (auto &i : mutableSamples)
                    i = 0;
        }

        void SetParameters(const std::vector<std::string> &params) override
        {
            if (params.size() != 2)
                throw Exceptions::InvalidArgument("Invalid count arguments in converter " + GetName());

            startTime_ = std::stoul(params[0]);
            endTime_ = std::stoul(params[1]);

            if (startTime_ > endTime_)
            {
                throw Exceptions::InvalidArgument("Invalid argument in converter " + GetName());
            }
        }

        std::string GetName() const override { return "mute"; }

        std::string GetDescription() const override
        {
            return "mute [start time(sec)] [end time(sec)]";
        }
    };

    class Mix : public IConverter
    {
    private:
        std::size_t addStreamInd_;
        std::size_t startTime_;

    public:
        void Process(std::vector<int16_t> &mutableSamples,
                     const std::shared_ptr<std::vector<std::vector<int16_t>>> additionalSamples_,
                     const size_t &currentSecond) override
        {
            if (currentSecond < startTime_)
                return;

            size_t minSize = std::min<size_t>(mutableSamples.size(),
                                              (*additionalSamples_)[addStreamInd_].size());

            for (size_t i = 0; i < minSize; i++)
            {
                int32_t mixed = static_cast<int32_t>(mutableSamples[i]) +
                                static_cast<int32_t>((*additionalSamples_)[addStreamInd_][i]);
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
            if (params.size() != 2)
            {
                throw Exceptions::InvalidArgument("Invalid count arguments in converter " + GetName());
            }
            addStreamInd_ = std::stoul(params[0]);
            startTime_ = std::stoul(params[1]);
        }

        std::string GetName() const override { return "mix"; }

        std::string GetDescription() const override
        {
            return "mix $[num file stream] [start time(sec)]";
        }
    };

    class Raise : public IConverter
    {
    private:
        float raise_;

    public:
        void Process(std::vector<int16_t> &mutableSamples,
                     const std::shared_ptr<std::vector<std::vector<int16_t>>> additionalSamples_,
                     const size_t &currentSecond) override
        {
            for (auto &ms : mutableSamples)
            {
                int32_t raised = static_cast<int32_t>(ms) * raise_;

                if (raised > INT16_MAX)
                    raised = INT16_MAX;
                if (raised < INT16_MIN)
                    raised = INT16_MIN;

                ms = static_cast<int16_t>(raised);
            }
        }

        void SetParameters(const std::vector<std::string> &params) override
        {
            if (params.size() != 1)
            {
                throw Exceptions::InvalidArgument("Invalid count arguments in converter " + GetName());
            }

            raise_ = std::stof(params[0]);

            if (raise_ < 0)
            {
                throw Exceptions::InvalidArgument("Raise factor cannot be negative");
            }
        }

        std::string GetName() const override { return "raise"; }

        std::string GetDescription() const override
        {
            return "raise [raise]\nExample: raise 1.5 (increase volume by 50%)\n     raise 0.5 (decrease volume by 50%)";
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
            creators_["raise"] = []()
            { return std::make_unique<Raise>(); };
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

        std::vector<std::string> GetAvailableConverters() const
        {
            std::vector<std::string> names;
            for (const auto &[name, creator] : creators_)
            {
                names.push_back(name);
            }
            return names;
        }
    };
}