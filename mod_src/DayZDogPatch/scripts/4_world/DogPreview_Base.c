class DogPreview_Base extends Inventory_Base
{
	protected int m_OwnerID;
	protected vector m_LastDogPos;
	protected float m_DogHealth;
	protected string m_OwnerSteamID;
	protected string m_DogName;
	//protected bool m_IsLocked;
		//damagezones
	protected ref array<ref Param2<string,float>> m_DogDamageData = new array<ref Param2<string,float>>; 
	
	//adding health, size, age, skills?
	protected int m_ZedsKilled;
	protected int m_AnimalsKilled;
	protected int m_ChickensCatched;
	protected int m_SurvivorsBitten;
	/*
	override bool CanReleaseAttachment (EntityAI attachment)
	{
		return false;
	}
	*/
	void DogPreview_Base()
	{
		RegisterNetSyncVariableFloat("m_DogHealth");
		RegisterNetSyncVariableInt("m_ZedsKilled");
		RegisterNetSyncVariableInt("m_AnimalsKilled");
		RegisterNetSyncVariableInt("m_ChickensCatched");
		RegisterNetSyncVariableInt("m_SurvivorsBitten");
		//RegisterNetSyncVariableBool("m_IsLocked");
		//SetEventMask(EntityEvent.INIT);
	}
	
	override void SetActions()
	{
		RemoveAction(ActionDropItem);
	}
	
	override void EOnInit( IEntity other, int extra )
	{
		//m_IsLocked = true;
		/*
		if (GetGame().IsClient())
		{
			this.GetInventory().LockInventory(HIDE_INV_FROM_SCRIPT);
		}
		*/
		//this.GetInventory().LockInventory(HIDE_INV_FROM_SCRIPT);
	}
	
	/*
	bool CanDisplayCargo()
	{
		return false;
	}
	*/
	/*
	override bool CanDisplayAttachmentSlot( string slot_name )
    {
		return false;
	}
	
	override bool CanDisplayAttachmentCategory( string category_name )
    {
		return false;
	}
	
	override bool CanReleaseAttachment(EntityAI attachment)
	{
		return false;
	}
	*/
	/*
	void LockPreview(bool lock)
	{
		m_IsLocked = lock;
		SetSynchDirty();
	}
	bool IsPreviewLocked()
	{
		return m_IsLocked;
	}
	*/
	override bool CanDisplayAttachmentSlot( int slot_id )
	{
		return false;
	}
	
	override bool CanReceiveAttachment(EntityAI attachment,int slotId)
	{
		InventoryLocation il = new InventoryLocation();
		attachment.GetInventory().GetCurrentInventoryLocation(il);
		if (il.GetType() != InventoryLocationType.ATTACHMENT && il.GetType() != InventoryLocationType.UNKNOWN)
		{
			return false;
		}
		return true;
	}
	
	
	override bool IsInventoryVisible()
	{
		return false;
	}
	
	override bool IsTakeable()
	{
		return false;
	}
	
	
	void StoreDogDamage(EntityAI dog)
    {
        array<array<string>> components;
        
        DamageZoneMap zonemap = dog.GetEntityDamageZoneMap();
        components = zonemap.GetValueArray();
        
        for( int i=0; i < components.Count(); i++)
        {
            array<string> DmgZoneElement = components.Get(i);
            for( int j=0; j < DmgZoneElement.Count(); j++)
            {
                string DamageZone = zonemap.GetKey(i);
                float value = dog.GetHealth( DamageZone, "");

                m_DogDamageData.Insert( new Param2<string,float>( DamageZone, value ));
            }
        }
    }
    
    void RestoreDogDamage(EntityAI dog)
    {
        for ( int i=0; i < m_DogDamageData.Count(); i++)
        {
            Param2<string,float> BodyDamageZone = m_DogDamageData.Get(i);
            
            dog.SetHealth( BodyDamageZone.param1, "", BodyDamageZone.param2);
        }
    }
	
	
	override string GetTooltip()
	{
		return ("WIP\n\n Doggo health: " + GetDogHealth().ToString());
		
	}
	int GetOwnerID()
	{
		return m_OwnerID;
	}
	
	void SetOwnerID(int owner)
	{
		m_OwnerID = owner;
		//DoggoLogger.Log("preview set owner id " + m_OwnerID);
	}
	
	string GetOwnerSteamID()
	{
		//DoggoLogger.Log("Preview owner STEAM ID " + m_OwnerSteamID);
		return m_OwnerSteamID;
	}
	
	void SetOwnerSteamID(string steamid)
	{
		m_OwnerSteamID = steamid;
		//DoggoLogger.Log("preview set owner id " + m_OwnerSteamID);
	}
	
	string GetDogName()
	{
		return m_DogName;
	}
	
	void SetDogName(string name)
	{
		m_DogName = name;
	}
	
	int GetZedKills()
	{
		return m_ZedsKilled;
	}
	
	void AddZedKill()
	{
		m_ZedsKilled++;
		SetSynchDirty();
	}
	
	int GetAnimalKills()
	{
		return m_AnimalsKilled;
	}
	
	void AddAnimalKill()
	{
		m_AnimalsKilled++;
		SetSynchDirty();
	}
	
	int GetChickenKills()
	{
		return m_ChickensCatched;
	}
	
	void AddChickenKill()
	{
		m_ChickensCatched++;
		SetSynchDirty();
	}
	
	vector GetLastDogPos()
	{
		return m_LastDogPos;
	}
	
	int GetSurvivorHits()
	{
		return m_SurvivorsBitten;
	}
	
	void AddSurvivorHit()
	{
		m_SurvivorsBitten++;
		SetSynchDirty();
	}
	
	void SetLastDogPos(vector pos)
	{
		m_LastDogPos = pos;
	}
	
	void SetDogHealth(EntityAI dog)
	{
		if (GetGame().IsServer())
		{
			m_DogHealth = dog.GetHealth("","");
			SetSynchDirty();
		}
	}
	
	float GetDogHealth()
	{
		return m_DogHealth;
	}
	/*
	override bool CanPutIntoHands(EntityAI parent)
	{
		return false;
    } 
	*/
	override bool CanPutInCargo( EntityAI parent )
    {
        return false;
    }
	
	override void OnVariablesSynchronized()
    {
        super.OnVariablesSynchronized();
    }
	
	override void OnStoreSave( ParamsWriteContext ctx )
	{
		super.OnStoreSave( ctx );
		ctx.Write( 2 ); // version
		ctx.Write( m_OwnerID );
		ctx.Write( m_LastDogPos );
		ctx.Write( m_DogHealth );
		ctx.Write( m_OwnerSteamID);
		ctx.Write( m_DogName);
		ctx.Write( m_ZedsKilled);
		ctx.Write( m_AnimalsKilled);
		ctx.Write( m_ChickensCatched);
		ctx.Write( m_SurvivorsBitten);
		//DoggoLogger.Log("preview save owner steamid " + m_OwnerSteamID);
	}
	
	override bool OnStoreLoad( ParamsReadContext ctx, int version )
	{
		if ( !super.OnStoreLoad( ctx, version ) )
			return false;

		int dog_version;
		if ( !ctx.Read( dog_version ) )
			return false;

		if ( !ctx.Read( m_OwnerID ) )
			return false;
		
		if ( !ctx.Read( m_LastDogPos ) )
			return false;
		
		if ( !ctx.Read( m_DogHealth ) )
			return false;
		
		if ( !ctx.Read( m_OwnerSteamID ) )
			return false;
		if ( !ctx.Read( m_DogName ) && dog_version > 1 )
			return false;
		if ( !ctx.Read( m_ZedsKilled ) && dog_version > 1 )
			return false;
		if ( !ctx.Read( m_AnimalsKilled ) && dog_version > 1 )
			return false;
		if ( !ctx.Read( m_ChickensCatched ) && dog_version > 1 )
			return false;
		if ( !ctx.Read( m_SurvivorsBitten ) && dog_version > 1 )
			return false;
		DoggoLogger.Log("preview load dog name " + m_DogName);
		
		if (this.GetParent() && this.GetParent().IsInherited(PlayerBase))
		{
			DoggoLogger.Log("player " + this.GetParent());
			//DogPreview_Base dogpr = DogPreview_Base.Cast(player.GetDogSlot());
			//string dogname = dogpr.GetDogName();
			//GetRPCManager().SendRPC( "Dayz_Dog", "SyncDogName", new Param1<string>(dogname) );
		}
		
		return true;
	}
	
};

class DogPreview1 extends DogPreview_Base {};
class DogPreview2 extends DogPreview_Base {};
class DogPreview3 extends DogPreview_Base {};
class DogPreview4 extends DogPreview_Base {};
class DogPreview5 extends DogPreview_Base {};
class DogPreview6 extends DogPreview_Base {};
class DogPreview7 extends DogPreview_Base {};
class DogPreview8 extends DogPreview_Base {};
class DogPreview9 extends DogPreview_Base {};
class DogPreview10 extends DogPreview_Base {};
class DogPreview11 extends DogPreview_Base {};
class DogPreview12 extends DogPreview_Base {};
class DogPreview13 extends DogPreview_Base {};
class DogPreview14 extends DogPreview_Base {};
class DogPreview15 extends DogPreview_Base {};
class DogPreview16 extends DogPreview_Base {};
class DogPreview17 extends DogPreview_Base {};
class DogPreview18 extends DogPreview_Base {};
class DogPreview19 extends DogPreview_Base {};
class DogPreview20 extends DogPreview_Base {};
class DogPreview21 extends DogPreview_Base {};
class DogPreview22 extends DogPreview_Base {};
class DogPreview23 extends DogPreview_Base {};
class DogPreview24 extends DogPreview_Base {};
class DogPreview25 extends DogPreview_Base {};
class DogPreview26 extends DogPreview_Base {};
class DogPreview27 extends DogPreview_Base {};
class DogPreview28 extends DogPreview_Base {};
class DogPreview29 extends DogPreview_Base {};
class DogPreview30 extends DogPreview_Base {};
class DogPreview31 extends DogPreview_Base {};
class DogPreview32 extends DogPreview_Base {};
class DogPreview33 extends DogPreview_Base {};
class DogPreview34 extends DogPreview_Base {};
class DogPreview35 extends DogPreview_Base {};
