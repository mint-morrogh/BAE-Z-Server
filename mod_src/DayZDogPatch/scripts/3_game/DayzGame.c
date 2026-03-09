modded class DayZGame
{	
	protected ref DoggoConfig m_DayzDogConfig;
	protected ref DoggoWaypoints m_DayzDogWaypoints;
	
    void SetDayzDogConfig(DoggoConfig config)
    {
        m_DayzDogConfig = config;
    }

    DoggoConfig GetDayzDogConfig()
    {
		return m_DayzDogConfig;
    }
	
    DoggoWaypoints GetDogWayPoints()
    {
		if (!m_DayzDogWaypoints)
			m_DayzDogWaypoints = new DoggoWaypoints();
		return m_DayzDogWaypoints;
    }
	
};