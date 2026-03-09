modded class PlayerBase
{
	protected Dayz_Doggo m_MyDog = NULL;
	protected int m_DogOwnerID;
	protected bool m_Whistle;
	protected bool m_Dance;
	protected bool m_PlayerPointing;
	protected vector m_DogSpawnPos;
	protected string m_ClientSteamID;
	protected bool m_DogAttLock;
	protected bool m_HiddenPlayer;
	protected bool m_HiddenPlayerClient;
	
	protected bool m_LockedTame;
	
	void PlayerBase()
	{
		m_Dance = false;
		m_DogAttLock = true;
	}
	
	override void Init()
	{
		super.Init();
		
		RegisterNetSyncVariableBool("m_Whistle");
		RegisterNetSyncVariableInt("m_DogOwnerID");
		RegisterNetSyncVariableBool("m_Dance");
		RegisterNetSyncVariableBool("m_HiddenPlayer");
	}
	
	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
		
		//let dog auto attack
		if( source.IsInherited(ZombieBase) )
		{
			if(m_MyDog && vector.Distance(this.GetPosition(), m_MyDog.GetPosition()) < 100)
			{
				
				m_MyDog.SetDoggoAttackTarget(source);
				m_MyDog.SetDoggoCmd(DoggoCmd.PATROL);
				/*
				if (m_MyDog.GetDoggoCmd() != DoggoCmd.FOLLOW)
				{
					Dayz_Doggo switch_dog = m_MyDog.zmb_atk_ownr(m_MyDog);
					m_MyDog = switch_dog;
					m_MyDog.SetOwnerId(m_DogOwnerID);
					m_MyDog.SetDoggoCmd(DoggoCmd.PATROL);
				}
				*/
			}
		}
		
		Dayz_Doggo dog = Dayz_Doggo.Cast(source);
		if (dog)
		{
			if (dog.GetOwnerId())
			{
				PlayerBase player = DoggoFunctions.GetPlayerByEntityID(dog.GetOwnerId());
				if (player)
				{
					EntityAI dogslot = player.FindAttachmentBySlotName("Dog");
					if (dogslot)
					{
						DogPreview_Base dogpr = DogPreview_Base.Cast(dogslot);
						dogpr.AddSurvivorHit();
					}
				}
			}
			
		}
		
	}
	
	void SetLockedTame( bool tame)
	{
		if (tame)
		{
			m_LockedTame = true;
			GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( UnlockTame, 30000, false );
		} else 
		{
			m_LockedTame = false;
		}
	}
	
	bool GetLockedTame()
	{
		return m_LockedTame;
	}
	
	void UnlockTame()
	{
		SetLockedTame(false);
	}
	
	/*
	override void EOnTouch( IEntity other, int extra )
	{
		super.EOnTouch(other, extra);
		dog_shed_small shed = dog_shed_small.Cast(other);
		DoggoLogger.Log("Dogshed touched " + other);
	}	
	*/
	override void EOnContact(IEntity other, Contact extra)
	{
		Dayz_Doggo dog = Dayz_Doggo.Cast(other);
		dog_shed_small shed = dog_shed_small.Cast(other);
		if (shed && GetGame().IsServer())
		{
			//DoggoLogger.Log("Dogshed touched");
			shed.SetDogOwnerID();
		}
		if (dog && dog.GetOwnerId() == this.GetID())
		{
			//dBodySetInteractionLayer(other, PhxInteractionLayers.RAGDOLL); //1.19 not needed anymore?
		}
		super.EOnContact(other, extra);
		// Print("CONTACT " + Object.Cast(other).GetModelName() );
		// Object.Cast(other).SetOrigin(vector.Zero); POSUN I S GEOM
		// Object.Cast(other).SetYawPitchRoll(Vector(Object.Cast(other).GetYawPitchRoll()[0],90,Object.Cast(other).GetYawPitchRoll()[2]));
	}
	
	override void EEItemAttached(EntityAI item, string slot_name)
	{
		super.EEItemAttached(item, slot_name);

		if (m_HiddenPlayer)
			HideBodyParts(m_HiddenPlayer);		
	}
	
	override bool CanJump()
	{
		if (m_HiddenPlayer)
			return false;
		return super.CanJump();
	}
	
	//Thx Zedmag and MDC
	void HideBodyParts(bool state)
    {    
		int decaySec = GetHiddenSelectionIndex("decay_preload"); //selection to be retextured
        SetObjectTexture(decaySec, ""); //engine sees the "" as a null image so it becomes invisiable in game.
		EntityAI bodypart;
        int slot_id;
        array<string> bodyparts = {"Gloves","Body","Legs","Feet","Head","Hips","pelvis","leftleg","rightleg","Back","Mask","Hands","HeadGear","Eyewear","Armband","Vest","Shoulder","Melee","Bow"};
        array<int> clothingArray = new array<int>;
		//clothingArray = {InventorySlots.BACK,InventorySlots.BODY,InventorySlots.VEST,InventorySlots.SHOULDER,InventorySlots.MELEE};
		//SetInvisibleRecursive(state, this, bodyparts);
		        
        for ( int i = 0; i < bodyparts.Count(); i++ )
        {
            
            slot_id = InventorySlots.GetSlotIdFromString(bodyparts.Get(i));
			bodypart = GetInventory().FindPlaceholderForSlot( slot_id );
            clothingArray.Insert(slot_id);
            if ( bodypart )
            {
                bodypart.SetInvisible( state );
                //RemovePhysics(state, bodyparts.Get(i));    //not working currently            
                //Print("[Prop_N_Hide] Hiding=<" + bodypart.GetType() + ">");
            }
			EntityAI gear = FindAttachmentBySlotName(bodyparts.Get(i));
			if (gear)
			{
				gear.SetInvisible( state );
			}
			EntityAI hands = GetItemInHands();
			if (hands)
			{
				hands.SetInvisible(state);
				//celou hiearchii je potreba
			}
        }
		SetInvisibleRecursive(state, this, clothingArray);
		m_HiddenPlayerClient = state;
           
    }
	
	void SetPlayerHidden(bool state)
	{
		if (GetGame().IsServer())
		{
			m_HiddenPlayer = state;
			SetSynchDirty();
		}
	}
	
	void HandleUnlink()
	{
		ActionManagerClient mngr_client = ActionManagerClient.Cast(this.GetActionManager());
		ActionTarget atrg;
		
		atrg = new ActionTarget(null,null,-1,vector.Zero,-1.0);
		if ( mngr_client.GetAction(ActionStopControlDog).Can(this,atrg,null) )
		{
			mngr_client.PerformActionStart(this.GetActionManager().GetAction(ActionStopControlDog),atrg,null);
		}
	}
	
  	override void SetActions() 
	{
		super.SetActions();
		AddAction(BBP_FollowMe);
		AddAction(BBP_StayHere);
		//AddAction(ActionDogWhistle); //removing
		AddAction(BBP_Patrol);
		AddAction(ActionFeedDog);
		AddAction(ActionSetDogMission);
		AddAction(ActionControlDog);
		AddAction(ActionStopControlDog);
		AddAction(ActionManageDog);
		//AddAction(ActionLickWound);
	}
	
	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();		
		//Print ("PLAYER VARIABLES SYNCH ");
		if (m_Whistle)
		{
			WhistleSoundPlay();
		}
		
		if (m_HiddenPlayer != m_HiddenPlayerClient)
		{
				HideBodyParts(m_HiddenPlayer);
		}
				
    }
	
	void WhistleSoundPlay()
	{
        EffectSound m_WhistleSound = SEffectManager.PlaySoundOnObject( "Whistle_SoundSet", this );
		//Print ("SOUND WHISTLE");
        //m_WhistleSound.SetMaxVolume(3.0);
		m_WhistleSound.SetSoundAutodestroy( true );
		ResetWhistle();
	}
	
	void BlowWhistle()
	{
		m_Whistle = true;
		SetSynchDirty();	
	}
	
	void ResetWhistle()
	{
		m_Whistle = false;
		SetSynchDirty();
	}

	void setMyDog(Dayz_Doggo target_dog)
	{
		m_MyDog = target_dog;
		//add to slot
		string dognum = target_dog.GetDogIndex().ToString();
		int slotId = InventorySlots.GetSlotIdFromString("Dog");
		EntityAI dogslot = FindAttachmentBySlotName("Dog");
		if (!dogslot)
		{
			DogPreview_Base preview = DogPreview_Base.Cast(this.GetInventory().CreateAttachmentEx("DogPreview"+dognum, slotId));
			preview.SetOwnerID(this.GetID());
			preview.SetOwnerSteamID(this.GetIdentity().GetPlainId());
			preview.SetDogHealth(target_dog);
			
		} else {
			DogPreview_Base.Cast(dogslot).SetOwnerID(this.GetID());
			DogPreview_Base.Cast(dogslot).SetOwnerSteamID(this.GetIdentity().GetPlainId());
			DogPreview_Base.Cast(dogslot).SetDogHealth(target_dog);
		}
		//m_HaveDog = true;
		SetSynchDirty();
	}
	
	Dayz_Doggo GetMyDog()
	{
		return m_MyDog;
	}
	

	void resetDog()
	{
		m_MyDog = NULL;
		EntityAI dogslot = FindAttachmentBySlotName("Dog");
		if (dogslot)
		{
			dogslot.DeleteSave();	
		}
		//m_HaveDog = false;
		//remove item
		SetSynchDirty();
	}
	
	bool HaveDog()
	{
		//get item
		//if variable set and item not it slot then add it
		EntityAI dogslot = FindAttachmentBySlotName("Dog");
		//Print("DOGSLOT " + dogslot );
		if (dogslot)
		{
			//m_HaveDog = true;
			return true;
		}
		return false;
		//return m_HaveDog;
	}
	
	void SetDogOwnerID(int owner)
	{
		m_DogOwnerID = owner;
		SetSynchDirty();
	}
	
	int GetDogOwnerID()
	{
		return m_DogOwnerID;
	}
	
	/*
	void SetDogIndex(int dogindex)
	{
		m_DogIndex = dogindex;
		SetSynchDirty();
	}
	*/
	
	int GetDogIndex()
	{
		EntityAI dogslot = FindAttachmentBySlotName("Dog");
		if (dogslot)
		{
			string dogpr = dogslot.GetType();
			
			TStringArray loc = new TStringArray;
			dogpr.Split( "w", loc );
			string numstr = loc.Get(1);
			return numstr.ToInt();
		}
		return 0;
	}
	
	vector GetDogLastPos()
	{
		EntityAI dogslot = FindAttachmentBySlotName("Dog");
		if (dogslot)
		{
			DogPreview_Base dogpr = DogPreview_Base.Cast(dogslot);
			return dogpr.GetLastDogPos();
		}
		return vector.Zero;
	}
	
	void SetDogLastPos(vector pos)
	{
		EntityAI dogslot = FindAttachmentBySlotName("Dog");
		if (dogslot)
		{
			DogPreview_Base dogpr = DogPreview_Base.Cast(dogslot);
			dogpr.SetLastDogPos(pos);
		}
	}
	
	void SetDogSpawnPos(vector pos)
	{
		m_DogSpawnPos = pos;
	}
	
	vector GetDogSpawnPos()
	{
		return m_DogSpawnPos;
	}
	
	EntityAI GetDogSlot()
	{
		EntityAI dogslot = FindAttachmentBySlotName("Dog");
		return dogslot;
	}
	
	override bool CanDropEntity(notnull EntityAI item) 
	{ 
		if (item.IsInherited(DogPreview_Base))
		{
			return false;
		}
		return super.CanDropEntity(item);
	}
	
	override bool CanReceiveAttachment(EntityAI attachment,int slotId)
	{
		//can receive only from attachment when you are owner of dog
		DogPreview_Base prev = DogPreview_Base.Cast(attachment);
		InventoryLocation il = new InventoryLocation();
		attachment.GetInventory().GetCurrentInventoryLocation(il);
		if (il.GetType() == InventoryLocationType.ATTACHMENT && prev && prev.GetParent() && dog_shed_small.Cast(prev.GetParent()) && dog_shed_small.Cast(prev.GetParent()).GetDogOwnerID() != this.GetDogOwnerID())
		{
			return false;
		}
		
		return super.CanReceiveAttachment(attachment, slotId);
	}
	
	void StoreDogHealth()
	{
		if (!GetGame().IsServer())
			return;
		EntityAI source = this.GetMyDog();
		EntityAI targetitem = this.GetDogSlot();
		if (targetitem && source && source.IsAlive())
		{
			DogPreview_Base.Cast(targetitem).SetDogHealth(source);
			DogPreview_Base.Cast(targetitem).StoreDogDamage(source);
		}

	}
	
	void RestoreDogHealth()
	{
		if (!GetGame().IsServer())
			return;
		EntityAI sourceitem = this.GetDogSlot();
		EntityAI target =  this.GetMyDog();
		if (target && target.IsAlive() && DogPreview_Base.Cast(sourceitem).GetDogHealth() > 0)
		{
			target.SetHealth( "", "" ,DogPreview_Base.Cast(sourceitem).GetDogHealth());
			DogPreview_Base.Cast(sourceitem).RestoreDogDamage(target);
		}

	}
	
	void StoreDogHealthTo( EntityAI targetitem)
	{
		if (!GetGame().IsServer())
			return;
		EntityAI source = this.GetMyDog();
		if (source && source.IsAlive())
		{
			DogPreview_Base.Cast(targetitem).SetDogHealth(source);
			DogPreview_Base.Cast(targetitem).StoreDogDamage(source);
		}

	}
	
	void StoreDogInventoryTo( EntityAI targetitem )
	{
		EntityAI sourceitem = this.GetMyDog();
		//targetitem.GetInventory().UnlockInventory(HIDE_INV_FROM_SCRIPT);
		if (sourceitem)
		{	
			CopyInventory(sourceitem,targetitem);
		}
		//targetitem.GetInventory().LockInventory(HIDE_INV_FROM_SCRIPT);  
	}
	
	void StoreDogInventory()
	{
		EntityAI sourceitem = this.GetMyDog();
		EntityAI targetitem = this.GetDogSlot();
		//DogPreview_Base slot = DogPreview_Base.Cast(targetitem);
		//slot.LockPreview(false);
		//targetitem.GetInventory().UnlockInventory(HIDE_INV_FROM_SCRIPT);
		//DoggoLogger.Log("Before Stored inventory size " + (targetitem.GetInventory().CountInventory() - 1).ToString() );
		if (sourceitem && targetitem)
		{
			CopyInventory(sourceitem,targetitem);
		}
		//slot.LockPreview(true);
		//DoggoLogger.Log("Stored inventory size " + (targetitem.GetInventory().CountInventory() - 1).ToString() );
		//targetitem.GetInventory().LockInventory(HIDE_INV_FROM_SCRIPT);
		
	}
	
	void RestoreDogInventory()
	{
		EntityAI targetitem = this.GetMyDog();
		EntityAI sourceitem = this.GetDogSlot();
		if (!targetitem || !sourceitem)
		{
			return;
		}
		//sourceitem.GetInventory().UnlockInventory(HIDE_INV_FROM_SCRIPT);
		CopyInventory(sourceitem,targetitem);
		//sourceitem.GetInventory().LockInventory(HIDE_INV_FROM_SCRIPT);
		GetGame().RemoteObjectTreeDelete(targetitem);
		GetGame().RemoteObjectTreeCreate(targetitem); 
	}
	void CopyInventory(EntityAI sourceItem, EntityAI targetItem)
	{
		//DoggoLogger.Log("source " + sourceItem + " TARGET " + targetItem);
		array<EntityAI> children = new array<EntityAI>;
		sourceItem.GetInventory().EnumerateInventory(InventoryTraversalType.LEVELORDER, children);
		int count = children.Count();
		float itemhealth;
		bool isruined, islocked;
		for (int i = 0; i < count; i++)
		{			
			EntityAI child = children.Get(i);
			if (child)
			{
				/*
				child.m_PendingDelete = false;
				child.m_PreparedToDelete = false;
				sourceItem.m_PendingDelete = false;
				sourceItem.m_PreparedToDelete = false;
				*/
				//DoggoLogger.Log("COPY entity " + child.GetType());
				isruined = false;
				islocked = false;
				if (child.IsRuined())
				{
					itemhealth = child.GetHealth("", "");
					child.SetHealthMax("","");
					isruined = true;
					
				}
				if (child.IsLockedInSlot())
				{
					//CCLogger.Log("child " + child + " is locked");
					islocked = true;
				}
					
				InventoryLocation child_src = new InventoryLocation;
				child.GetInventory().GetCurrentInventoryLocation( child_src );
				
				InventoryLocation child_dst = new InventoryLocation;
				child_dst.Copy( child_src );
				child_dst.SetParent( targetItem );

				if (GameInventory.LocationCanAddEntity(child_dst))
				{
					//DoggoLogger.Log("CAN COPY entity " + child.GetType());
					// try to move it to the same exact place in dst
					//targetItem.GetInventory().TakeToDst(InventoryMode.LOCAL, child_src, child_dst);
					targetItem.GetInventory().TakeToDst(InventoryMode.SERVER, child_src, child_dst);
					if (isruined)
					{
						child_dst.GetItem().SetHealth("", "", itemhealth);
					}
					if (islocked)
					{
						ItemBase.Cast(child_dst.GetItem()).LockToParent();
					}
				}

			}
		}
	}
	/*
	override bool CanReceiveAttachment(EntityAI attachment,int slotId)
	{
		return false;
	}
	*/
	void SetDance(bool dan)
	{
		m_Dance = dan;
		SetSynchDirty();
	}
	
	bool IsDancing()
	{
		return m_Dance;
	}
	
	// follow only owner player
	override bool CanBeTargetedByAI(EntityAI ai)
	{
		
		Dayz_Doggo dog = Dayz_Doggo.Cast(ai);
		#ifdef AI_BANDITS
		if (dog && dog.GetNPCOwner())
		{
			return true;
		}
		#endif
		if (dog && dog.GetOwnerId() == this.GetID()) /// != this.GetID() TESTING
		{
			return false;
		}
		DoggoConfig config = GetDayZGame().GetDayzDogConfig();
		if ((dog && dog.GetDoggoAttackTarget() != this) || (dog && dog.GetDoggoAttackTarget() == this && config.DisablePlayerAttack == true))
		{
			return false;
		}
		
		return super.CanBeTargetedByAI( ai );
	}
	
	// nastavit patrol ai hrace jako nepratele
	// tim vyse ovladat na koho zautocit
	
	void SetPointing(bool point)
	{
		m_PlayerPointing = point;
	}
	
	bool IsPointing()
	{
		return m_PlayerPointing;
	}
	
	/*
	void HandleFollowClient()
	{
		ActionManagerClient mngr_client = ActionManagerClient.Cast(this.GetActionManager());
		ActionTarget atrg;
		//Print("HANDLE FOLLOW");
		atrg = new ActionTarget(GetMyDog(),null,-1,vector.Zero,-1.0);
		
		if ( mngr_client.GetAction(BBP_FollowMe).Can(this,atrg,null) )
		{			
			mngr_client.PerformActionStart(this.GetActionManager().GetAction(BBP_FollowMe),atrg,null);
		}
	}
	*/
	void HandleFollow()
	{
		if (GetMyDog().GetDoggoCmd() == DoggoCmd.FOLLOW)
		{
			return;
		}
		Dayz_Doggo targetObject = GetMyDog();				
		vector targetObjectPos = targetObject.GetPosition();
		vector targetObjectOri = targetObject.GetOrientation();
		
		if (targetObject.GetType() == "Doggo_Follow"+this.GetDogIndex())
		{
			targetObject.SetDoggoCmd(DoggoCmd.FOLLOW);
			return;
		}
		/*
		targetObject.SetPosition(vector.Zero);
		GetGame().ObjectDelete( targetObject );

		Dayz_Doggo target_dog = Dayz_Doggo.Cast(GetGame().CreateObject("Doggo_Follow"+this.GetDogIndex(), targetObjectPos, false, true ));
		//target_dog.SetOrientation(targetObjectOri);
		target_dog.SetOwnerId(GetID());
		setMyDog(target_dog);
		target_dog.SetDoggoCmd(DoggoCmd.FOLLOW);
		//GetGame().RemoteObjectCreate( target_dog );
		*/
	}
	
	void HandleLayDown()
	{
		if (GetMyDog().GetDoggoCmd() == DoggoCmd.STAY)
		{
			return;
		}
		Dayz_Doggo targetObject = GetMyDog();				
		vector targetObjectPos = targetObject.GetPosition();
		
		if (targetObject.GetType() == "Doggo_Follow"+this.GetDogIndex())
		{
			targetObject.SetDoggoCmd(DoggoCmd.STAY);
			return;
		}
		/*
		targetObject.SetPosition(vector.Zero);
		GetGame().ObjectDelete( targetObject );

		Dayz_Doggo target_dog = Dayz_Doggo.Cast(GetGame().CreateObject("Doggo_Follow"+this.GetDogIndex(), targetObjectPos, false, true ));

		target_dog.SetOwnerId(GetID());
		setMyDog(target_dog);
		target_dog.SetDoggoCmd(DoggoCmd.STAY);
		*/
	}
	
	void HandlePatrol()
	{
		if (GetMyDog().GetDoggoCmd() == DoggoCmd.PATROL)
		{
			return;
		}
		Dayz_Doggo targetObject = GetMyDog();				
		vector targetObjectPos = targetObject.GetPosition();
		
		if (targetObject.GetType() == "Doggo_Follow"+this.GetDogIndex())
		{
			targetObject.SetDoggoCmd(DoggoCmd.PATROL);
			return;
		}
		/*
		targetObject.SetPosition(vector.Zero);
		GetGame().ObjectDelete( targetObject );

		Dayz_Doggo target_dog = Dayz_Doggo.Cast(GetGame().CreateObject("Doggo_Patrol"+this.GetDogIndex(), targetObjectPos, false, true ));

		target_dog.SetOwnerId(GetID());
		setMyDog(target_dog);
		target_dog.SetDoggoCmd(DoggoCmd.PATROL);
		*/
	}
	
	
	/*
	override bool OnStoreLoad(ParamsReadContext ctx, int version)
	{   
		if ( !super.OnStoreLoad(ctx, version) )
			return false;
		if ( GetDayZGame().IsServer() && GetDayZGame().IsMultiplayer() )
		{
			if (!ctx.Read(m_LastDogPos))
				return false;
		}
		return true;
	}
	
	override void OnStoreSave(ParamsWriteContext ctx)
	{
		super.OnStoreSave(ctx);
		if ( GetDayZGame().IsServer() && GetDayZGame().IsMultiplayer() )
		{
			ctx.Write(m_LastDogPos);
		}
		
	}
	*/
	
	#ifdef MAPLINK
	/*
	override void SendUApiAfterLoadClient()
	{
		super.SendUApiAfterLoadClient();
		if (HaveDog())
		{
			GetGame().ObjectDelete( this.GetMyDog());
			InitialDogSpawn(this.ModelToWorld("-0.5 0 1.5"));
		}		
	}*/
	#endif
	
	void SetDogAttLock(bool lock)
	{
		m_DogAttLock = lock;
	}
	
	/*
	override bool CanReleaseAttachment( EntityAI attachment )
	{
		if (attachment.IsKindOf("DogPreview_Base") && GetGame().IsClient())
		{
			return false; //!m_DogAttLock;
		}
		return super.CanReleaseAttachment( attachment );
		
	}
	*/
	override void OnCommandDeathStart()
	{	
		if (HaveDog())
		{
			resetDog();
		}
		super.OnCommandDeathStart();
	}
	
	override void OnConnect()
	{
		super.OnConnect();
		
		this.SetDogOwnerID(this.GetID());
		//DoggoLogger.Log("Player DOG OwnerID " + this.GetID());
		//GetRPCManager().SendRPC( "Dayz_Dog", "UpdateOwner", new Param1< bool >( true ) ); 
		/*
		DoggoConfig config = GetDayZGame().GetDayzDogConfig();
		private static FileHandle configFile;
		string DoggoconfDir = "$profile:\\Dayz-Dog\\players\\";
		if (config.StoreDogStatus && config.AllowOnlyStoredDogs)
		{
			if (!FileExist(DoggoconfDir + this.GetIdentity().GetPlainId() + ".json"))
			{
				resetDog();
			}
		}
		*/
		if (HaveDog())
		{
			//spawn dog
			DoggoLogger.Log("Player " + this.GetIdentity().GetName() + " have dog " + HaveDog() + " slot " + this.FindAttachmentBySlotName("Dog").GetType() + " getid " + this.GetID() + " dog index " + this.GetDogIndex());
			/*
			if (!m_DogIndex || m_DogIndex > NUM_OF_DOGS || m_DogIndex < 1)
			{
				this.SetDogIndex(1);
			}
			*/
			InitialDogSpawn(GetDogLastPos(), 3000);			
		}
	}
	void InitialDogSpawn(vector pos, int time=100)
	{
		
		Dayz_Doggo target_dog = Dayz_Doggo.Cast(GetGame().CreateObject("Doggo_Follow"+this.GetDogIndex(), pos, false, false ));
		//DoggoLogger.Log("DOGGO " + this.GetDogIndex() + " dd " + target_dog);
		AIGroup grp = GetGame().GetWorld().GetAIWorld().CreateGroup("DZDogGroupBeh");
		///AIGroup grp = GetGame().GetWorld().GetAIWorld().CreateGroup("Doggo_Follow_Old");
		//AIGroup grp = GetGame().GetWorld().GetAIWorld().CreateGroup("DZWolfGroupBeh");
		target_dog.InitAIAgent(grp);
		//DoggoLogger.Log("dog entity " + this.GetID());
		//target_dog.DestroyAIAgent();
		//target_dog.InitAIAgent(grp0);
		//DoggoLogger.Log("dog entity after " + this.GetID());
		//Trigger trigger = Trigger.Cast(GetGame().CreateObject("Trigger", vector.Zero));
		//target_dog.AddChild(trigger, -1);
		target_dog.SetOwnerId(this.GetID());
		setMyDog(target_dog);
		//RestoreDogInventory();
		target_dog.SetDoggoCmd(DoggoCmd.STAY);
		DoggoLogger.Log("Spawned dog entity ID " + target_dog.GetID());
		DoggoLogger.Log("Restored " + (this.GetDogSlot().GetInventory().CountInventory() - 1).ToString() + " items in dog inventory of Player " + this.GetIdentity().GetName());
		GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( RestoreDogInventory, time, false);
		RestoreDogHealth();
		//DoggoLogger.Log("Entity " + GetGame().GetWorld().FindEntityByName(null, "Doggo_Follow1"));
		
	}
	
	override void OnDisconnect()
	{
		super.OnDisconnect();
		/*
		if (HaveDog() && m_MyDog)
		{
			SetDogLastPos(m_MyDog.GetPosition());
			m_MyDog.SetDoggoCmd(DoggoCmd.STAY);
			DoggoLogger.Log("Stored " + (m_MyDog.GetInventory().CountInventory() - 1).ToString() + " items in dog inventory of Player " + this.GetIdentity().GetName());
			StoreDogInventory();
			StoreDogHealth();
			GetGame().ObjectDelete( m_MyDog );
		}
		*/
	}
	
}
