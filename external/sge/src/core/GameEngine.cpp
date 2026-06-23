#include <SGE/core/GameEngine.h>

namespace sge {

GameEngine::GameEngine() :
m_FIXED_SIMULATION_DT((sm::real)1 / 60),
m_accumulator_dt(0),
m_running(true)
{
}

std::shared_ptr<sge::Scene> GameEngine::currentScene()
{
    return this->m_scenes.at(this->m_currentScene);
}

void GameEngine::quit()
{
    this->m_running = false;
}

void GameEngine::run()
{
    using Clock = std::chrono::high_resolution_clock;
    auto prevTime = Clock::now();
    
    const float TARGET_FPS = 300.f;
    const std::chrono::duration<float> TARGET_FRAME_DURATION(1.f / TARGET_FPS);
    
    while (!WindowShouldClose() && this->m_running)
    {
        auto curTime = Clock::now();
        std::chrono::duration<float> elapsed = curTime - prevTime;
        float raw_dt = elapsed.count();

        // Call input system.
        sge::PollInputSystem::update(currentScene()->getRegistry(), currentScene()->getCommandBuffer(), raw_dt);
        
        if (elapsed < TARGET_FRAME_DURATION)
        {
            auto sleepTime = TARGET_FRAME_DURATION - elapsed;
            std::this_thread::sleep_for(sleepTime * 0.9f);
            
            // Spin-lock thread until it's time to continue.
            while (Clock::now() - prevTime < TARGET_FRAME_DURATION)
            {
                #if defined(_MSC_VER)
                _mm_pause();
                #endif
            }
            
            curTime = Clock::now();
            elapsed = curTime - prevTime;
        }
        
        prevTime = curTime;
        raw_dt = elapsed.count();

        if (raw_dt > 0.10f)
        {
            raw_dt = 0.10f;
        }
        
        this->m_accumulator_dt += raw_dt;
        
        // Update current scene using fixed time-step.
        while (this->m_accumulator_dt >= this->m_FIXED_SIMULATION_DT)
        {
            // All registered systems pertaining to the current scene will be invoked.
            currentScene()->update(this->m_FIXED_SIMULATION_DT);
            this->m_accumulator_dt -= this->m_FIXED_SIMULATION_DT;
        }
        
        float alpha = this->m_accumulator_dt / this->m_FIXED_SIMULATION_DT;
        
        // Finally, render current frame.
        BeginDrawing();
        ClearBackground(WHITE);
        sge::RenderingSystem3::update(currentScene()->getRegistry(), currentScene()->getCommandBuffer(), alpha);
        EndDrawing();
    }

    CloseWindow();
}

bool GameEngine::isRunning() const
{
    return this->m_running;
}

} // End namespace SGE
