modded class MissionGameplay
{
	protected int m_LastDogCommandTime;
	
	void MissionGameplay()
	{
		DogClientConfig.ClearInstance();
		DogClientConfig.GetInstance();
	}
	
	void SetLastDogCommandTime(int time)
	{
		m_LastDogCommandTime = time;
	}
	
	bool CanExecuteDogCommand(int time)
	{
		if (time > m_LastDogCommandTime+1) //cooldown 5 seconds
		{
			return true;
		}
			
		return false;
	}
	
	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);

		Input input = GetGame().GetInput();
		PlayerBase doggoplayer = PlayerBase.Cast(GetGame().GetPlayer());
		Object ob = GetCursorObject();
		EntityAI pointedentity = EntityAI.Cast(ob);
		//GetGame().GetMission().OnEvent(ChatMessageEventTypeID, new ChatMessageEventParams(CCDirect, "", "Player pointing " + player.IsPointing() + " " + ob, ""));
		/*
		//pro pole hracu se podivat do triggeru - pridani odebrani
		foreach( Man man : ClientData.m_PlayerBaseList )
        {
			if (player != PlayerBase.Cast(man))
			{
				if (man.IsAlive() && vector.Distance(player.GetPosition(), man.GetPosition()) < 150 && man.GetIdentity().GetName())
				{
					GetGame().GetMission().OnEvent(ChatMessageEventTypeID, new ChatMessageEventParams(CCDirect, "", "Man " + man.GetIdentity().GetName() + " distance " +  vector.Distance(player.GetPosition(), man.GetPosition()), ""));
				}
			}
			
		}
		*/
		if (doggoplayer && CanExecuteDogCommand(GetGame().GetTime()/1000)) //COOLDOWN
		{
			if (pointedentity && dog_shed_small.Cast(pointedentity) && vector.Distance(doggoplayer.GetPosition(), pointedentity.GetPosition()) < 2 )
			{
				//GetGame().GetMission().OnEvent(ChatMessageEventTypeID, new ChatMessageEventParams(CCDirect, "", "Player pointing " + pointedentity, ""));	
				GetRPCManager().SendRPC( "Dayz_Dog", "UpdateDogHouseOwner", new Param1< EntityAI >( pointedentity ) );
			}
			if (doggoplayer.HaveDog())
			{
				if (!doggoplayer.IsInTransport())
				{
					if (doggoplayer.IsPointing() && pointedentity)
					{
						//GetGame().GetMission().OnEvent(ChatMessageEventTypeID, new ChatMessageEventParams(CCDirect, "", "Player pointing " + player.IsPointing() + " " + ob, ""));
						//SetLastDogCommandTime(GetGame().GetTime()/1000);
						GetRPCManager().SendRPC( "Dayz_Dog", "SelectTarget", new Param1< EntityAI >( pointedentity ) );
					}
					
					if ( input.LocalPress( "UADayzDogWhistle", false ) )
					{
						//GetGame().GetMission().OnEvent(ChatMessageEventTypeID, new ChatMessageEventParams(CCDirect, "", "WHISTLE", ""));
						SetLastDogCommandTime(GetGame().GetTime()/1000);
						GetRPCManager().SendRPC( "Dayz_Dog", "Whistle", new Param1< int >( 0 ) );
					}
					
					else if ( input.LocalPress( "UADayzDogFollow", false ) )
					{
						//GetGame().GetMission().OnEvent(ChatMessageEventTypeID, new ChatMessageEventParams(CCDirect, "", "FOLLOW", ""));
						SetLastDogCommandTime(GetGame().GetTime()/1000);
						GetRPCManager().SendRPC( "Dayz_Dog", "ChangeDogBehavior", new Param1< int >( 1 ) );
					}
					
					else if ( input.LocalPress( "UADayzDogPatrol", false ) )
					{
						//GetGame().GetMission().OnEvent(ChatMessageEventTypeID, new ChatMessageEventParams(CCDirect, "", "PATROL", ""));
						SetLastDogCommandTime(GetGame().GetTime()/1000);
						GetRPCManager().SendRPC( "Dayz_Dog", "ChangeDogBehavior", new Param1< int >( 2 ) );
					}
					
					else if ( input.LocalPress( "UADayzDogStayHere", false ) )
					{
						//GetGame().GetMission().OnEvent(ChatMessageEventTypeID, new ChatMessageEventParams(CCDirect, "", "STAY HERE", ""));
						SetLastDogCommandTime(GetGame().GetTime()/1000);
						GetRPCManager().SendRPC( "Dayz_Dog", "ChangeDogBehavior", new Param1< int >( 3 ) );
					}
					else if ( input.LocalPress( "UADayzDogEmoteToggle", false ) )
					{
						//GetGame().GetMission().OnEvent(ChatMessageEventTypeID, new ChatMessageEventParams(CCDirect, "", "STAY HERE", ""));
						SetLastDogCommandTime(GetGame().GetTime()/1000);
						GetRPCManager().SendRPC( "Dayz_Dog", "EmoteToggle", new Param1< int >( 4 ) );
					}			
				}
			}	
		}
	}
	
	static Object GetCursorObject()
    {
        vector rayStart = GetGame().GetCurrentCameraPosition();
        vector rayEnd = rayStart + GetGame().GetCurrentCameraDirection() * 100;
        vector contact_pos;
        vector contact_dir;
        int contact_component;
        set< Object > objects = new set< Object >;
        if( DayZPhysics.RaycastRV( rayStart, rayEnd, contact_pos, contact_dir, contact_component, objects, NULL, GetGame().GetPlayer(), false, false, ObjIntersectView, 0.5 ) )
        {
            if( objects.Count())
            {
                return objects.Get( 0 );
            }
        }

        return NULL;
    }
};