class DogServerRPC
{
	protected static ref DogServerRPC Instance;
	
	static DogServerRPC GetInstance()
	{
		if (!Instance)
        {
            Instance = new DogServerRPC();
        }
		return Instance;
	}
	
	static void ClearInstance()
	{
		Instance = null;
	}
	
	void DogServerRPC()
    {
		GetRPCManager().AddRPC( "Dayz_Dog", "SelectTarget", this, SingleplayerExecutionType.Client );
		GetRPCManager().AddRPC( "Dayz_Dog", "SpawnDog", this, SingleplayerExecutionType.Client );
		GetRPCManager().AddRPC( "Dayz_Dog", "ChangeDogBehavior", this, SingleplayerExecutionType.Client );
		GetRPCManager().AddRPC( "Dayz_Dog", "Whistle", this, SingleplayerExecutionType.Client );
		GetRPCManager().AddRPC( "Dayz_Dog", "RequestConfig", this, SingleplayerExecutionType.Client );
		GetRPCManager().AddRPC( "Dayz_Dog", "ServerSpawnDog", this, SingleplayerExecutionType.Client );
		GetRPCManager().AddRPC( "Dayz_Dog", "SetDogWaypoint", this, SingleplayerExecutionType.Client );
		GetRPCManager().AddRPC( "Dayz_Dog", "EmoteToggle", this, SingleplayerExecutionType.Client );
		GetRPCManager().AddRPC( "Dayz_Dog", "UpdateDogHouseOwner", this, SingleplayerExecutionType.Client );
		GetRPCManager().AddRPC( "Dayz_Dog", "SetNameDog", this, SingleplayerExecutionType.Client );
	}
	
	void SelectTarget( CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target )
	{
		Param1< EntityAI > data;
        if ( !ctx.Read( data ) ) return;
		
		if( type == CallType.Server )
        {	
			if (data.param1 && data.param1.IsAlive())
			{
				PlayerBase plr = PlayerBase.Cast(data.param1);
				DayZCreature creature = DayZCreature.Cast(data.param1);
				if (creature || plr)
				{
					PlayerBase player = GetPlayerByPlainID(sender.GetPlainId());
					//only when target was not set previously
					//DoggoLogger.Log("Attack Target " + player.GetMyDog().GetDoggoAttackTarget() + " point " + data.param1 );
					if (player.GetMyDog() && player.GetMyDog().GetDoggoAttackTarget() != data.param1 && player.GetMyDog().CanListenEmotes() && player.GetMyDog() != data.param1 )
					{
						player.GetMyDog().SetDoggoAttackTarget(data.param1); //null pointer need fix
						NotificationSystem.SendNotificationToPlayerExtended(player, 2, "#DD_Dog", "#DD_target " + data.param1.GetDisplayName(), "set:dayzdog_gui image:dog_run");
						// set to patrol with waypoint
						if (player.GetMyDog().GetDoggoCmd() != DoggoCmd.PATROL)
						{
							player.GetMyDog().SetDoggoCmd(DoggoCmd.PATROL);
						}
						
					}
					
				}
			}

		}
	}
	
	void SpawnDog( CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target )
    {
        Param2< int, vector > data;
        if ( !ctx.Read( data ) ) return;
        
        if( type == CallType.Server )
        {
			PlayerBase player = GetPlayerByPlainID(sender.GetPlainId());

			Dayz_Doggo target_dog = Dayz_Doggo.Cast(GetGame().CreateObject("Doggo_Follow"+data.param1.ToString(), data.param2, false, false, true));
			AIGroup grp = GetGame().GetWorld().GetAIWorld().CreateGroup("DZDogGroupBeh");
			target_dog.InitAIAgent(grp);
			target_dog.SetOwnerId(player.GetID());
			player.setMyDog(target_dog);
			target_dog.SetDoggoCmd(DoggoCmd.STAY);
			//player.SetDogIndex(data.param1);
			DoggoLogger.Log("Player " + player.GetIdentity().GetName() + " spawn dog using dog house at " + data.param2);
			//ulozeni do konfigu
			/*
			DoggoConfig config = GetDayZGame().GetDayzDogConfig();
			private static FileHandle configFile;
			string DoggoconfDir = "$profile:\\Dayz-Dog\\players\\";
			if (config.StoreDogStatus)
			{
				configFile = OpenFile(DoggoconfDir + player.GetIdentity().GetPlainId() + ".json", FileMode.WRITE);
				//FPrintln(configFile, message);
				CloseFile(configFile);
			}
			*/
        }
    }
	
	void ChangeDogBehavior( CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target )
	{
		Param1< int > data;
        if ( !ctx.Read( data ) ) return;
		
		if( type == CallType.Server )
        {	
			PlayerBase player = GetPlayerByPlainID(sender.GetPlainId());
			AIGroup grp = GetGame().GetWorld().GetAIWorld().CreateGroup("DZDogGroupBeh");
			AIGroup grp0 = GetGame().GetWorld().GetAIWorld().CreateGroup("Doggo_Follow_Old");
			AIGroup grp1 = GetGame().GetWorld().GetAIWorld().CreateGroup("Doggo_Predator");
			if (player.GetMyDog() && player.GetMyDog().GetDoggoCmd() != DoggoCmd.STAY && data.param1 == 3 && GetGame().GetWaterDepth(  player.GetMyDog().GetPosition() ) < 0.1)
			{
				//RespawnDog(player, "Doggo_Follow", DoggoCmd.STAY);
				player.GetMyDog().DestroyAIAgent();
				player.GetMyDog().InitAIAgent(grp); //grp1
				player.GetMyDog().SetDoggoCmd(DoggoCmd.STAY);
			}
			else if (player.GetMyDog() && player.GetMyDog().GetDoggoCmd() != DoggoCmd.FOLLOW && data.param1 == 1)
			{
				//RespawnDog(player, "Doggo_Follow", DoggoCmd.FOLLOW);
				player.GetMyDog().DestroyAIAgent();
				player.GetMyDog().InitAIAgent(grp);
				player.GetMyDog().SetDoggoCmd(DoggoCmd.FOLLOW);
			}
			else if (player.GetMyDog() && player.GetMyDog().GetDoggoCmd() != DoggoCmd.PATROL && data.param1 == 2 )
			{
				//RespawnDog(player, "Doggo_Patrol", DoggoCmd.PATROL);
				
				player.GetMyDog().DestroyAIAgent();
				player.GetMyDog().InitAIAgent(grp);
				player.GetMyDog().SetDoggoCmd(DoggoCmd.PATROL);
			}

		}
	}
	
	void ServerSpawnDog( CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target )
    {
        Param1< vector > data;
        if ( !ctx.Read( data ) ) return;
        
        if( type == CallType.Server )
        {
			PlayerBase player = GetPlayerByPlainID(sender.GetPlainId());
			DoggoLogger.Log("Player have dog " + player.HaveDog() + " X " + player.GetMyDog());
			if (player.HaveDog() && !player.GetMyDog())
			{
				DoggoLogger.Log("INITIAL DOG SPAWN");
				player.InitialDogSpawn(data.param1);
			}
			
		}
	}
	
	void SetDogWaypoint( CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target )
	{
		Param1< vector > data;
        if ( !ctx.Read( data ) ) return;
		
		if( type == CallType.Server )
        {	
			PlayerBase player = GetPlayerByPlainID(sender.GetPlainId());
			if (player.HaveDog() && player.GetMyDog())
			{
				player.GetMyDog().SetDoggoWaypoint(data.param1);
				player.GetMyDog().SetDoggoCmd(DoggoCmd.MISSION);
			}
		}
	}
	
	void SetNameDog( CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target )
	{
		Param1< string > data;
        if ( !ctx.Read( data ) ) return;
		
		if( type == CallType.Server )
        {	
			PlayerBase player = GetPlayerByPlainID(sender.GetPlainId());
			if (player.HaveDog() && player.GetMyDog())
			{
				DogPreview_Base dogpr = DogPreview_Base.Cast(player.GetDogSlot());
				dogpr.SetDogName(data.param1);
				//player.GetMyDog().SetDoggoWaypoint(data.param1);
			}
		}
	}
	
	void EmoteToggle (CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target )
	{
		Param1< int > data;
        if ( !ctx.Read( data ) ) return;
		
		if( type == CallType.Server )
        {	
			PlayerBase player = GetPlayerByPlainID(sender.GetPlainId());
			//DoggoLogger.Log("EMOTE TOGGLE RPC");
			if (player.HaveDog() && player.GetMyDog())
			{
				if (player.GetMyDog().CanListenEmotes())
				{
					player.GetMyDog().SetListenEmotes(false);
					//DoggoLogger.Log("SET EMOTES FALSE");
					NotificationSystem.SendNotificationToPlayerExtended(player, 2, "#DD_dog", "#STR_DD_emotes_disabled", "set:dayzdog_gui image:dog_stay");
				}
				else
				{
					player.GetMyDog().SetListenEmotes(true);
					//DoggoLogger.Log("SET EMOTES TRUE");
					NotificationSystem.SendNotificationToPlayerExtended(player, 2, "#DD_dog", "#STR_DD_emotes_enabled", "set:dayzdog_gui image:dog_stay");
				}
				
			}
		}
	}
	
	void UpdateDogHouseOwner( CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target )
	{
		Param1< EntityAI > data;
        if ( !ctx.Read( data ) ) return;
		
		if( type == CallType.Server )
        {
			dog_shed_small dogshed = dog_shed_small.Cast(data.param1);
			if (dogshed)
			{
				dogshed.SetDogOwnerID();
			}	
		}
	}
	void RespawnDog(PlayerBase player, string dog, int doggocmd)
	{
		if (player.GetMyDog().GetType() == dog+player.GetDogIndex())
		{
			player.GetMyDog().SetDoggoCmd(doggocmd);
			return;
		}
		vector targetObjectPos = player.GetMyDog().GetPosition();
		vector tmDog[4];
		player.GetMyDog().GetTransform(tmDog);
		GetGame().ObjectDelete( player.GetMyDog() );
		Dayz_Doggo target_dog = Dayz_Doggo.Cast(GetGame().CreateObject(dog+player.GetDogIndex(), targetObjectPos, false, true ));
		target_dog.SetOwnerId(player.GetID());
		target_dog.SetTransform(tmDog);
		player.setMyDog(target_dog);
		player.GetMyDog().SetDoggoCmd(doggocmd);
		//target_dog.OnSoundEvent(2, "WolfBark2_SoundSet");
	}
	
	static void SpawnTamedDog(PlayerBase playerz, Dayz_Doggo dog)
	{
		int dogindex = dog.GetDogIndex();
		vector pos = dog.GetPosition();
		GetGame().ObjectDelete( dog );
		Dayz_Doggo target_dog = Dayz_Doggo.Cast(GetGame().CreateObject("Doggo_Follow"+dogindex, pos, false, true ));
		target_dog.SetOwnerId(playerz.GetID());
		//playerz.SetDogIndex(dogindex);
		playerz.setMyDog(target_dog);
		target_dog.SetDoggoCmd(DoggoCmd.FOLLOW);
		DoggoLogger.Log("Player " + playerz.GetIdentity().GetName() + " tamed dog at pos " + pos);
		//ulozeni do konfigu
		/*
		DoggoConfig config = GetDayZGame().GetDayzDogConfig();
		private static FileHandle configFile;
		string DoggoconfDir = "$profile:\\Dayz-Dog\\players\\";
		if (config.StoreDogStatus)
		{
			configFile = OpenFile(DoggoconfDir + playerz.GetIdentity().GetPlainId() + ".json", FileMode.WRITE);
			//FPrintln(configFile, message);
			CloseFile(configFile);
		}
		*/	
	}
	
	void Whistle( CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target )
	{
		Param1< int > data;
        if ( !ctx.Read( data ) ) return;
		
		if( type == CallType.Server )
        {
			private PlayerBase player = GetPlayerByPlainID(sender.GetPlainId());
			//player.ResetWhistle();
			if (!player.HaveDog())
				return;
			if (player.GetMyDog() && player.GetMyDog().GetDoggoCmd() == DoggoCmd.PLRCONTROL)
				return;
			if (player.GetMyDog())
			{
				player.BlowWhistle();
				player.GetMyDog().SetDoggoCmd(DoggoCmd.FOLLOW);
				GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( TPDog, 1500, false , player);
			}

		}
	}
	void TPDog(PlayerBase player)
	{
		//Camera testing - need send rpc everytime set my dog to client
		//GetCurrentCamera
		//GetGame().SelectSpectator( player.GetIdentity() , "DoggoCamera", (player.GetMyDog().GetPosition() + Vector(0,1,-1)));
		//return;
		vector Doggo_location = player.ModelToWorld("-0.5 0 1");
		if (!player.GetMyDog())
		{
			return;
		}
		//vector targetObjectPos;
		/*
		if (player.GetMyDog())
		{
		  targetObjectPos = player.GetMyDog().GetPosition();
		}
		*/		
		/*
		else
		{
			targetObjectPos = Doggo_location; 	
		}
		
		GetGame().ObjectDelete( player.GetMyDog());
		Dayz_Doggo target_dog = Dayz_Doggo.Cast(GetGame().CreateObject("Doggo_Follow"+player.GetDogIndex(), targetObjectPos, false, false ));

				//needs to be an rpc
		AIGroup grp = GetGame().GetWorld().GetAIWorld().CreateGroup("DZDogGroupBeh");
		///AIGroup grp = GetGame().GetWorld().GetAIWorld().CreateGroup("Doggo_Follow_Old");
		target_dog.InitAIAgent(grp);
		target_dog.SetOwnerId(player.GetID());
		player.setMyDog(target_dog);
		*/
		AIGroup grp = GetGame().GetWorld().GetAIWorld().CreateGroup("DZDogGroupBeh");
		player.GetMyDog().SetPosition( Doggo_location );
		player.GetMyDog().DestroyAIAgent();
		player.GetMyDog().InitAIAgent(grp);
		player.GetMyDog().SetDoggoCmd(DoggoCmd.FOLLOW);
		player.ResetWhistle();
	}
	
	void RequestConfig( CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target )
    {
		if(type == CallType.Server)
        {
			Param1<string> data;
			if (!ctx.Read(data)) return; //Return if data sent over was null/unable to deserialize 
		
			DoggoConfig config =  GetDayZGame().GetDayzDogConfig();
			GetRPCManager().SendRPC( "Dayz_Dog", "SyncConfig", new Param1<DoggoConfig>(config) );
			
		}
	}
	
	protected static PlayerBase GetPlayerByPlainID(string plainID)
	{
		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);
		for(int i = 0; i < players.Count(); i++)
		{
			if(players.Get(i).GetIdentity().GetPlainId() == plainID)
			{
				return PlayerBase.Cast(players.Get(i));
			}
		}
		return null;
	}
	
}