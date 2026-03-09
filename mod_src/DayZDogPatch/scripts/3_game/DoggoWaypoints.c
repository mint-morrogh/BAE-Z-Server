class DoggoWaypoint
{
	string name;
	string pos;
	void DoggoWaypoint( string nam, string po)
	{
		name = nam;
		pos = po;
	}

}

class DoggoWaypoints
{
	ref array< ref DoggoWaypoint > WayPoints;
	
	void DoggoWaypoints()
	{
		WayPoints = new array< ref DoggoWaypoint >;
	}

}

class DayzDogWaypointsConfig
{

    protected const static string m_ConfigRoot = "$profile:\\Dayz-Dog\\";

    static void LoadConfig(string configName, out DoggoWaypoints configData)
    {
        string configPath = m_ConfigRoot + configName;
		
		if (!FileExist(m_ConfigRoot))
		{
			MakeDirectory(m_ConfigRoot);
		}
		
        if (!FileExist(configPath))
        {
            return;
        }

        JsonFileLoader<DoggoWaypoints>.JsonLoadFile(configPath, configData);

    }

    static void SaveConfig(string configName, DoggoWaypoints configData)
    {
        string configPath = m_ConfigRoot + configName;

        if (!FileExist(m_ConfigRoot))
        {
            MakeDirectory(m_ConfigRoot);
        }

        JsonFileLoader<DoggoWaypoints>.JsonSaveFile(configPath, configData);
    }
	
    protected static void CreateDefaultConfig(out DoggoWaypoints configData)
    {

	}
	
	
}
