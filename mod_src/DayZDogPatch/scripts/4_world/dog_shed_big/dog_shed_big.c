class dog_shed_big extends dog_shed_small //BaseBuildingBase
{	
	protected bool m_HasGate		= true;
	protected bool m_IsOpened 		= false;
	protected bool m_IsOpenedClient	= false;
	
	typename ATTACHMENT_COMBINATION_LOCK	= CombinationLock;
	
	const string ATTACHMENT_SLOT_COMBINATION_LOCK = "Att_CombinationLock";
	
	const string SOUND_GATE_OPEN_START			= "DoorWoodTowerOpen_SoundSet";
	const string SOUND_GATE_CLOSE_START			= "DoorWoodTowerClose_start_SoundSet";
	const string SOUND_GATE_CLOSE_END			= "DoorWoodTowerClose_end_SoundSet";
	
	//gate openining
	const float GATE_ROTATION_ANGLE_DEG 			= 100;
	const float GATE_ROTATION_TIME_APPROX			= 2000;		//ms
	
	const float MAX_ACTION_DETECTION_ANGLE_RAD 		= 1.3;		//1.3 RAD = ~75 DEG
	const float MAX_ACTION_DETECTION_DISTANCE 		= 2.0;		//meters
	
	protected EffectSound m_SoundGate_Start;
	protected EffectSound m_SoundGate_End;
	
	void dog_shed_big()
	{
		RegisterNetSyncVariableBool( "m_HasGate" );
		RegisterNetSyncVariableBool( "m_IsOpened" );
	}
	
	override string GetConstructionKitType()
	{
		return "dog_shed_big_kit";
	}
	
	bool HasGate()
	{
		return m_HasGate;
	}
	
	void SetGateState( bool has_gate )
	{
		m_HasGate = has_gate;
	}	
	
	void SetOpenedState( bool state )
	{
		m_IsOpened = state;
	}
	
	override bool IsOpened()
	{
		return m_IsOpened;
	}
	
	bool IsLocked()
	{
		CombinationLock combination_lock = GetCombinationLock();
		if ( combination_lock && combination_lock.IsLocked() )
		{
			return true;
		}
		return false;
	}
		
	CombinationLock GetCombinationLock()
	{
		CombinationLock combination_lock = CombinationLock.Cast( FindAttachmentBySlotName( ATTACHMENT_SLOT_COMBINATION_LOCK ) );
		return combination_lock;
	}
	
	override void SetPartsAfterStoreLoad()
	{
		
	}
	
	override void OnStoreSave( ParamsWriteContext ctx )
	{   
		super.OnStoreSave( ctx );
		
		//write
		ctx.Write( m_HasGate );
		ctx.Write( m_IsOpened );
		bsbDebugPrint("[bsb] OnStoreSave - build=" + m_HasGate + " opened=" + m_IsOpened);
	}
	
	override bool OnStoreLoad( ParamsReadContext ctx, int version )
	{
		if ( !super.OnStoreLoad( ctx, version ) )
			return false;

		//has gate
		if ( !ctx.Read( m_HasGate ) )
		{
			m_HasGate = false;
			return false;
		}
		
		//is opened
		if ( !ctx.Read( m_IsOpened ) )
		{
			m_IsOpened = false;
			return false;
		}
		
		bsbDebugPrint("[bsb] OnStoreLoad - build=" + m_HasGate + " opened=" + m_IsOpened);
		
		return true;
	}
	
	override void AfterStoreLoad()
	{	
		//super.AfterStoreLoad();
		
		//set gate state
		ConstructionPart gate_part = GetConstruction().GetGateConstructionPart();
		//SetGateState( gate_part.IsBuilt() );
		SetGateState( true );
		
		//update gate state visual
		if ( IsOpened() )
		{
			Opendogshed_door();
		}
		else
		{
			Closedogshed_door();
		}
		
		UpdateVisuals();
		//super.AfterStoreLoad();
	}
	
	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();

		if ( m_IsOpenedClient != m_IsOpened )
		{
			m_IsOpenedClient = m_IsOpened;
			
			if ( m_IsOpenedClient )
			{
				Opendogshed_door();
			}
			else
			{
				Closedogshed_door();
			}
		}
	}
	
	override void OnPartDismantledServer( notnull Man player, string part_name, int action_id )
	{
		ConstructionPart constrution_part = GetConstruction().GetConstructionPart( part_name );

		//check gate state
		if ( constrution_part.IsGate() )
		{
			SetGateState( false );
		}
		
		//check gate state
		if ( constrution_part.IsGate() )
		{
			if ( IsLocked() )
			{
				CombinationLock combination_lock = CombinationLock.Cast( FindAttachmentBySlotName( ATTACHMENT_SLOT_COMBINATION_LOCK ) );
				combination_lock.UnlockServer( player , this );
			}
		}
		
		super.OnPartDismantledServer( player, part_name, action_id );
	}
	
	override void OnPartDestroyedServer( Man player, string part_name, int action_id,  bool destroyed_by_connected_part = false )
	{
		ConstructionPart constrution_part = GetConstruction().GetConstructionPart( part_name );

		//check gate state
		if ( constrution_part.IsGate() )
		{
			SetGateState( false );
		}

		super.OnPartDestroyedServer( player, part_name, action_id );
	}

	override bool CanReceiveAttachment( EntityAI attachment, int slotId )
	{
		//manage action initiator (AT_ATTACH_TO_CONSTRUCTION)
		if ( !GetGame().IsMultiplayer() || GetGame().IsClient() )
		{
			PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
			if ( player )
			{
				ConstructionActionData construction_action_data = player.GetConstructionActionData();
				
				//reset action initiator
				construction_action_data.SetActionInitiator( NULL );				
			}
		}
		
		if ( attachment.Type() == ATTACHMENT_COMBINATION_LOCK )
		{
			if ( !HasGate() || IsOpened() )
			{
				return false;
			}
		}
			
		return true;
	}

	override bool CanPutIntoHands( EntityAI parent )
	{
		if( !super.CanPutIntoHands( parent ) )
		{
			return false;
		}
		
		if ( HasBase() )
		{
			return false;
		}
		
		return true;
	}
	
	//--- OPEN/CLOSE ACTIONS
	bool CanOpendogshed_door()
	{
		if ( HasGate() && !IsOpened() && !IsLocked() )
		{
			return true;
		}
		
		return false;
	}
	
	bool CanClosedogshed_door()
	{
		if ( HasGate() && IsOpened() )
		{
			return true;
		}
		
		return false;
	}
	
	void Opendogshed_door()
	{
		//server or single player
		if ( GetGame().IsServer() )
		{
			float value = GATE_ROTATION_ANGLE_DEG;
			SetAnimationPhase( "Wall_Interact_Rotate", 		value );
			SetAnimationPhase( "dogshed_door_Rotate", 		value );
			
			SetOpenedState( true );
			
			//regenerate navmesh
			GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( UpdateNavmesh, GATE_ROTATION_TIME_APPROX, false );
			
			//synchronize
			SynchronizeBaseState();
		}
		
		//client or single player
		if ( !GetGame().IsMultiplayer() || GetGame().IsClient() )
		{
			//play sound
			SoundGateOpenStart();
		}
		
		//add check
		GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).Remove(Checkdogshed_doorClosed);
		GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( Checkdogshed_doorOpened, 0, true );
	}
	
	void Closedogshed_door()
	{
		//server or single player
		if ( GetGame().IsServer() )
		{		
			float value = 0;
			SetAnimationPhase( "Wall_Interact_Rotate", 		value );
			SetAnimationPhase( "dogshed_door_Rotate", 		value );

			SetOpenedState( false );
			
			//regenerate navmesh
			GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( UpdateNavmesh, GATE_ROTATION_TIME_APPROX, false );
			
			//synchronize
			SynchronizeBaseState();
		}
		
		//client or single player
		if ( !GetGame().IsMultiplayer() || GetGame().IsClient() )
		{
			//play sound
			SoundGateCloseStart();
		}
		
		//add check
		GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).Remove(Checkdogshed_doorOpened);
		GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( Checkdogshed_doorClosed, 0, true );
	}
	
	protected void Checkdogshed_doorClosed()
	{
		if ( GetAnimationPhase( "dogshed_door_Rotate" ) == 0 )			//animation closed
		{
			//play sound
			if ( this ) SoundGateCloseEnd();
			
			//remove check
			GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).Remove( Checkdogshed_doorClosed );
		}
	}
	
	protected void Checkdogshed_doorOpened()
	{
		if ( GetAnimationPhase( "dogshed_door_Rotate" ) == GATE_ROTATION_ANGLE_DEG )			//animation finished - open
		{
			//remove check
			GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).Remove( Checkdogshed_doorOpened );
		}
	}
	
	void UpdateVisualState()
    {
        if ( IsOpen() )
        {
			SetAnimationPhase("dogshed_door",1);
        }
        else
        {
			SetAnimationPhase("dogshed_door",0);
        }		
    }
	
	override bool IsFacingPlayer( PlayerBase player, string selection )
	{
		return false;
	}
	
	override bool IsFacingCamera( string selection )
	{
		return false;
	}
	
	override bool HasProperDistance( string selection, PlayerBase player )
	{
		if ( MemoryPointExists( selection ) )
		{
			vector selection_pos = ModelToWorld( GetMemoryPointPos( selection ) );
			float distance = vector.Distance( selection_pos, player.GetPosition() );
			if ( distance >= MAX_ACTION_DETECTION_DISTANCE )
			{
				return false;
			}
		}
		
		return true;
	}
	
	//================================================================
	// SOUNDS
	//================================================================
	protected void SoundGateOpenStart()
	{
		PlaySoundSet( m_SoundGate_Start, SOUND_GATE_OPEN_START, 0.1, 0.1 );
	}

	protected void SoundGateCloseStart()
	{
		PlaySoundSet( m_SoundGate_Start, SOUND_GATE_CLOSE_START, 0.1, 0.1 );
	}

	protected void SoundGateCloseEnd()
	{
		PlaySoundSet( m_SoundGate_End, SOUND_GATE_CLOSE_END, 0.1, 0.1 );
	}
	
	bool pickable = false;
	
	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ActionOpendogshed_door);
		AddAction(ActionClosedogshed_door);
		AddAction(ActionSummonDogMenu);
    }
}