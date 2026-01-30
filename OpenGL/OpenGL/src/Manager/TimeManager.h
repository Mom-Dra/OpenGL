#pragma once

class TimeManager
{
private:
	double deltaTime{ 0.0 };
	double lastTime{ 0.0 };

	//unsigned int fps{ 0 };
	//unsigned int updateCallCount{ 0 };

public:
	static TimeManager& GetInstance() noexcept;
	void Initialize() noexcept;

	float GetDeltaTime() const noexcept;
	float GetNow() const noexcept;
	void Update() noexcept;

private:
	explicit TimeManager() noexcept = default;
	explicit TimeManager(const TimeManager& other) noexcept = delete;
	TimeManager& operator=(const TimeManager& other) noexcept = delete;
};