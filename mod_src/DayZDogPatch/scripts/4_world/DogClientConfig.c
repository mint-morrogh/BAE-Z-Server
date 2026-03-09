class DogClientConfig
{
	protected static ref DogClientConfig Instance;
	static ref DoggoConfig m_DogClientConfig;

	void DogClientConfig()
	{
		m_DogClientConfig = GetDayZGame().GetDayzDogConfig();
		GetRPCManager().AddRPC( "Dayz_Dog", "SyncConfig", this, SingleplayerExecutionType.Server );
		GetRPCManager().AddRPC( "Dayz_Dog", "SyncDogName", this, SingleplayerExecutionType.Server );
		ConfigRequest();
	}

	static DogClientConfig GetInstance()
	{
		if (!Instance)
        {
            Instance = new DogClientConfig();
        }
		return Instance;
	}
	
	static void ClearInstance()
	{
		Instance = null;
	}
	
	void ConfigRequest()
	{
		if(!GetGame().IsMultiplayer()) return; //fix Community Offline Mode - RPC is disabled
		GameOptions gameOptions = new GameOptions;
		ListOptionsAccess lang = ListOptionsAccess.Cast( gameOptions.GetOptionByType( AT_OPTIONS_LANGUAGE ) );
		string currentLang;
		lang.GetItemText(lang.GetIndex(), currentLang);
		Print( "Doggo Requesting config " );
		GetRPCManager().SendRPC("Dayz_Dog", "RequestConfig",  new Param1<string>( currentLang ), true);
	}
	
	void SyncConfig( CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target )
    {
        Param1< DoggoConfig > data;
        if ( !ctx.Read( data ) ) return;
        
        if( type == CallType.Client )
        {
			//prenos konfigu
			m_DogClientConfig = data.param1;
			GetDayZGame().SetDayzDogConfig(m_DogClientConfig);
			
		}
	}
	
	void SyncDogName( CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target )
    {
        Param1< string > data;
        if ( !ctx.Read( data ) ) return;
        
        if( type == CallType.Client )
        {
			PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
			DogPreview_Base dogpr = DogPreview_Base.Cast(player.GetDogSlot());	
			dogpr.SetDogName(data.param1);			
		}
	}
	
};