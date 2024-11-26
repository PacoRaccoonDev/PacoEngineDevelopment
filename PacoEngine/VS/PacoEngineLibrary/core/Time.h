#pragma once
#include<iostream>
#include <chrono>

struct TimeManager
{
    int fps = 0;

    long long fixed_delta_time = 0.0;

    std::chrono::milliseconds fixed_delta_frequency = std::chrono::milliseconds(0);

    std::chrono::time_point<std::chrono::high_resolution_clock> last_update_time;

    std::chrono::duration<float> t_accumulator = std::chrono::duration<float>::zero();

    double current_delta_time;

    bool is_fixed_step;
};

struct TimeManagerData
{
    long long fixed_delta_time;
    long long fixed_delta_frequency;
};

class Stopwatch {
public:
    // Constructor initializes the running flag to false
    Stopwatch() : m_running(false) {}

    // Starts the stopwatch
    void Start() {
        if (!m_running) {
            m_start_time = std::chrono::high_resolution_clock::now();
            m_running = true;
        }
    }

    // Stops the stopwatch
    void Stop() {
        if (m_running) {
            m_end_time = std::chrono::high_resolution_clock::now();
            m_running = false;
        }
    }

    // Stops the stopwatch with message
    void StopAndPrintNs(std::string p_message = "") {
        if (m_running) {
            m_end_time = std::chrono::high_resolution_clock::now();
            m_running = false;
            PrintElapsedTimeNs(p_message);
        }
    }

    // Stops the stopwatch with message
    void StopAndPrintMs(std::string p_message = "") {
        if (m_running) {
            m_end_time = std::chrono::high_resolution_clock::now();
            m_running = false;
            PrintElapsedTimeMs(p_message);
        }
    }

    // Prints the elapsed time in milliseconds
    void PrintElapsedTimeMs(std::string p_message = "") const {
        if (!m_running) {
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(m_end_time - m_start_time);
            std::cout << p_message << "Elapsed time: " << elapsed.count() << " ms" << std::endl;
        }
        else {
            std::cout << "Stopwatch is still running. Stop it to print elapsed time." << std::endl;
        }
    }

    // Prints the elapsed time in nanoseconds
    void PrintElapsedTimeNs() const {
        if (!m_running) {
            auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(m_end_time - m_start_time);
            std::cout << "Elapsed time: " << elapsed.count() << " ns" << std::endl;
        }
        else {
            std::cout << "Stopwatch is still running. Stop it to print elapsed time." << std::endl;
        }
    }

    // Prints the elapsed time in nanoseconds
    void PrintElapsedTimeNs(std::string p_message = "") const {
        if (!m_running) {
            auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(m_end_time - m_start_time);
            std::cout << p_message << "Elapsed time: " << elapsed.count() << " ns" << std::endl;
        }
        else {
            std::cout << "Stopwatch is still running. Stop it to print elapsed time." << std::endl;
        }
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_start_time;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_end_time;
    bool m_running;
};

namespace TimeFunctions
{
    void SetFixedDeltaTimeData(TimeManager& p_time, long long p_fixed_delta_time, long long p_fixed_delta_frequency)
    {
        p_time.fixed_delta_time = p_fixed_delta_time;
        p_time.fixed_delta_frequency = std::chrono::milliseconds(p_fixed_delta_frequency);
    }

    static void Initialize(TimeManager& p_time, const TimeManagerData& p_time_data)
    {
        SetFixedDeltaTimeData(p_time, p_time_data.fixed_delta_time, p_time_data.fixed_delta_frequency);
        p_time.last_update_time = std::chrono::high_resolution_clock::now();
        p_time.t_accumulator = std::chrono::duration<float>::zero();
    }

    static bool IsFixedStep(TimeManager& p_time)
    {
        if (p_time.t_accumulator >= p_time.fixed_delta_frequency)
        {
            p_time.t_accumulator -= p_time.fixed_delta_frequency;

            // This should prevent things from moving when you block the execution of the loop (e.g., Moving Window)
            if (p_time.t_accumulator > p_time.fixed_delta_frequency)
            {
                return false;
            }

            return true;
        }
        else
        {
            return false;
        }
    }

    static double GetDeltaTime(TimeManager& p_time)
    {
        auto current_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> delta_time = current_time - p_time.last_update_time;
        p_time.fps = 1.0 / delta_time.count();

        p_time.last_update_time = current_time;
        p_time.t_accumulator += delta_time;
        return delta_time.count();
    }

    void UpdateTime(TimeManager& p_time_manager)
    {
        p_time_manager.current_delta_time = TimeFunctions::GetDeltaTime(p_time_manager);
        p_time_manager.is_fixed_step = TimeFunctions::IsFixedStep(p_time_manager);
    }
}
