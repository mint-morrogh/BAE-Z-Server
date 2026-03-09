enum DoggoCmd
{
	WILD,
	TAME,
	MOVE,
	STAY,
	FOLLOW,
	PATROL,
	SITDOWN,
	EATING,
	DANCING,
	PLRCONTROL,
	MISSION
}

class Dayz_Doggo extends AnimalBase
{
	protected int m_DogOwner;
	protected int m_DogIndex;
	
	private ref DayzDogST m_AnimST;
	
	private DayzDogCommandMovement m_Movement;
	private DayzDogCommandStay m_Stay;
	private DayzDogCommandSitting m_Sitting;
	private DayzDogCommandDance m_Dancing;
	private DayzDogCommandEating m_Eating;
	private DayzDogCommandTaming m_Taming;
	private DayzDogCommandControl m_CommandControl;
	
	private ref PGFilter m_PathFilter;
	private ref TVectorArray m_PathWaypoints;
	
	protected AIAgent m_AIAgent;
	protected int m_DoggoCmd;
	protected int m_DogTameID;
	protected EntityAI m_DogAttackTarget;
	protected bool m_IsDogWounded;
	protected bool m_DogAttackLow;
	protected bool m_DogGrowl;
	protected bool m_DogEmoteControl;
	
	protected bool m_IsControlled;
	
	ref array<ref BehaviourGroupInfectedPackWaypointParams> m_WaypointParams = new array<ref BehaviourGroupInfectedPackWaypointParams>();
	protected DoggoCloseTrigger m_DoggoClosetrigger;
	protected DoggoFarTrigger m_DoggoFartrigger;
	
	protected vector m_DoggoWaypoint;
	
	protected int m_TimeInWater;
	
	protected ref array<Object> m_AllTargetObjects;
	protected ref array<typename>m_TargetableObjects;
	#ifdef AI_BANDITS
	protected InfectedBanditBase m_NPCOwner;
	#endif
	//protected float         m_Scale = 0.6;
	void Dayz_Doggo()
	{
		RegisterNetSyncVariableInt("m_DogOwner");
		m_DogIndex = 0;
		m_DogOwner = 0;
		m_PathWaypoints = new TVectorArray();
		m_DogEmoteControl = false;
		//SetEventMask(EntityEvent.POSTFRAME);
		SetEventMask(EntityEvent.INIT);
        //RegisterNetSyncVariableFloat("m_Scale");
		RegisterNetSyncVariableInt("m_DoggoCmd");
		RegisterNetSyncVariableBool("m_IsDogWounded");
		RegisterNetSyncVariableBool("m_IsControlled");
		RegisterNetSyncVariableBool("m_DogGrowl");
		RegisterNetSyncVariableBool("m_DogEmoteControl");
		
		
		m_AllTargetObjects		= new array<Object>;
		m_TargetableObjects 	= new array<typename>;
		//m_TargetableObjects.Insert(PlayerBase);
		m_TargetableObjects.Insert(AnimalBase);
		m_TargetableObjects.Insert(ZombieBase);
		//
		//m_AnimST = new DayzDogST( this );
		//DoggoLogger.Log("BONE " + GetBoneIndex("head"));
		// bone 16 is head
		//ItemBase item = ItemBase.Cast(GetGame().CreateObject("dog_shed_small_kit", Vector(0, 0.5, 0.5)));
		//this.AddChild(item,17, false);
	}
	
	void ~Dayz_Doggo()
	{
		//GetGame().ObjectDelete(m_DoggoClosetrigger);
	}

	/*
	override void EEItemAttached( EntityAI item, string slot_name ) 
	{
		super.EEItemAttached( item, slot_name );
		this.AddChild(item, 25, false);	
	}
	*/
	
	void AttachPlayer(PlayerBase player)
	{
		Object obj = this;

		vector tmPlayer[4]; 
		vector tmTarget[4];
		vector tmLocal[4];
		player.GetTransform( tmPlayer );	
		obj.GetTransform( tmTarget );
		tmPlayer[3] = tmTarget[3] + Vector(0, 0.0, -1);
		Math3D.MatrixInvMultiply4( tmTarget, tmPlayer, tmLocal );
		if (!player.GetParent())
		{
			player.LinkToLocalSpaceOf( obj, tmLocal );
			dBodyEnableGravity(player, false);
			this.SetControlled(true);
		}
	}
	
	bool IsControlled()
	{
		return m_IsControlled;
	}
	
	void SetControlled(bool ctr)
	{
		m_IsControlled = ctr;
		SetSynchDirty();
	}
	
	bool CanListenEmotes()
	{
		return m_DogEmoteControl;
	}
	
	void SetListenEmotes(bool listen)
	{
		m_DogEmoteControl = listen;
		SetSynchDirty();
	}
	
	override void EOnInit( IEntity other, int extra )
	{
		//jen pro wild dogy
		super.EOnInit(other, extra);
		if (this.IsWild())
		{
			DoggoLogger.Log("Spawned Wild Dog at " + GetPosition());
		} else {
			SetListenEmotes(false);
			
			m_DoggoClosetrigger = DoggoCloseTrigger.Cast(GetGame().CreateObject("DoggoCloseTrigger", vector.Zero));
			m_DoggoClosetrigger.SetParentDog(this);
			this.AddChild(m_DoggoClosetrigger, -1);
			
			m_DoggoFartrigger = DoggoFarTrigger.Cast(GetGame().CreateObject("DoggoFarTrigger", vector.Zero));
			m_DoggoFartrigger.SetParentDog(this);
			this.AddChild(m_DoggoFartrigger, -1);
			
			m_DogAttackLow = false;
			//DoggoLogger.Log("BONE1 " + this.GetBoneIndex( "Head" ) + " " + this.GetBoneObject( this.GetBoneIndex( "Head" ) ));
			//DoggoLogger.Log("BONE2 " + this.GetBoneIndex( "Spine1" ) );
			//DoggoLogger.Log("BONE3 " + this.GetBoneIndex( "Spine2" ) );
			//ItemBase item = ItemBase.Cast(GetGame().CreateObject("dog_shed_small_kit", vector.Zero));
			//this.AddChild(item,17);
			
		}
		/*
		PlayerBase player = GetPlayerByEntityID(this.GetOwnerId());
		m_AIAgent = this.GetAIAgent();
		BehaviourGroupInfectedPack bgip;
		Class.CastTo( bgip, m_AIAgent.GetGroup().GetBehaviour() );
		if (bgip)
		{
			m_WaypointParams.Insert( BehaviourGroupInfectedPackWaypointParams( player.ModelToWorld("0 0 20"), 5 ) );
			m_WaypointParams.Insert( BehaviourGroupInfectedPackWaypointParams( player.ModelToWorld("-20 0 0"), 5 ) );
			m_WaypointParams.Insert( BehaviourGroupInfectedPackWaypointParams( player.ModelToWorld("0 0 -20"), 5 ) );
			m_WaypointParams.Insert( BehaviourGroupInfectedPackWaypointParams( player.ModelToWorld("20 0 0"), 5 ) );
						
			//waypointParams.Insert( BehaviourGroupInfectedPackWaypointParams( player.GetPosition(), 10 ) );
			bgip.SetWaypoints( m_WaypointParams, 0, true, true );
		}	
		*/		
	}
	
	void CheckAroundDog()
	{
		if (GetGame().IsServer() && this.IsAlive())
		{
			array<Object> objects_in_vicinity = new array<Object>;
			DayZPlayerUtils.GetEntitiesInCone( this.GetPosition(), "0 0 1", 30, 2, 0, 1, objects_in_vicinity);
			//DayZPlayerUtils.GetEntitiesInCone( this.GetBonePositionWS(this.GetBoneIndexByName("head")), "0 0 1", 90, 2, 0, 1, objects_in_vicinity);
			//Print("vicinity " + objects_in_vicinity.Count());
			for ( int k = 0; k < objects_in_vicinity.Count(); k++ )
			{
				EntityAI entity_in_cone = EntityAI.Cast(objects_in_vicinity.Get(k));
				if ((this.GetDoggoCmd() == DoggoCmd.PATROL || this.GetDoggoCmd() == DoggoCmd.FOLLOW) && entity_in_cone && (entity_in_cone.IsInherited(DeadChicken_ColorBase)||entity_in_cone.IsInherited(dog_bone_toy) ))
				{
					int slot = InventorySlots.GetSlotIdFromString( "DogMouth" );
					//this.PredictiveTakeEntityAsAttachmentEx( entity_in_cone, slot );
					EntityAI mouth_ent = FindAttachmentBySlotName("DogMouth");
					EntityAI mask_ent = FindAttachmentBySlotName("DogMask");
					InventoryLocation child_src = new InventoryLocation;
					
					if (!mouth_ent && !mask_ent && entity_in_cone.GetInventory() && entity_in_cone.GetInventory().GetCurrentInventoryLocation( child_src ))
					{
						this.GetInventory().TakeEntityAsAttachment(InventoryMode.SERVER, entity_in_cone);
						if (entity_in_cone.IsInherited(dog_bone_toy))
						{
							this.SetDoggoCmd(DoggoCmd.FOLLOW);
							this.SetDoggoAttackTarget(null);
						}
					}
					//DoggoLogger.Log("Slot "+ slot + " Entity " + entity_in_cone.GetType());
				}
				else if ((this.GetDoggoCmd() == DoggoCmd.PATROL || this.GetDoggoCmd() == DoggoCmd.FOLLOW) && entity_in_cone && (entity_in_cone.IsInherited(Animal_GallusGallusDomesticus)))
				{
					this.SetDogAttackLow(true);
				}
			}
		}
		
	}
	/*
	override void OnBeforeTryDelete() 
	{
		PlayerBase player = GetPlayerByEntityID(this.GetOwnerId());
		if (player && GetGame().IsServer())
		{
			player.StoreDogInventory();
			player.StoreDogHealth();
		}
	}
	*/
	/*
	override void EEDelete(EntityAI parent)
	{
		//GetGame() can be sometimes NULL when turning off server
		
		if ( GetGame() && m_DoggoClosetrigger )
		{
			GetGame().ObjectDelete( m_DoggoClosetrigger );
			m_DoggoClosetrigger = NULL;
		}
		if ( GetGame() && m_DoggoFartrigger )
		{
			GetGame().ObjectDelete( m_DoggoFartrigger );
			m_DoggoFartrigger = NULL;
		}
		
		super.EEDelete(parent);
	}
	*/
	override bool IsInventoryVisible()
	{
		if (this.GetDoggoCmd() == DoggoCmd.STAY || this.GetDoggoCmd() == DoggoCmd.SITDOWN || !this.IsAlive())
		{
			return true;
		}
		//return true; // TEMPORARY
		return false;
	}
	
	void SetStartEating(EntityAI ent, PlayerBase player, int prevcmd)
	{
		this.SetDoggoCmd(DoggoCmd.EATING);
		GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( DeleteEntity, 19500, false, ent, player, prevcmd );
	}

	void DeleteEntity(EntityAI ent, PlayerBase player, int prevcmd)
	{
		if (ent)
		{
			ent.DeleteSave();
		}
		//SetDoggoCmd(DoggoCmd.MOVE);
		// jidlo vyleci nastavit mu max health
		//DoggoLogger.Log("END EATING " + this.GetDoggoCmd());
		if (this.IsWild() && !player.HaveDog())
		{
			this.SetDoggoCmd(DoggoCmd.TAME);
			GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( EndTameAnim, 9000, false, player );
		} else {
			if (this.IsAlive())
			{
				this.AddHealth("", "", 40);
				this.AddHealth("", "Blood", 100);
				//SetDoggoCmd(DoggoCmd.MOVE);
				this.SetDoggoCmd(prevcmd);
				PlayerBase plr = GetPlayerByEntityID(this.GetOwnerId());
				if (plr)
				{
					plr.StoreDogHealth();
				}
			}
			
		}
		
		
	}
	
	void EndTameAnim(PlayerBase player)
	{
		//zavolat smazani psa a spawnout followera
		if (this.IsAlive())
		{
			DogServerRPC.SpawnTamedDog(player, this);
		}
		
	}
	
	override bool CanReceiveAttachment(EntityAI attachment,int slotId)
	{
		//prevent attach mask when mouth is not empty and prevent attach to mouth when mask weared
		int mouth = InventorySlots.GetSlotIdFromString( "DogMouth" );
		int mask = InventorySlots.GetSlotIdFromString( "DogMask" );
		EntityAI mouth_ent = FindAttachmentBySlotName("DogMouth");
		EntityAI mask_ent = FindAttachmentBySlotName("DogMask");
		if ((mask_ent && slotId == mouth) || (mouth_ent && slotId == mask))
		{
			return false;
		}
		
		return super.CanReceiveAttachment(attachment, slotId);
	}
	
	override bool ResistContaminatedEffect()
	{
		//dog mask set resistance to contaminated effect
		EntityAI mask_ent = FindAttachmentBySlotName("DogMask");
		if (mask_ent)
		{
			return true;
		}
		return false;
	}
	
	void SetDoggoCmd(int doggocmd)
	{
		m_DoggoCmd = doggocmd;
		SetSynchDirty();
	}
	
	int GetDoggoCmd()
	{
		return m_DoggoCmd;
	}
	
	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();		
		if (m_DogGrowl && IsAlive())
		{
			GrowlSoundPlay();
		}
	}
	
	void GrowlSoundPlay()
	{
        EffectSound m_GrowlSound = SEffectManager.PlaySoundOnObject( "WolfGrowl_SoundSet", this );
		m_GrowlSound.SetSoundAutodestroy( true );
	}
	
	void StartGrowl()
	{
		m_DogGrowl = true;
		SetSynchDirty();	
	}
	
	void ResetGrowl()
	{
		m_DogGrowl = false;
		SetSynchDirty();
	}
	
	void SetDoggoWaypoint(vector waypoint)
	{
		m_DoggoWaypoint = waypoint;
	}
	
	vector GetDoggoWaypoint()
	{
		return m_DoggoWaypoint;
	}
	
	void SetDogAttackLow(bool attacktype)
	{
		m_DogAttackLow = attacktype;
	}
	
	bool ShouldDogAttackLow()
	{
		return m_DogAttackLow;
	}
	
	void SetDogWounded()
	{
		if (GetGame().IsServer() && this.GetHealth("", "") < this.GetMaxHealth("", ""))
		{
			m_IsDogWounded = true;
		}
		else
		{
			m_IsDogWounded = false;
		}
		SetSynchDirty();
	}
	
	bool IsDogWounded()
	{
		return m_IsDogWounded;
	}
	
	void SetDoggoAttackTarget(EntityAI target)
	{
		m_DogAttackTarget = target;
	}
	
	EntityAI GetDoggoAttackTarget()
	{
		return m_DogAttackTarget;
	}
	DayzDogST GetAnimST()
	{
		return m_AnimST;
	}

	DayzDogCommandMovement StartCommand_DogMovement()
	{
		if ( !m_AnimST )
			m_AnimST = new DayzDogST( this );

		m_Movement = DayzDogCommandMovement.Cast( StartCommand_Script( new DayzDogCommandMovement( this ) ) );
		
		return m_Movement;
	}
	
	DayzDogCommandStay StartCommand_DogStay()
	{
		if ( !m_AnimST )
			m_AnimST = new DayzDogST( this );

		DayzDogCommandStay cmd = new DayzDogCommandStay(this);
		m_Stay = DayzDogCommandStay.Cast( StartCommand_Script( cmd ) );
		
		return m_Stay;
	}
	
	DayzDogCommandSitting StartCommand_DogSit()
	{
		if ( !m_AnimST )
			m_AnimST = new DayzDogST( this );

		m_Sitting = DayzDogCommandSitting.Cast( StartCommand_Script( new DayzDogCommandSitting( this ) ) );
		
		return m_Sitting;
	}
	
	DayzDogCommandDance StartCommand_DogDance()
	{
		if ( !m_AnimST )
			m_AnimST = new DayzDogST( this );

		m_Dancing = DayzDogCommandDance.Cast( StartCommand_Script( new DayzDogCommandDance( this ) ) );
		
		return m_Dancing;
	}
	
	DayzDogCommandEating StartCommand_DogEating()
	{
		if ( !m_AnimST )
			m_AnimST = new DayzDogST( this );

		m_Eating = DayzDogCommandEating.Cast( StartCommand_Script( new DayzDogCommandEating( this ) ) );
		
		return m_Eating;
	}
	
	DayzDogCommandTaming StartCommand_DogTaming()
	{
		if ( !m_AnimST )
			m_AnimST = new DayzDogST( this );

		m_Taming = DayzDogCommandTaming.Cast( StartCommand_Script( new DayzDogCommandTaming( this ) ) );
		
		return m_Taming;
	}
	
	DayzDogCommandControl StartCommand_DogControl()
	{
		if ( !m_AnimST )
			m_AnimST = new DayzDogST( this );

		m_CommandControl = DayzDogCommandControl.Cast( StartCommand_Script( new DayzDogCommandControl( this ) ) );
		
		return m_CommandControl;
	}
	//Thx Ralian and Jacob
	void UpdatePath( vector position, PGFilter filter )
	{
		m_PathFilter = filter;
		
		m_PathWaypoints.Clear();
		
		AIWorld world = GetGame().GetWorld().GetAIWorld();

		world.FindPath( GetPosition(), position, m_PathFilter, m_PathWaypoints );
	}

	float Distance( int index, vector position )
	{
		vector b = m_PathWaypoints[index];
		vector e = m_PathWaypoints[index + 1] - b;
		vector p = position - b;
		float eSize2 = e.LengthSq();
		if ( eSize2 > 0 )
		{
			float t = ( e * p ) / eSize2;
			vector nearest = b + Math.Clamp( t, 0, 1 ) * e;
			return vector.DistanceSq( nearest, position );
		} else
		{
			return vector.DistanceSq( b, position );
		}
	}

	int PathCount()
	{
		return m_PathWaypoints.Count();
	}
	
	vector PathGet( int index )
	{
		return m_PathWaypoints[ index ];
	}

	int FindNext( vector position )
	{
		float dist;
		return FindNext( position, dist );
	}
	
	bool PathBlocked( vector start, vector end )
	{
		vector hitPos;
		vector hitNormal;
		
		m_PathFilter = new PGFilter();

		int inFlags = PGPolyFlags.WALK | PGPolyFlags.DOOR | PGPolyFlags.INSIDE | PGPolyFlags.JUMP_OVER | PGPolyFlags.SWIM | PGPolyFlags.SWIM_SEA ;
		int exFlags = PGPolyFlags.DISABLED | PGPolyFlags.JUMP | PGPolyFlags.CLIMB | PGPolyFlags.CRAWL | PGPolyFlags.CROUCH;

		m_PathFilter.SetFlags( inFlags, exFlags, PGPolyFlags.NONE );
		m_PathFilter.SetCost( PGAreaType.JUMP, 0.0 );
		m_PathFilter.SetCost( PGAreaType.FENCE_WALL, 0.0 );
		m_PathFilter.SetCost( PGAreaType.WATER, 1.0 );
		m_PathFilter.SetCost( PGAreaType.WATER_DEEP, 1.0);
		m_PathFilter.SetCost( PGAreaType.WATER_SEA, 1.0);
		m_PathFilter.SetCost( PGAreaType.WATER_SEA_DEEP, 1.0);
		
		return GetGame().GetWorld().GetAIWorld().RaycastNavMesh( start, end, m_PathFilter, hitPos, hitNormal );
	}

	int FindNext( vector position, out float minDist )
	{
		int index = 0;
		minDist = 1000000000.0;

		float epsilon = 0.1;
		for ( int i = 0; i < m_PathWaypoints.Count() - 1; ++i )
		{
			float dist = Distance( i, position );
			if ( minDist >= dist - epsilon )
			{
				if ( !PathBlocked( position, m_PathWaypoints[i + 1] ) )
				{
					minDist = dist;
					index = i;
				}
			}
		}
		
		return index + 1;
	}

	vector PointOnPathXZ( vector begin, vector end, vector position )
	{
		vector eb = end - begin;
		vector pb = position - begin;
		eb[1] = 0;
		pb[1] = 0;

		float denom2 = eb.LengthSq();
		if ( denom2 <= 0 )
			return begin;

		float t = ( eb * pb ) / denom2;
		return begin + ( eb * Math.Clamp( t, 0, 1 ) );
	}

	vector PointOnPathXZ( vector position )
	{
		int index;
		float dist;
		return PointOnPathXZ( position, index, dist );
	}

	vector PointOnPathXZ( vector position, out int index, out float dist )
	{
		if ( m_PathWaypoints.Count() < 2 )
			return m_PathWaypoints[ m_PathWaypoints.Count() - 1 ];
		
		index = FindNext( position, dist );
		if ( index <= 0 )
		{
			return m_PathWaypoints[ m_PathWaypoints.Count() - 1 ];
		}

		return PointOnPathXZ( m_PathWaypoints[index], m_PathWaypoints[index - 1], position );
	}
	
	vector ClipToPath( vector newPos, vector oldPos, vector nearestNewPos, vector nearestOldPos )
	{
		if ( Math.AbsFloat( nearestNewPos[1] - newPos[1] ) < 0.5 && Math.AbsFloat( nearestOldPos[1] - oldPos[1] ) < 0.5 )
		{
			float distNew2 = vector.Distance( MakeXZ( nearestNewPos ), MakeXZ( newPos ) );
			float distOld2 = vector.Distance( MakeXZ( nearestOldPos ), MakeXZ( oldPos ) );
			
			float maxDist = 10 * 0.5;
			if ( distNew2 > maxDist * maxDist && distNew2 > distOld2)
			{
				vector limitedPos = ( newPos - nearestNewPos ) * ( 1.0 / Math.Sqrt( distNew2 ) ) * maxDist + nearestNewPos;
				return limitedPos - newPos;
			}
		}
		return vector.Zero;
	}

	vector MakeXZ( vector i )
	{
		return Vector( i[0], 0, i[2] );
	}

	float AngleBetweenPoints( vector pos1, vector pos2 )
	{
		return vector.Direction( pos1, pos2 ).Normalized().VectorToAngles()[0];
	}

	float AngleBetweenPointsXZ( vector pos1, vector pos2 )
	{
		return vector.Direction( MakeXZ( pos1 ), MakeXZ( pos2 ) ).Normalized().VectorToAngles()[0];
	}
	
	bool ShouldSwim()
	{
		float tobottom = GetPosition()[1] - GetGame().SurfaceY(GetPosition()[0],GetPosition()[2]) ;
		if ((GetGame().GetWaterDepth( GetPosition() ) > 0.5 && tobottom > 0.8) || GetGame().GetWaterDepth( GetPosition() ) > 0.65 )
		{
			m_TimeInWater++;
			return true;
		}
		m_TimeInWater = 0;
		return false;
	}
	
	protected float BuoyancyForce(vector position, vector velocity)
	{
		float bForce;
		float downForce = 0.5;
		float offset = 0.1; //0.4
		
		float depth = GetGame().GetWaterDepth( position ) + offset;
		//predelat podle ponoreni
		float forceFactor = 1.0 - ( depth / 0.5 );
		forceFactor = -forceFactor;
		if ( forceFactor > 0 )
		{
			bForce = 9.81 * ( forceFactor - velocity[1] * 0.5 );
			bForce += -downForce;
			
		}			
		return bForce;
	}
	
	override bool ModCommandHandlerAfter(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)
	{
		if (super.ModCommandHandlerAfter(pDt, pCurrentCommandID, pCurrentCommandFinished))
		{
			return true;
		}
		if (GetGame().IsClient() || !GetGame().IsMultiplayer())
		{
			return true;
		}
		PlayerBase player = GetPlayerByEntityID(this.GetOwnerId());
		float plrspeed;
		vector plrdirection;
		HumanInputController hic;
		DayZAnimalInputController inputController = GetInputController();
		if (inputController.IsDead())
		{
			return true;
		}
		//chickens
		CheckAroundDog();
		//DoggoLogger.Log(" LIFETIME " + this.GetEconomyProfile().GetLifetime());
		float buoyancyValue = BuoyancyForce(GetPosition(), GetVelocity(this));
		vector transform[4];
		this.GetTransform( transform );
		
		vector impulse;
		vector linImpulseMS; // model space
		vector linImpulseWS = vector.Zero;
		SetDogWounded();
		/*
		if (ShouldEat())
		{
			SetDoggoCmd(DoggoCmd.EATING);
		}
		*/
		m_AIAgent = this.GetAIAgent();
		//DoggoLogger.Log("GROUP " + m_AIAgent.GetGroup() + " BEHAVIOUR " + m_AIAgent.GetGroup().GetBehaviour());
		//DoggoLogger.Log("DOG CMD HANDLER " + this.GetDoggoCmd());
		//DoggoLogger.Log("DOG CURRENT CMD ID " + pCurrentCommandID);
		BehaviourGroupInfectedPack bgip;
		/*
		BehaviourGroupPredators bpr;
		Class.CastTo( bpr, m_AIAgent.GetGroup().GetBehaviour() );
		if (bpr)
		{
			DoggoLogger.Log(" BEHAVIOUR Predator" );
		}
		*/
		//inputController.OverrideBehaviourSlot(true, DayZAnimalBehaviourSlot.ENRAGED);
		//FUNGUJE
		//inputController.OverrideBehaviourAction(true, DayZAnimalBehaviourAction.CHARGING);
		//DoggoLogger.Log("ALERT LEVEL " + inputController.GetAlertLevel() + " BEHAVIOUR SLOT " + inputController.GetBehaviourSlot() + " BEHAVIOUR ACTION " + inputController.GetBehaviourAction());
		if (m_AIAgent && m_AIAgent.GetGroup() && m_AIAgent.GetGroup().GetBehaviour())
		{
			Class.CastTo( bgip, m_AIAgent.GetGroup().GetBehaviour() );
		}
		SetLifetime(360000);
		if (bgip)
		{
			//DoggoLogger.Log(" BEHAVIOUR GroupInfectedPack" ); //MUSIME WAYPOINTY ULOZIT
			m_WaypointParams.Clear();
			//ref array<ref BehaviourGroupInfectedPackWaypointParams> waypointParams = new array<ref BehaviourGroupInfectedPackWaypointParams>();
			#ifdef AI_BANDITS
			if (GetNPCOwner())
			{
				inputController.OverrideBehaviourAction(true, DayZAnimalBehaviourAction.CHARGING);
				m_WaypointParams.Insert( BehaviourGroupInfectedPackWaypointParams( GetNPCOwner().ModelToWorld("-0.5 0 10.5"), 8 ) );
				SetDoggoWaypoint(GetNPCOwner().ModelToWorld("-0.5 0 10.5"));
				bgip.SetWaypoints( m_WaypointParams, 0, true, false );	
			}
			#endif
			if (player)
			{
				if (this.GetDoggoCmd() == DoggoCmd.PATROL)
				{
					if (ShouldSwim() || ( GetDoggoAttackTarget() && GetDoggoAttackTarget().IsInherited(dog_bone_toy)))
					{
						inputController.OverrideBehaviourAction(false, DayZAnimalBehaviourAction.CHARGING);
						inputController.OverrideBehaviourAction(true, DayZAnimalBehaviourAction.TRAVELING_INPUT);
					}
					else
					{
						inputController.OverrideBehaviourAction(true, DayZAnimalBehaviourAction.CHARGING);
					}
					if (GetDoggoAttackTarget() != null)
					{
						//DoggoLogger.Log("Entity " + GetDoggoAttackTarget() );
						if (GetDoggoAttackTarget().IsAlive())
						{
							m_WaypointParams.Insert( BehaviourGroupInfectedPackWaypointParams( GetDoggoAttackTarget().ModelToWorld("0 0 0.8"), 3 ) );
							SetDoggoWaypoint(GetDoggoAttackTarget().ModelToWorld("0 0 0.8"));
						}
						else
						{
							SetDoggoAttackTarget(null);
							m_WaypointParams.Insert( BehaviourGroupInfectedPackWaypointParams( player.ModelToWorld("-0.5 0 20.5"), 8 ) );
							SetDoggoWaypoint(player.ModelToWorld("-0.5 0 20.5"));
						}
						
					} else {
						m_WaypointParams.Insert( BehaviourGroupInfectedPackWaypointParams( player.ModelToWorld("-0.5 0 20.5"), 8 ) );
						SetDoggoWaypoint(player.ModelToWorld("-0.5 0 20.5"));
					}
					
					//waypointParams.Insert( BehaviourGroupInfectedPackWaypointParams( player.GetPosition(), 5 ) );
					bgip.SetWaypoints( m_WaypointParams, 0, true, false );	
				}
				else if (this.GetDoggoCmd() == DoggoCmd.FOLLOW)
				{
					//inputController.OverrideBehaviourAction(false, DayZAnimalBehaviourAction.CHARGING);
					inputController.OverrideBehaviourAction(true, DayZAnimalBehaviourAction.TRAVELING_INPUT);
					m_WaypointParams.Insert( BehaviourGroupInfectedPackWaypointParams( player.ModelToWorld("-0.5 0 1.5"), 3 ) );
					bgip.SetWaypoints( m_WaypointParams, 0, true, false );
					SetDoggoAttackTarget(null);
					SetDogAttackLow(false);	
					SetDoggoWaypoint(player.ModelToWorld("-0.5 0 1.5"));
				}
				else if (this.GetDoggoCmd() == DoggoCmd.MISSION)
				{
					inputController.OverrideBehaviourAction(false, DayZAnimalBehaviourAction.CHARGING);
					m_WaypointParams.Insert( BehaviourGroupInfectedPackWaypointParams( m_DoggoWaypoint, 5 ) );
					bgip.SetWaypoints( m_WaypointParams, 0, true, false );
					SetDoggoAttackTarget(null);
					if 	(vector.Distance(this.GetPosition(), m_DoggoWaypoint ) <= 5)
					{
						NotificationSystem.SendNotificationToPlayerExtended(player, 2, "#STR_DD_mission", "#STR_DD_mission_info", "set:dayzdog_gui image:dog_stay");
						this.SetDoggoCmd(DoggoCmd.STAY);
					}
				}
				else if (this.GetDoggoCmd() == DoggoCmd.STAY)
				{
					inputController.OverrideBehaviourAction(false, DayZAnimalBehaviourAction.CHARGING);
					m_WaypointParams.Clear();
					bgip.SetWaypoints( m_WaypointParams, 0, true, false );
					SetDoggoAttackTarget(null);
					SetDogAttackLow(false);
				}
				//bgip.SetWaypoints( m_WaypointParams, 0, true, false );
				//DoggoLogger.Log(" waypointParams " + waypointParams);
				if (this.GetPosition()[1] < GetGame().SurfaceY(this.GetPosition()[0], this.GetPosition()[2])+5)
				{
					//dBodyApplyImpulse( this, "0 1 0".ToVector() * 20);
				}
				
				impulse = Vector( 0, buoyancyValue , 0 ) * dBodyGetMass( this )* pDt;
				linImpulseMS += impulse;
				linImpulseWS = linImpulseMS.Multiply3( transform );
				if (GetGame().GetWaterDepth( this.GetPosition() ) > 0.8 && m_TimeInWater > 40)
				{
					dBodyApplyImpulse( this, linImpulseWS );
				}
				
				
			}
			
		}
		//DoggoLogger.Log("AI AGENT " + m_AIAgent.GetGroup() + " beh " + m_AIAgent.GetGroup().GetBehaviour() );
		//BehaviourGroupInfectedPack testik = BehaviourGroupInfectedPack.Cast(m_AIAgent.GetGroup().GetBehaviour());
		//DoggoLogger.Log("AI TYPE " + GetCreatureAIType() );
		// dog ve follow modu, jen budu menit cile a pak spustit attack
		/*
		EntityAI vest = FindAttachmentBySlotName("DogVest");
		if (vest && vest.GetEconomyProfile())
		{
		
			//DoggoLogger.Log("Vest lifetime " + vest.GetEconomyProfile().GetLifetime());
		}
		*/
		
		if (FightAttackLogic( pCurrentCommandID, inputController, pDt))
		{
			return true;
		}
		
		if ( pCurrentCommandID == DayZAnimalConstants.COMMANDID_SCRIPT )
		{
			//DoggoLogger.Log("SCRIPT_CMD ID " + this.GetCommand_Script());
			DayZAnimalCommandScript acs = GetCommand_Script();
			if ( DayzDogCommandControl.Cast(acs) != null )
			{
				//hic = player.GetInputController();
				//inputController.OverrideHeading(true, inputController.GetHeading() + Math.RAD2DEG*( hic.GetAimChange()[0]));
				//inputController.OverrideHeading(true, 180);
				if (this.GetDoggoCmd() != DoggoCmd.PLRCONTROL )
				{
					//inputController.OverrideHeading(false, 0);
					m_AIAgent.SetKeepInIdle(false); //zapnout AI zpatky
					StartCommand_Move();
				}
				return true;
			}
			/*
			if ( this.GetCommand_Script() == m_Movement )
			{
				if (GetGame().GetWaterDepth( this.GetPosition() ) <= 0.6)
				{
					m_AIAgent.SetKeepInIdle(false); //zapnout AI zpatky
					StartCommand_Move();
				}
				
				return true;
			}
			*/
			if ( this.GetCommand_Script() == m_Sitting )
			{
				//DoggoLogger.Log("SIT");
				if (player)
				{
					hic = player.GetInputController();
					hic.GetMovement(plrspeed,plrdirection);
					if ( vector.Distance(this.GetPosition(), player.GetPosition()) > 1.5 || plrspeed > 0 || this.GetDoggoCmd() == DoggoCmd.EATING || this.GetDoggoCmd() == DoggoCmd.STAY || this.GetDoggoCmd() == DoggoCmd.TAME || this.GetDoggoCmd() == DoggoCmd.PATROL || this.GetDoggoCmd() == DoggoCmd.MISSION)
					{
						switch (this.GetDoggoCmd())
						{
							case DoggoCmd.EATING:
								StartCommand_DogEating();
							break;
							
							case DoggoCmd.STAY:
								m_AIAgent.SetKeepInIdle(true);
								StartCommand_DogStay();
							break;
							
							case DoggoCmd.TAME:
								StartCommand_DogTaming();
							break;
							
							default:
								m_AIAgent.SetKeepInIdle(false); //zapnout AI zpatky
								StartCommand_Move();
							break;
						}
						
					}
				}
				return true;
			}
			
			if (this.GetCommand_Script() == m_Eating)
			{
				//DoggoLogger.Log("EAT");
				
				if (this.GetDoggoCmd() != DoggoCmd.EATING )
				{
					switch (this.GetDoggoCmd())
					{
						case DoggoCmd.STAY:
							m_AIAgent.SetKeepInIdle(true);
							StartCommand_DogStay();
						break;
						
						case DoggoCmd.TAME:
							StartCommand_DogTaming();
						break;
						
						default:
							m_AIAgent.SetKeepInIdle(false); //zapnout AI zpatky
							StartCommand_Move();
						break;
					}
				}
				
				return true;
			}
			
			if ( this.GetCommand_Script() == m_Stay)
			{
				//DoggoLogger.Log("STAY");
				if (this.GetDoggoCmd() != DoggoCmd.STAY)
				{
					switch (this.GetDoggoCmd())
					{
						case DoggoCmd.EATING:
							StartCommand_DogEating();
						break;
									
						case DoggoCmd.TAME:
							StartCommand_DogTaming();
						break;
						
						default:
							m_AIAgent.SetKeepInIdle(false); //zapnout AI zpatky
							StartCommand_Move();
						break;
					}
					
				}
				
				return true;
			}
			
			

			if ( this.GetCommand_Script() == m_Taming)
			{
				//DoggoLogger.Log("TAME");
				if (this.GetDoggoCmd() != DoggoCmd.TAME )
				{
					switch (this.GetDoggoCmd())
					{
						case DoggoCmd.STAY:
							m_AIAgent.SetKeepInIdle(true);
							StartCommand_DogStay();
						break;
						
						case DoggoCmd.EATING:
							StartCommand_DogEating();
						break;
						
						default:
							m_AIAgent.SetKeepInIdle(false); //zapnout AI zpatky
							StartCommand_Move();
						break;
					}
				}
				return true;
			}
			
		}
		if (this.GetDoggoCmd() == DoggoCmd.PLRCONTROL)
		{
			m_AIAgent.SetKeepInIdle(true);
			//StartCommand_DogControl();
			hic = player.GetInputController();
			inputController.OverrideHeading(true, inputController.GetHeading()+(hic.GetAimChange()[0]));
			//inputController.OverrideHeading(true, 180);
			hic.GetMovement(plrspeed,plrdirection);
			float mspeed = plrspeed;
			if (plrspeed == 1)
			mspeed = 3;
			if (plrspeed == 2)
			mspeed = 3.8;
			if (plrspeed >= 3)
			mspeed = 5.2;
			inputController.OverrideMovementSpeed(true, mspeed);
			if (hic.IsJumpClimb())
			{
				inputController.OverrideJump(true, 101, 100);
			} else {
				inputController.OverrideJump(false, 0, 0);	
			}
			if (hic.IsUseButton() && !hic.IsWeaponRaised())
			{
				SetDogAttackLow(false);
				StartCommand_Attack();
			}
			else if (hic.IsUseButton() && hic.IsWeaponRaised())
			{
				SetDogAttackLow(true);
				StartCommand_Attack();
			}
		} else {
			//hic = player.GetInputController();
			
			inputController.OverrideHeading(false, 0);
			inputController.OverrideMovementSpeed(false, 0);
		}
		
		if (GetGame().GetWaterDepth( this.GetPosition() ) > 0.6)
		{
			//m_AIAgent.SetKeepInIdle(true);
			//StartCommand_DogMovement();
		}
		
		if (player)
		{
			hic = player.GetInputController();
			hic.GetMovement(plrspeed,plrdirection);
			//DoggoLogger.Log("player speed " + plrspeed);
			//DoggoLogger.Log("DOG CMD BEFORE SIT " + this.GetDoggoCmd());
			if ( vector.Distance(this.GetPosition(), player.GetPosition()) <= 1.5 && plrspeed == 0 && this.GetSpeed().Normalize() < 0.7 && !player.IsSwimming() && this.GetDoggoCmd() != DoggoCmd.MISSION && this.GetDoggoCmd() != DoggoCmd.PATROL && this.GetDoggoCmd() != DoggoCmd.EATING && this.GetDoggoCmd() != DoggoCmd.STAY && this.GetDoggoCmd() != DoggoCmd.TAME)
			{
				//DoggoLogger.Log("DOG SHOULD SIT ");
				m_AIAgent.SetKeepInIdle(true); //vypnout AI
				StartCommand_DogSit();
				//DoggoLogger.Log("START SIT");
				
			}

		}
		
		if (this.GetDoggoCmd() == DoggoCmd.STAY )
		{
			m_AIAgent.SetKeepInIdle(true); //vypnout AI
			StartCommand_DogStay();
			
		}
		
		if (this.GetDoggoCmd() == DoggoCmd.EATING)
		{
			m_AIAgent.SetKeepInIdle(true); //vypnout AI
			StartCommand_DogEating();
			//DoggoLogger.Log("START EAT");
		}
		if (this.GetDoggoCmd() == DoggoCmd.TAME)
		{
			m_AIAgent.SetKeepInIdle(true); //vypnout AI
			StartCommand_DogTaming();
			//DoggoLogger.Log("START TAME");
		}
		
		
		return false;
	}
	
	bool FightAttackLogic(int pCurrentCommandID, DayZAnimalInputController pInputController, float pDt)
	{
		Object target = DayZPlayerUtils.GetMeleeTarget(this.GetPosition(), this.GetDirection(), 20, 2, -1.0, 2.0, this, m_TargetableObjects, m_AllTargetObjects);
		Dayz_Doggo dog = Dayz_Doggo.Cast(target);
		if (target && this.GetDoggoCmd() == DoggoCmd.PATROL && !dog)
		{
			StartCommand_Attack();
			return true;
		}
		#ifdef AI_BANDITS
		else if (target && GetNPCOwner())
		{
			StartCommand_Attack();
			return true;
		}
		#endif
		return false;
	}

	int GetOwnerId()
	{
		return m_DogOwner;
	}
	
	int GetDogIndex()
	{
		return m_DogIndex;
	}
	
	/*
	override bool IsHealthVisible()
	{
		return true;
	}
	*/

	override bool CanBeSkinned()
    {
        return false;
    }
		
	void SetOwnerId(int owner)
	{
		m_DogOwner = owner;
		SetSynchDirty();
	}
	#ifdef AI_BANDITS
	void SetNPCOwner(InfectedBanditBase owner)
	{
		m_NPCOwner = owner;
		m_TargetableObjects.Insert(PlayerBase);
	}
	
	InfectedBanditBase GetNPCOwner()
	{
		return m_NPCOwner;
	}
	#endif
	//pridat targetovani hrace kdyz je pes npc owned
	
	override bool CanBeTargetedByAI(EntityAI ai)
	{
		#ifdef DAYZ_HORSE
		Horse_Base horse = Horse_Base.Cast(ai);
		if (horse)
		{
			return false;
		}
		#endif
		#ifdef AI_BANDITS
		InfectedBanditBase bandit = InfectedBanditBase.Cast(ai);
		if (bandit)
		{
			return false;
		}
		#endif
		ZombieBase infected = ZombieBase.Cast(ai);
		#ifdef AI_BANDITS
		if (infected && (this.GetDoggoCmd() == DoggoCmd.STAY || this.GetDoggoCmd() == DoggoCmd.MISSION || this.GetNPCOwner() != null) )
		#else
		if (infected && (this.GetDoggoCmd() == DoggoCmd.STAY || this.GetDoggoCmd() == DoggoCmd.MISSION) )
		#endif
		{
			return false;
		}
		AnimalBase animal = AnimalBase.Cast(ai);
		if (animal && this.GetDoggoCmd() == DoggoCmd.MISSION)
		{
			return false;
		}
		// dog mission !!! also in zombies
		return super.CanBeTargetedByAI( ai );
	}
	
	override bool HandleDeath(int currentCommandID, DayZAnimalInputController inputController)
	{
		if (inputController.IsDead())
		{
			//informuj hrace a pak smaz psa pomoci resetDog()
			if ( GetGame().IsServer() && GetGame().IsMultiplayer() )
			{
				if (m_DoggoClosetrigger)
				{
					GetGame().ObjectDelete( m_DoggoClosetrigger );
					
				}
				if ( m_DoggoFartrigger)
				{
					GetGame().ObjectDelete( m_DoggoFartrigger );
				}
				array<Man> players = new array<Man>;
				GetGame().GetPlayers(players);
				for(int i = 0; i < players.Count(); i++)
				{
					if(players.Get(i).GetID() == this.GetOwnerId() )
					{
						if (PlayerBase.Cast(players.Get(i)).HaveDog())
						{
							DoggoLogger.Log("Dog of " + players.Get(i).GetIdentity().GetName() + " has died" );
						}
						PlayerBase.Cast(players.Get(i)).resetDog();
						
						
					}
				}
			}
			
		}
		return super.HandleDeath(currentCommandID, inputController);
	}
	
	override void RegisterHitComponentsForAI()
	{
		m_DefaultHitComponent = "Zone_Chest";
		m_DefaultHitPositionComponent = "Pelvis";
		
		DayZAIHitComponentHelpers.RegisterHitComponent(m_HitComponentsForAI, "Zone_Head", 2);
		DayZAIHitComponentHelpers.RegisterHitComponent(m_HitComponentsForAI, "Zone_Neck", 65);
		DayZAIHitComponentHelpers.RegisterHitComponent(m_HitComponentsForAI, "Zone_Chest", 50);
		DayZAIHitComponentHelpers.RegisterHitComponent(m_HitComponentsForAI, "Zone_Legs_Back", 75);
		DayZAIHitComponentHelpers.RegisterHitComponent(m_HitComponentsForAI, "Zone_Legs_Front", 70);
	}
	/*
	override private void EOnContact(IEntity other, Contact extra)
	{
		//why?
		if( !IsAlive() )
			return;
	}
	*/
	bool IsWild()
	{
		return false;
	}

	bool can_follow()
	{
		return false;
	}

	bool can_stay()
	{
		return false;
	}

	bool can_patrol()
	{
		return false;
	}
	/*
	Dayz_Doggo zmb_atk_ownr(Dayz_Doggo my_dog)
	{
		vector targetObjectPos = my_dog.GetPosition();
		int owner = my_dog.GetOwnerId();
		int dogindex = my_dog.GetDogIndex(); //vzit cislo z classname
		GetGame().ObjectDelete( my_dog );
		Dayz_Doggo target_dog = Dayz_Doggo.Cast(GetGame().CreateObject("Doggo_Patrol"+dogindex, targetObjectPos, false, true ));
		target_dog.SetOwnerId(owner);
		target_dog.SetDoggoCmd(DoggoCmd.PATROL);
		return target_dog;
	}
	*/
	/*
	void SwitchDogPatrol(Dayz_Doggo my_dog)
	{
		if (!my_dog.IsAlive())
			return;
		PlayerBase player = GetPlayerByEntityID(my_dog.GetOwnerId());
		vector targetObjectPos = my_dog.GetPosition();
		int owner = my_dog.GetOwnerId();
		int dogindex = my_dog.GetDogIndex();
		GetGame().ObjectDelete( my_dog );
		Dayz_Doggo target_dog = Dayz_Doggo.Cast(GetGame().CreateObject("Doggo_Patrol"+dogindex, targetObjectPos, false, true ));
		target_dog.SetOwnerId(owner);
		player.setMyDog(target_dog);
		target_dog.SetDoggoCmd(DoggoCmd.PATROL);
	}
	*/
	
	protected static PlayerBase GetPlayerByEntityID(int entityID) //bylo protected static
	{
		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);
		for(int i = 0; i < players.Count(); i++)
		{
			if(players.Get(i).GetID() == entityID)
			{
				return PlayerBase.Cast(players.Get(i));
			}
		}
		return null;
	}
	/*
	override void EOnFrame(IEntity other, float timeSlice) // | EntityEvent.FRAME
    {
        vector rotation = GetYawPitchRoll();
        SetYawPitchRoll(Vector(rotation[0], 0, 0));
    }
        
    override void EOnPostFrame(IEntity other, int extra) // | EntityEvent.POSTFRAME
    {
        vector mat[4];
        GetTransform(mat);

        mat[0] = mat[0].Normalized() * m_Scale;
        mat[1] = mat[1].Normalized() * m_Scale;
        mat[2] = mat[2].Normalized() * m_Scale;

        SetTransform(mat);
    }  
	*/
	
	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{	
		if( source.IsInherited(Dayz_Doggo) )
		{
			return;
		}
		if (source.IsInherited(ZombieBase) && this.GetDoggoCmd() != DoggoCmd.PLRCONTROL )
		{
			this.SetDoggoCmd(DoggoCmd.PATROL);
		}
		PlayerBase player = GetPlayerByEntityID(this.GetOwnerId());
		if (player && GetGame().IsServer())
		{
			player.StoreDogHealth();
		}
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
		/*
		// TODO
		if ((!can_stay() || !can_follow()) && GetGame().IsServer() && GetGame().IsMultiplayer() && !source.IsInherited(PlayerBase) )
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SwitchDogPatrol, 1000, false, this);
		}
		*/		
	}
	
	override bool EEOnDamageCalculated(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		if (!super.EEOnDamageCalculated(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef))
			return false;

		
		if (GetGame().IsMultiplayer() && !GetGame().IsServer())
			return true;
		
		EntityAI vest = FindAttachmentBySlotName("DogVest");
		//Print("dog damage " + dmgZone + " " + damageResult.GetDamage(dmgZone, "Health") + GetHealth(dmgZone, "Health") + " X " +  GetHealth("", ""));
		if (vest && source.GetHierarchyRootPlayer() && GetOwnerId() != source.GetHierarchyRootPlayer().GetID() && GetDoggoAttackTarget() != source && dmgZone != "Zone_Head")
		{
			return false;
		}
		
		
		return true;
	}
	
	
	override void EEKilled( Object killer )
	{
		EntityAI vrah = EntityAI.Cast(killer);
		if (GetOwnerId()>0 && vrah.GetHierarchyRootPlayer() && vrah.GetHierarchyRootPlayer().GetIdentity() && vrah.GetHierarchyRootPlayer().GetIdentity().GetName())
		{
			DoggoLogger.Log("Dog " + this.GetType() + " killed by " + vrah.GetHierarchyRootPlayer().GetIdentity().GetName() + " (" + vrah.GetHierarchyRootPlayer().GetIdentity().GetPlainId() + ")" );
			DoggoConfig config = GetDayZGame().GetDayzDogConfig();
			if (config.PunishDogKillers)
			{
				vrah.GetHierarchyRootPlayer().InsertAgent(eAgents.BRAIN, 1999);
			}
			
		}
		
		//super.EEKilled( killer );
	}
}

class Doggo_Follow extends Dayz_Doggo
{
	void Doggo_Follow1()
	{
		m_DoggoCmd = DoggoCmd.FOLLOW;
	}
	
	override bool can_follow()
	{
		return false;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Stay extends Dayz_Doggo
{
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return false;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Patrol extends Dayz_Doggo
{
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return false;
	}
}

class Doggo_Wild extends Dayz_Doggo
{
	void Doggo_Wild()
	{
		m_DoggoCmd = DoggoCmd.WILD;
	}
	
	override bool IsWild()
	{
		return true;
	}
}

class Dayz_Doggo1 extends Dayz_Doggo
{
	void Dayz_Doggo1()
	{
		m_DogIndex = 1;

	}
	
}

class Doggo_Follow1 extends Dayz_Doggo1
{
	
	void Doggo_Follow1()
	{
		m_DoggoCmd = DoggoCmd.FOLLOW;
		//DestroyAIAgent();
		//AIWorld world = GetGame().GetWorld().GetAIWorld();
		//InitAIAgent(world.CreateGroup("DZWolfGroupBeh"));
	}

	override bool can_follow()
	{
		return false;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Stay1 extends Dayz_Doggo1
{
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return false;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Patrol1 extends Dayz_Doggo1
{
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return false;
	}
}

class Doggo_Wild1 extends Dayz_Doggo1
{
	void Doggo_Wild1()
	{
		m_DoggoCmd = DoggoCmd.WILD;
	}
	
	override bool IsWild()
	{
		return true;
	}
}

class Dayz_Doggo2 extends Dayz_Doggo
{
	void Dayz_Doggo2()
	{
		m_DogIndex = 2;
	}
}

class Doggo_Follow2 extends Dayz_Doggo2
{
	void Doggo_Follow2()
	{
		m_DoggoCmd = DoggoCmd.FOLLOW;
	}
	
	override bool can_follow()
	{
		return false;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Stay2 extends Dayz_Doggo2
{
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return false;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Patrol2 extends Dayz_Doggo2
{
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return false;
	}
}

class Doggo_Wild2 extends Dayz_Doggo2
{
	void Doggo_Wild2()
	{
		m_DoggoCmd = DoggoCmd.WILD;
	}
	
	override bool IsWild()
	{
		return true;
	}
}

class Dayz_Doggo3 extends Dayz_Doggo
{
	void Dayz_Doggo3()
	{
		m_DogIndex = 3;
	}
}

class Doggo_Follow3 extends Dayz_Doggo3
{
	void Doggo_Follow3()
	{
		m_DoggoCmd = DoggoCmd.FOLLOW;
	}
	
	override bool can_follow()
	{
		return false;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Stay3 extends Dayz_Doggo3
{
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return false;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Patrol3 extends Dayz_Doggo3
{
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return false;
	}
}

class Doggo_Wild3 extends Dayz_Doggo3
{
	void Doggo_Wild3()
	{
		m_DoggoCmd = DoggoCmd.WILD;
	}
	
	override bool IsWild()
	{
		return true;
	}
}

class Dayz_Doggo4 extends Dayz_Doggo
{
	void Dayz_Doggo4()
	{
		m_DogIndex = 4;
	}
}

class Doggo_Follow4 extends Dayz_Doggo4
{
	void Doggo_Follow4()
	{
		m_DoggoCmd = DoggoCmd.FOLLOW;
	}
	
	override bool can_follow()
	{
		return false;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Stay4 extends Dayz_Doggo4
{
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return false;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Patrol4 extends Dayz_Doggo4
{
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return false;
	}
}

class Doggo_Wild4 extends Dayz_Doggo4
{
	void Doggo_Wild4()
	{
		m_DoggoCmd = DoggoCmd.WILD;
	}
	override bool IsWild()
	{
		return true;
	}
}

class Dayz_Doggo5 extends Dayz_Doggo
{
	void Dayz_Doggo5()
	{
		m_DogIndex = 5;
	}
}

class Doggo_Follow5 extends Dayz_Doggo5
{
	void Doggo_Follow5()
	{
		m_DoggoCmd = DoggoCmd.FOLLOW;
	}
	
	override bool can_follow()
	{
		return false;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Stay5 extends Dayz_Doggo5
{
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return false;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Patrol5 extends Dayz_Doggo5
{
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return false;
	}
}

class Doggo_Wild5 extends Dayz_Doggo5
{
	void Doggo_Wild5()
	{
		m_DoggoCmd = DoggoCmd.WILD;
	}
	
	override bool IsWild()
	{
		return true;
	}
}

class Dayz_Doggo6 extends Dayz_Doggo
{
	void Dayz_Doggo6()
	{
		m_DogIndex = 6;
	}
}

class Doggo_Follow6 extends Dayz_Doggo6
{
	void Doggo_Follow6()
	{
		m_DoggoCmd = DoggoCmd.FOLLOW;
	}
	
	override bool can_follow()
	{
		return false;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Stay6 extends Dayz_Doggo6
{
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return false;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Patrol6 extends Dayz_Doggo6
{
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return false;
	}
}

class Doggo_Wild6 extends Dayz_Doggo6
{
	void Doggo_Wild6()
	{
		m_DoggoCmd = DoggoCmd.WILD;
	}
	
	override bool IsWild()
	{
		return true;
	}
}

class Dayz_Doggo7 extends Dayz_Doggo
{
	void Dayz_Doggo7()
	{
		m_DogIndex = 7;
	}
}

class Doggo_Follow7 extends Dayz_Doggo7
{
	void Doggo_Follow7()
	{
		m_DoggoCmd = DoggoCmd.FOLLOW;
	}
	
	override bool can_follow()
	{
		return false;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Stay7 extends Dayz_Doggo7
{
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return false;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Patrol7 extends Dayz_Doggo7
{
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return false;
	}
}

class Doggo_Wild7 extends Dayz_Doggo7
{
	void Doggo_Wild7()
	{
		m_DoggoCmd = DoggoCmd.WILD;
	}
	
	override bool IsWild()
	{
		return true;
	}
}

class Dayz_Doggo8 extends Dayz_Doggo
{
	void Dayz_Doggo8()
	{
		m_DogIndex = 8;
	}
}

class Doggo_Follow8 extends Dayz_Doggo8
{
	void Doggo_Follow8()
	{
		m_DoggoCmd = DoggoCmd.FOLLOW;
	}
	
	override bool can_follow()
	{
		return false;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Stay8 extends Dayz_Doggo8
{
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return false;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Patrol8 extends Dayz_Doggo8
{
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return false;
	}
}

class Doggo_Wild8 extends Dayz_Doggo8
{
	
	void Doggo_Wild8()
	{
		m_DoggoCmd = DoggoCmd.WILD;
	}
	
	override bool IsWild()
	{
		return true;
	}
}

class Dayz_Doggo9 extends Dayz_Doggo
{
	void Dayz_Doggo9()
	{
		m_DogIndex = 9;
	}
}

class Doggo_Follow9 extends Dayz_Doggo9
{
	void Doggo_Follow9()
	{
		m_DoggoCmd = DoggoCmd.FOLLOW;
	}
	
	override bool can_follow()
	{
		return false;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Stay9 extends Dayz_Doggo9
{
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return false;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Patrol9 extends Dayz_Doggo9
{
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return false;
	}
}

class Doggo_Wild9 extends Dayz_Doggo9
{
	void Doggo_Wild9()
	{
		m_DoggoCmd = DoggoCmd.WILD;
	}
	
	override bool IsWild()
	{
		return true;
	}
}

class Dayz_Doggo10 extends Dayz_Doggo
{
	void Dayz_Doggo10()
	{
		m_DogIndex = 10;
	}
}

class Doggo_Follow10 extends Dayz_Doggo10
{
	void Doggo_Follow10()
	{
		m_DoggoCmd = DoggoCmd.FOLLOW;
	}
	
	override bool can_follow()
	{
		return false;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Stay10 extends Dayz_Doggo10
{
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return false;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Patrol10 extends Dayz_Doggo10
{
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return false;
	}
}

class Doggo_Wild10 extends Dayz_Doggo10
{
	
	void Doggo_Wild10()
	{
		m_DoggoCmd = DoggoCmd.WILD;
	}
	
	override bool IsWild()
	{
		return true;
	}
}

class Dayz_Doggo11 extends Dayz_Doggo
{
	void Dayz_Doggo11()
	{
		m_DogIndex = 11;
	}
}

class Doggo_Follow11 extends Dayz_Doggo11
{
	void Doggo_Follow11()
	{
		m_DoggoCmd = DoggoCmd.FOLLOW;
	}
	
	override bool can_follow()
	{
		return false;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Stay11 extends Dayz_Doggo11
{
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return false;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Patrol11 extends Dayz_Doggo11
{
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return false;
	}
}

class Doggo_Wild11 extends Dayz_Doggo11
{
	void Doggo_Wild11()
	{
		m_DoggoCmd = DoggoCmd.WILD;
	}
	
	override bool IsWild()
	{
		return true;
	}
}

class Dayz_Doggo12 extends Dayz_Doggo
{
	void Dayz_Doggo12()
	{
		m_DogIndex = 12;
	}
	/*
	override bool CanDisplayAttachmentSlot( int slot_id )
	{
		if (!super.CanDisplayAttachmentSlot(slot_id))
			return false;
		
		string slot_name = InventorySlots.GetSlotName(slot_id);
		
		if ( slot_name == "DogMask" )
		{
				return false;
		}
			
		return true;
	}
	*/
}

class Doggo_Follow12 extends Dayz_Doggo12
{
	void Doggo_Follow12()
	{
		m_DoggoCmd = DoggoCmd.FOLLOW;
	}
	
	override bool can_follow()
	{
		return false;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Stay12 extends Dayz_Doggo12
{
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return false;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Patrol12 extends Dayz_Doggo12
{
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return false;
	}
}

class Doggo_Wild12 extends Dayz_Doggo12
{
	void Doggo_Wild12()
	{
		m_DoggoCmd = DoggoCmd.WILD;
	}
	
	override bool IsWild()
	{
		return true;
	}
}

class Dayz_Doggo13 extends Dayz_Doggo
{
	void Dayz_Doggo13()
	{
		m_DogIndex = 13;
	}
}

class Doggo_Follow13 extends Dayz_Doggo13
{
	void Doggo_Follow13()
	{
		m_DoggoCmd = DoggoCmd.FOLLOW;
	}
	
	override bool can_follow()
	{
		return false;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Stay13 extends Dayz_Doggo13
{
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return false;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Patrol13 extends Dayz_Doggo13
{
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return false;
	}
}

class Doggo_Wild13 extends Dayz_Doggo13
{
	void Doggo_Wild13()
	{
		m_DoggoCmd = DoggoCmd.WILD;
	}
	
	override bool IsWild()
	{
		return true;
	}
}

class Dayz_Doggo14 extends Dayz_Doggo
{
	void Dayz_Doggo14()
	{
		m_DogIndex = 14;
	}
}

class Doggo_Follow14 extends Dayz_Doggo14
{
	void Doggo_Follow14()
	{
		m_DoggoCmd = DoggoCmd.FOLLOW;
	}
	
	override bool can_follow()
	{
		return false;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Stay14 extends Dayz_Doggo14
{
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return false;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Patrol14 extends Dayz_Doggo14
{
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return false;
	}
}

class Doggo_Wild14 extends Dayz_Doggo14
{
	void Doggo_Wild14()
	{
		m_DoggoCmd = DoggoCmd.WILD;
	}
	
	override bool IsWild()
	{
		return true;
	}
}

class Dayz_Doggo15 extends Dayz_Doggo
{
	void Dayz_Doggo15()
	{
		m_DogIndex = 15;
	}
}

class Doggo_Follow15 extends Dayz_Doggo15
{
	void Doggo_Follow15()
	{
		m_DoggoCmd = DoggoCmd.FOLLOW;
	}
	
	override bool can_follow()
	{
		return false;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Stay15 extends Dayz_Doggo15
{
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return false;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Patrol15 extends Dayz_Doggo15
{
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return false;
	}
}

class Doggo_Wild15 extends Dayz_Doggo15
{
	void Doggo_Wild15()
	{
		m_DoggoCmd = DoggoCmd.WILD;
	}
	
	override bool IsWild()
	{
		return true;
	}
}

class Dayz_Doggo16 extends Dayz_Doggo
{
	void Dayz_Doggo16()
	{
		m_DogIndex = 16;
	}
}

class Doggo_Follow16 extends Dayz_Doggo16
{
	void Doggo_Follow16()
	{
		m_DoggoCmd = DoggoCmd.FOLLOW;
	}
	override bool can_follow()
	{
		return false;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Stay16 extends Dayz_Doggo16
{
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return false;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Patrol16 extends Dayz_Doggo16
{
	void Doggo_Patrol16()
	{
		m_DoggoCmd = DoggoCmd.PATROL;
	}
	
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return false;
	}
}

class Doggo_Wild16 extends Dayz_Doggo16
{
	void Doggo_Wild16()
	{
		m_DoggoCmd = DoggoCmd.WILD;
	}
	
	override bool IsWild()
	{
		return true;
	}
}

class Dayz_Doggo17 extends Dayz_Doggo
{
	void Dayz_Doggo17()
	{
		m_DogIndex = 17;
		
	}
}

class Doggo_Follow17 extends Dayz_Doggo17
{
	override bool can_follow()
	{
		return false;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Stay17 extends Dayz_Doggo17
{
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return false;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Patrol17 extends Dayz_Doggo17
{
	void Doggo_Patrol17()
	{
		m_DoggoCmd = DoggoCmd.PATROL;
	}
	
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return false;
	}
}
class Doggo_Wild17 extends Dayz_Doggo17
{
	void Doggo_Wild17()
	{
		m_DoggoCmd = DoggoCmd.WILD;
	}
	
	override bool IsWild()
	{
		return true;
	}
}
class Dayz_Doggo18 extends Dayz_Doggo
{
	void Dayz_Doggo18()
	{
		m_DogIndex = 18;
		
	}
}

class Doggo_Follow18 extends Dayz_Doggo18
{
	override bool can_follow()
	{
		return false;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Stay18 extends Dayz_Doggo18
{
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return false;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Patrol18 extends Dayz_Doggo18
{
	void Doggo_Patrol18()
	{
		m_DoggoCmd = DoggoCmd.PATROL;
	}
	
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return false;
	}
}
class Doggo_Wild18 extends Dayz_Doggo18
{
	void Doggo_Wild18()
	{
		m_DoggoCmd = DoggoCmd.WILD;
	}
	
	override bool IsWild()
	{
		return true;
	}
}
class Dayz_Doggo19 extends Dayz_Doggo
{
	void Dayz_Doggo19()
	{
		m_DogIndex = 19;
		
	}
}

class Doggo_Follow19 extends Dayz_Doggo19
{
	override bool can_follow()
	{
		return false;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Stay19 extends Dayz_Doggo19
{
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return false;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Patrol19 extends Dayz_Doggo19
{
	void Doggo_Patrol19()
	{
		m_DoggoCmd = DoggoCmd.PATROL;
	}
	
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return false;
	}
}
class Doggo_Wild19 extends Dayz_Doggo19
{
	void Doggo_Wild19()
	{
		m_DoggoCmd = DoggoCmd.WILD;
	}
	
	override bool IsWild()
	{
		return true;
	}
}
class Dayz_Doggo20 extends Dayz_Doggo
{
	void Dayz_Doggo20()
	{
		m_DogIndex = 20;
		
	}
}

class Doggo_Follow20 extends Dayz_Doggo20
{
	override bool can_follow()
	{
		return false;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Stay20 extends Dayz_Doggo20
{
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return false;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Patrol20 extends Dayz_Doggo20
{
	void Doggo_Patrol20()
	{
		m_DoggoCmd = DoggoCmd.PATROL;
	}
	
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return false;
	}
}
class Doggo_Wild20 extends Dayz_Doggo20
{
	void Doggo_Wild20()
	{
		m_DoggoCmd = DoggoCmd.WILD;
	}
	
	override bool IsWild()
	{
		return true;
	}
}
class Dayz_Doggo21 extends Dayz_Doggo
{
	void Dayz_Doggo21()
	{
		m_DogIndex = 21;
		
	}
}

class Doggo_Follow21 extends Dayz_Doggo21
{
	override bool can_follow()
	{
		return false;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Stay21 extends Dayz_Doggo21
{
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return false;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Patrol21 extends Dayz_Doggo21
{
	void Doggo_Patrol21()
	{
		m_DoggoCmd = DoggoCmd.PATROL;
	}
	
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return false;
	}
}
class Doggo_Wild21 extends Dayz_Doggo21
{
	void Doggo_Wild21()
	{
		m_DoggoCmd = DoggoCmd.WILD;
	}
	
	override bool IsWild()
	{
		return true;
	}
}
class Dayz_Doggo22 extends Dayz_Doggo
{
	void Dayz_Doggo22()
	{
		m_DogIndex = 22;
		
	}
}

class Doggo_Follow22 extends Dayz_Doggo22
{
	override bool can_follow()
	{
		return false;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Stay22 extends Dayz_Doggo22
{
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return false;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Patrol22 extends Dayz_Doggo22
{
	void Doggo_Patrol22()
	{
		m_DoggoCmd = DoggoCmd.PATROL;
	}
	
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return false;
	}
}
class Doggo_Wild22 extends Dayz_Doggo22
{
	void Doggo_Wild22()
	{
		m_DoggoCmd = DoggoCmd.WILD;
	}
	
	override bool IsWild()
	{
		return true;
	}
}
class Dayz_Doggo23 extends Dayz_Doggo
{
	void Dayz_Doggo23()
	{
		m_DogIndex = 23;
		
	}
}

class Doggo_Follow23 extends Dayz_Doggo23
{
	override bool can_follow()
	{
		return false;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Stay23 extends Dayz_Doggo23
{
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return false;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Patrol23 extends Dayz_Doggo23
{
	void Doggo_Patrol23()
	{
		m_DoggoCmd = DoggoCmd.PATROL;
	}
	
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return false;
	}
}
class Doggo_Wild23 extends Dayz_Doggo23
{
	void Doggo_Wild23()
	{
		m_DoggoCmd = DoggoCmd.WILD;
	}
	
	override bool IsWild()
	{
		return true;
	}
}
class Dayz_Doggo24 extends Dayz_Doggo
{
	void Dayz_Doggo24()
	{
		m_DogIndex = 24;
		
	}
}

class Doggo_Follow24 extends Dayz_Doggo24
{
	override bool can_follow()
	{
		return false;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Stay24 extends Dayz_Doggo24
{
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return false;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Patrol24 extends Dayz_Doggo24
{
	void Doggo_Patrol24()
	{
		m_DoggoCmd = DoggoCmd.PATROL;
	}
	
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return false;
	}
}
class Doggo_Wild24 extends Dayz_Doggo24
{
	void Doggo_Wild24()
	{
		m_DoggoCmd = DoggoCmd.WILD;
	}
	
	override bool IsWild()
	{
		return true;
	}
}

class Dayz_Doggo25 extends Dayz_Doggo
{
	void Dayz_Doggo25()
	{
		m_DogIndex = 25;
		
	}
}

class Doggo_Follow25 extends Dayz_Doggo25
{
	override bool can_follow()
	{
		return false;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Stay25 extends Dayz_Doggo25
{
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return false;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Patrol25 extends Dayz_Doggo25
{
	void Doggo_Patrol25()
	{
		m_DoggoCmd = DoggoCmd.PATROL;
	}
	
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return false;
	}
}
class Doggo_Wild25 extends Dayz_Doggo25
{
	void Doggo_Wild25()
	{
		m_DoggoCmd = DoggoCmd.WILD;
	}
	
	override bool IsWild()
	{
		return true;
	}
}

class Dayz_Doggo26 extends Dayz_Doggo
{
	void Dayz_Doggo26()
	{
		m_DogIndex = 26;
		
	}
}

class Doggo_Follow26 extends Dayz_Doggo26
{
	override bool can_follow()
	{
		return false;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Stay26 extends Dayz_Doggo26
{
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return false;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Patrol26 extends Dayz_Doggo26
{
	void Doggo_Patrol26()
	{
		m_DoggoCmd = DoggoCmd.PATROL;
	}
	
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return false;
	}
}

class Doggo_Wild26 extends Dayz_Doggo26
{
	void Doggo_Wild26()
	{
		m_DoggoCmd = DoggoCmd.WILD;
	}
	
	override bool IsWild()
	{
		return true;
	}
}

class Dayz_Doggo27 extends Dayz_Doggo
{
	void Dayz_Doggo27()
	{
		m_DogIndex = 27;
		
	}
}

class Doggo_Follow27 extends Dayz_Doggo27
{
	override bool can_follow()
	{
		return false;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Stay27 extends Dayz_Doggo27
{
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return false;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Patrol27 extends Dayz_Doggo27
{
	void Doggo_Patrol27()
	{
		m_DoggoCmd = DoggoCmd.PATROL;
	}
	
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return false;
	}
}
class Doggo_Wild27 extends Dayz_Doggo27
{
	void Doggo_Wild27()
	{
		m_DoggoCmd = DoggoCmd.WILD;
	}
	
	override bool IsWild()
	{
		return true;
	}
}

class Dayz_Doggo28 extends Dayz_Doggo
{
	void Dayz_Doggo28()
	{
		m_DogIndex = 28;
		
	}
}

class Doggo_Follow28 extends Dayz_Doggo28
{
	override bool can_follow()
	{
		return false;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Stay28 extends Dayz_Doggo28
{
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return false;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Patrol28 extends Dayz_Doggo28
{
	void Doggo_Patrol28()
	{
		m_DoggoCmd = DoggoCmd.PATROL;
	}
	
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return false;
	}
}
class Doggo_Wild28 extends Dayz_Doggo28
{
	void Doggo_Wild28()
	{
		m_DoggoCmd = DoggoCmd.WILD;
	}
	
	override bool IsWild()
	{
		return true;
	}
}

class Dayz_Doggo29 extends Dayz_Doggo
{
	void Dayz_Doggo29()
	{
		m_DogIndex = 29;
		
	}
}

class Doggo_Follow29 extends Dayz_Doggo29
{
	override bool can_follow()
	{
		return false;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Stay29 extends Dayz_Doggo29
{
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return false;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Patrol29 extends Dayz_Doggo29
{
	void Doggo_Patrol29()
	{
		m_DoggoCmd = DoggoCmd.PATROL;
	}
	
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return false;
	}
}
class Doggo_Wild29 extends Dayz_Doggo29
{
	void Doggo_Wild29()
	{
		m_DoggoCmd = DoggoCmd.WILD;
	}
	
	override bool IsWild()
	{
		return true;
	}
}

class Dayz_Doggo30 extends Dayz_Doggo
{
	void Dayz_Doggo30()
	{
		m_DogIndex = 30;
		
	}
}

class Doggo_Follow30 extends Dayz_Doggo30
{
	override bool can_follow()
	{
		return false;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Stay30 extends Dayz_Doggo30
{
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return false;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Patrol30 extends Dayz_Doggo30
{
	void Doggo_Patrol30()
	{
		m_DoggoCmd = DoggoCmd.PATROL;
	}
	
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return false;
	}
}
class Doggo_Wild30 extends Dayz_Doggo30
{
	void Doggo_Wild30()
	{
		m_DoggoCmd = DoggoCmd.WILD;
	}
	
	override bool IsWild()
	{
		return true;
	}
}

class Dayz_Doggo31 extends Dayz_Doggo
{
	void Dayz_Doggo31()
	{
		m_DogIndex = 31;
		
	}
}

class Doggo_Follow31 extends Dayz_Doggo31
{
	override bool can_follow()
	{
		return false;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Stay31 extends Dayz_Doggo31
{
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return false;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Patrol31 extends Dayz_Doggo31
{
	void Doggo_Patrol31()
	{
		m_DoggoCmd = DoggoCmd.PATROL;
	}
	
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return false;
	}
}
class Doggo_Wild31 extends Dayz_Doggo31
{
	void Doggo_Wild31()
	{
		m_DoggoCmd = DoggoCmd.WILD;
	}
	
	override bool IsWild()
	{
		return true;
	}
}

class Dayz_Doggo32 extends Dayz_Doggo
{
	void Dayz_Doggo32()
	{
		m_DogIndex = 32;
		
	}
}

class Doggo_Follow32 extends Dayz_Doggo32
{
	override bool can_follow()
	{
		return false;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Stay32 extends Dayz_Doggo32
{
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return false;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Patrol32 extends Dayz_Doggo32
{
	void Doggo_Patrol32()
	{
		m_DoggoCmd = DoggoCmd.PATROL;
	}
	
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return false;
	}
}
class Doggo_Wild32 extends Dayz_Doggo32
{
	void Doggo_Wild32()
	{
		m_DoggoCmd = DoggoCmd.WILD;
	}
	
	override bool IsWild()
	{
		return true;
	}
}

class Dayz_Doggo33 extends Dayz_Doggo
{
	void Dayz_Doggo33()
	{
		m_DogIndex = 33;
		
	}
}

class Doggo_Follow33 extends Dayz_Doggo33
{
	override bool can_follow()
	{
		return false;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Stay33 extends Dayz_Doggo33
{
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return false;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Patrol33 extends Dayz_Doggo33
{
	void Doggo_Patrol33()
	{
		m_DoggoCmd = DoggoCmd.PATROL;
	}
	
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return false;
	}
}
class Doggo_Wild33 extends Dayz_Doggo33
{
	void Doggo_Wild33()
	{
		m_DoggoCmd = DoggoCmd.WILD;
	}
	
	override bool IsWild()
	{
		return true;
	}
}

class Dayz_Doggo34 extends Dayz_Doggo
{
	void Dayz_Doggo34()
	{
		m_DogIndex = 34;
		
	}
}

class Doggo_Follow34 extends Dayz_Doggo34
{
	override bool can_follow()
	{
		return false;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Stay34 extends Dayz_Doggo34
{
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return false;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Patrol34 extends Dayz_Doggo34
{
	void Doggo_Patrol34()
	{
		m_DoggoCmd = DoggoCmd.PATROL;
	}
	
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return false;
	}
}
class Doggo_Wild34 extends Dayz_Doggo34
{
	void Doggo_Wild34()
	{
		m_DoggoCmd = DoggoCmd.WILD;
	}
	
	override bool IsWild()
	{
		return true;
	}
}

class Dayz_Doggo35 extends Dayz_Doggo
{
	void Dayz_Doggo35()
	{
		m_DogIndex = 35;
		
	}
}

class Doggo_Follow35 extends Dayz_Doggo35
{
	override bool can_follow()
	{
		return false;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Stay35 extends Dayz_Doggo35
{
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return false;
	}

	override bool can_patrol()
	{
		return true;
	}
}

class Doggo_Patrol35 extends Dayz_Doggo35
{
	void Doggo_Patrol35()
	{
		m_DoggoCmd = DoggoCmd.PATROL;
	}
	
	override bool can_follow()
	{
		return true;
	}

	override bool can_stay()
	{
		return true;
	}

	override bool can_patrol()
	{
		return false;
	}
}
class Doggo_Wild35 extends Dayz_Doggo35
{
	void Doggo_Wild35()
	{
		m_DoggoCmd = DoggoCmd.WILD;
	}
	
	override bool IsWild()
	{
		return true;
	}
}