class dog_shed_small extends BaseBuildingBase
{
	protected int m_DogOwnerID;
	protected string m_DogOwnerSteamID;
	void dog_shed_small()
	{
		m_DogOwnerSteamID = "0";
		RegisterNetSyncVariableInt("m_DogOwnerID");
		//GetRPCManager().AddRPC( "Dayz_Dog", "UpdateOwner", this, SingleplayerExecutionType.Client );
		//SetEventMask(EntityEvent.CONTACT);
	}
	
	override bool CanDropEntity(notnull EntityAI item)
	{
		return false;
	}
	/*
	override void EOnContact(IEntity other, Contact extra)
	{
		super.EOnContact(other, extra);
		DoggoLogger.Log("Dogshed " + other);
	}
	*/	
	override void EEItemAttached(EntityAI item, string slot_name)
	{
		super.EEItemAttached(item, slot_name);
		DogPreview_Base dogitem = DogPreview_Base.Cast(item);
		if (GetGame().IsServer() && dogitem)
		{
			//DoggoLogger.Log("Dogshed " + EntityAI.Cast(this).GetID() + " SET OWNER STEAM ID " + dogitem.GetOwnerSteamID());
			SetDogOwnerSteamID(dogitem.GetOwnerSteamID());	
		}
		else if (GetGame().IsClient() && dogitem)
		{
			//GetRPCManager().SendRPC( "Dayz_Dog", "UpdateOwner", new Param1< dog_shed_small >( this ) );
		}
		
	}
	/*
	void UpdateOwner( CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target )
	{
		Param1< dog_shed_small > data;
        if ( !ctx.Read( data ) ) return;
		
		if( type == CallType.Server )
        {	
			//DoggoLogger.Log("DOG SET OWNER ID RPC " + this.GetID());
			SetDogOwnerID();
			
		}
	}
	*/
	void SetDogOwnerID()
	{
		m_DogOwnerID = GetOwnerFromSteamID(GetDogOwnerSteamID());
		//DoggoLogger.Log("Dogshed " + EntityAI.Cast(this).GetID() + " OWNERID " + m_DogOwnerID + " STEAMID " + GetDogOwnerSteamID());
		SetSynchDirty();	
	}
	
	int GetDogOwnerID()
	{
		if (GetGame().IsServer())
		{
			SetDogOwnerID();
		}
		else
		{
			//GetRPCManager().SendRPC( "Dayz_Dog", "UpdateOwner", new Param1< dog_shed_small >( this ) ); 
		}
		return m_DogOwnerID;
	}
	
	void SetDogOwnerSteamID(string steamid)
	{	
		m_DogOwnerSteamID = steamid;
	}
	
	string GetDogOwnerSteamID()
	{
		return m_DogOwnerSteamID;
	}
	
	int GetOwnerFromSteamID(string steamid)
	{
		int ownerid = 0;
		//DoggoLogger.Log("DOG SHED STEAMID " + steamid );
		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);
		for(int i = 0; i < players.Count(); i++)
		{
			if(players.Get(i).GetIdentity().GetPlainId() == steamid)
			{
				//DoggoLogger.Log("DOG SHED STEAMID " + steamid + " ownerid " + PlayerBase.Cast(players.Get(i)).GetID());
				return PlayerBase.Cast(players.Get(i)).GetID();
			}
		}

		return ownerid;
	}
		
	override void AfterStoreLoad()
	{
		
	}
	
	override void SetPartsAfterStoreLoad()
	{
		
	}
	
	override void OnStoreSave( ParamsWriteContext ctx )
	{
		super.OnStoreSave( ctx );
		ctx.Write( 1 ); // version
		ctx.Write( m_DogOwnerID );
		ctx.Write( m_DogOwnerSteamID );
		//DoggoLogger.Log("dog shed " + EntityAI.Cast(this).GetID() + " save owner steamid " + m_DogOwnerSteamID);
	}
	
	override bool OnStoreLoad( ParamsReadContext ctx, int version )
	{
		if ( !super.OnStoreLoad( ctx, version ) )
			return false;

		int dog_version;
		if ( !ctx.Read( dog_version ) )
			return false;

		if ( !ctx.Read( m_DogOwnerID ) )
			return false;
		
		if ( !ctx.Read( m_DogOwnerSteamID ) )
			return false;
		//DoggoLogger.Log("dog shed " + EntityAI.Cast(this).GetID() + " load owner steamid " + m_DogOwnerSteamID);

		return true;
	}
	
	override string GetConstructionKitType()
	{
		return "dog_shed_small_kit";
	}
	
	override bool CanReleaseAttachment(EntityAI attachment)
	{
		return true;
	}

	override bool CanReceiveAttachment(EntityAI attachment,int slotId)
	{
		return true;
	}
	
	/*
	override bool IsInventoryVisible()
	{
		//should be visible only for dog owner or when is empty
		return false;
	}
	*/
	override void SetActions()
	{
		super.SetActions();
		AddAction(ActionSummonDogMenu);
		AddAction(ActionPutDogToKennel);
		AddAction(ActionTakeDogFromKennel);
    }
}