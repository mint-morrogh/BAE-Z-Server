class ActionControlDog: ActionInteractBase
{
	void ActionControlDog()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_HUDCursorIcon = CursorIcons.CloseHood;
	}

	override string GetText()
	{
		return "Ride Doggo";
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		PlayerBase player_me = PlayerBase.Cast(GetGame().GetPlayer());

		Dayz_Doggo body_EAI;
		Class.CastTo(body_EAI, target.GetObject());
		
		if ( body_EAI  &&  body_EAI.IsAlive()  &&  (body_EAI.IsInherited(Dayz_Doggo)) )
		{
			if((body_EAI.GetDoggoCmd() != DoggoCmd.PLRCONTROL )&& body_EAI.GetOwnerId() == player.GetDogOwnerID() && player.GetItemInHands() && player.GetItemInHands().GetType() == "magic_dog_bone_toy" )

			{
				return true;
			}
		}
		return false;
	}
	override void Start(ActionData action_data)
	{
		super.Start( action_data );
		
		action_data.m_Player.TryHideItemInHands(true);
		action_data.m_Player.SetPlayerHidden(true);	
		if (action_data.m_Player.GetInventory())
		{
			action_data.m_Player.GetInventory().LockInventory(LOCK_FROM_SCRIPT);
		}
		
	}
	override void OnExecuteClient( ActionData action_data )
	{	
		Object targetObject = action_data.m_Target.GetObject();				
		vector targetObjectPos = targetObject.GetPosition();
		
		Dayz_Doggo target_dog;
		Class.CastTo(target_dog, targetObject);
		GetGame().SelectSpectator( action_data.m_Player.GetIdentity() , "DoggoCamera", (targetObject.ModelToWorld("0 1 -1")));
		DoggoCamera.Cast(Camera.GetCurrentCamera()).SetTarget(targetObject);
		AttachPlayer(action_data.m_Player, targetObject);
		
	}

	override void OnExecuteServer( ActionData action_data )
	{	
		Object targetObject = action_data.m_Target.GetObject();				
		vector targetObjectPos = targetObject.GetPosition();
		
		Dayz_Doggo target_dog;
		Class.CastTo(target_dog, targetObject);
		target_dog.SetDoggoCmd(DoggoCmd.PLRCONTROL);
		AttachPlayer(action_data.m_Player, targetObject);
		

	}
	
	void AttachPlayer(PlayerBase player, Object obj)
	{
		vector tmPlayer[4]; 
		vector tmTarget[4];
		vector tmLocal[4];
		player.GetTransform( tmPlayer );	
		obj.GetTransform( tmTarget );
		tmPlayer[3] = tmTarget[3] + Vector(0, 1.2, -1.2);
		Math3D.MatrixInvMultiply4( tmTarget, tmPlayer, tmLocal );
		if (!player.GetParent())
		{
			player.LinkToLocalSpaceOf( obj, tmLocal );
			dBodyEnableGravity(player, false);
			
		}
	}
	
	/*
	void AttachPlayer(PlayerBase player, Object obj)
	{
		vector tmPlayer[4]; 
		vector tmTarget[4];
		vector tmLocal[4];
		player.GetTransform( tmPlayer );
		EntityAI vestslot = EntityAI.Cast(obj).FindAttachmentBySlotName("DogMouth");
		if (!vestslot)
		{
			return;
		}
		//DoggoLogger.Log("BONE " + Entity.Cast(obj).GetBoneIndex( "Head" ) + " " + (EntityAI.Cast(obj)).GetBoneObject( 16 ));
		//Object bone = Entity.Cast(obj).GetBoneObject( 16 );	
		vestslot.GetTransform( tmTarget );
		tmPlayer[3] = tmTarget[3] + Vector(0, 0, 0);
		Math3D.MatrixInvMultiply4( tmTarget, tmPlayer, tmLocal );
		if (!player.GetParent())
		{
			player.LinkToLocalSpaceOf( vestslot, tmLocal );
			dBodyEnableGravity(IEntity.Cast(player), false);
			
		}
	}
	*/
};