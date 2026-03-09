class DoggoConfig
{

	bool EnableDogWaypoints;
	bool PunishDogKillers;
	bool DisablePlayerAttack;
	string SummonItem;
	
	void DoggoConfig()
	{
	
	}

}

class DayzDogConfig
{

    protected const static string m_ConfigRoot = "$profile:\\Dayz-Dog\\";

    static void LoadConfig(string configName, out DoggoConfig configData)
    {
        string configPath = m_ConfigRoot + configName;
		
		if (!FileExist(m_ConfigRoot))
		{
			MakeDirectory(m_ConfigRoot);
		}
		
		if (!FileExist(m_ConfigRoot+"players\\"))
		{
			MakeDirectory(m_ConfigRoot+"players\\");
		}
		
        if (!FileExist(configPath))
        {
            DoggoLogger.Log("'" + configName + "' does not exist, creating default config");
            CreateDefaultConfig(configData);
            SaveConfig(configName, configData);
            return;
        }

        JsonFileLoader<DoggoConfig>.JsonLoadFile(configPath, configData);
        DoggoLogger.Log("'" + configName + "' found, loading existing config");

    }

    protected static void SaveConfig(string configName, DoggoConfig configData)
    {
        string configPath = m_ConfigRoot + configName;

        if (!FileExist(m_ConfigRoot))
        {
            DoggoLogger.Log("'" + m_ConfigRoot + "' does not exist, creating directory");
            MakeDirectory(m_ConfigRoot);
        }

        JsonFileLoader<DoggoConfig>.JsonSaveFile(configPath, configData);
    }
	
	static void UpgradeConfig(string configName, out DoggoConfig configData)
    {
        string configPath = m_ConfigRoot + configName;

        if (!FileExist(configPath))
        {
            DoggoLogger.Log("'"  + configName + "' does not exist, creating default config");
            CreateDefaultConfig(configData);
            SaveConfig(configName, configData);
            return;
        }
		JsonFileLoader<DoggoConfig>.JsonLoadFile(configPath, configData);
		
		//upgrades...
			
        JsonFileLoader<DoggoConfig>.JsonSaveFile(configPath, configData);
    }

    protected static void CreateDefaultConfig(out DoggoConfig configData)
    {
		configData = new DoggoConfig();
		configData.EnableDogWaypoints = false;
		configData.PunishDogKillers = false;
		configData.DisablePlayerAttack = false;
		configData.SummonItem = "Bone";

	}
}
