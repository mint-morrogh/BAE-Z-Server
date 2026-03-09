class dog_bowl extends Container_Base
{
	override bool IsContainer()
	{
		return true;
	}
	
	override bool CanPutInCargo( EntityAI parent )
	{
		if( !super.CanPutInCargo(parent) ) 
		{
			return false;
		}	
		if ( parent.IsKindOf("dog_shed_big") && parent.IsKindOf("dog_shed_wooden"))
		{
			return false;
		}
		
		return true;
	}
	
	override bool CanPutAsAttachment( EntityAI parent )
    {
        if ( !IsRuined() )
            return true;
        return false;
    }
	
	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ActionAttachOnProxy);
		AddAction(ActionAttachOnSelection);
		AddAction(ActionDetach);
	}
}
class dog_bone_toy extends Container_Base
{
	private PlayerBase m_ThrowerID;
	protected int m_SurfaceTimer;
	void dog_bone_toy()
	{
		SetEventMask(EntityEvent.SIMULATE);
		//dBodyActive(this, ActiveState.ALWAYS_ACTIVE);
	}
	
	override bool IsContainer()
	{
		return true;
	}
	
	void SetToyThrower(PlayerBase thrower)
	{
		m_ThrowerID = thrower;	
	}
	
	PlayerBase GetToyThrover()
	{
		return m_ThrowerID;
	}
	

	
	override void EOnSimulate(IEntity other, float dt)
	{
		//NotificationSystem.SendNotificationToPlayerExtended(Man.Cast(GetToyThrover()), 2, "APORT", GetToyThrover().GetIdentity().GetName());

		if (GetGame().IsServer())
		{
			if (GetToyThrover() && !GetToyThrover().HaveDog())
			{
				return;
			}
			// kdyz je ve vode !!!
			if (GetToyThrover() && GetToyThrover().GetMyDog() && GetToyThrover().GetMyDog().GetDoggoAttackTarget() == this)
			{
				m_SurfaceTimer++;
			}
			float depth = GetGame().GetWaterDepth( GetPosition() );
			//vector transform[4];
			//GetTransform( transform );
			//transform[3][1] = GetGame().SurfaceY(GetPosition()[0], GetPosition()[2]) + depth;
			vector pos = GetPosition();
			
			if ( (GetGame().SurfaceIsSea(pos[0],pos[2]) || GetGame().SurfaceIsPond(pos[0],pos[2])) && GetToyThrover() && GetToyThrover().GetMyDog() && GetToyThrover().GetMyDog().GetDoggoAttackTarget() == this && m_SurfaceTimer > 200 )
			{
				//MoveInTime( transform, dt );
				pos[1] = pos[1] + depth;
				SetPosition(pos);
			}
			
		}
		

	}
	
	override void EOnContact( IEntity other, Contact extra )
	{
		//pokud je pes u hrace po dopadu se zavola event
		//NotificationSystem.SendNotificationToPlayerExtended(Man.Cast(GetToyThrover()), 2, "APORT", GetToyThrover().GetIdentity().GetName());
		if (GetGame().IsServer())
		{
			if ( GetToyThrover().HaveDog())
			{
				float distance = vector.Distance(GetToyThrover().GetMyDog().GetPosition(), GetToyThrover().GetPosition());
				EntityAI mouth_ent = GetToyThrover().GetMyDog().FindAttachmentBySlotName("DogMouth");
				EntityAI mask_ent = GetToyThrover().GetMyDog().FindAttachmentBySlotName("DogMask");
				if (distance < 5 && !mouth_ent && !mask_ent)
				{
					GetToyThrover().GetMyDog().SetDoggoAttackTarget(this);
					GetToyThrover().GetMyDog().SetDoggoCmd(DoggoCmd.PATROL);
					//DayZAnimalInputController inputController = GetToyThrover().GetMyDog().GetInputController();
					//inputController.OverrideMovementSpeed(true, 4);
					m_SurfaceTimer = 0;
				}	
			}
		}
	}
	
	
	
	override bool CanPutInCargo( EntityAI parent )
	{
		if( !super.CanPutInCargo(parent) ) 
		{
			return false;
		}	
		if ( parent.IsKindOf("dog_bowl"))
		{
			return false;
		}
		
		return true;
	}
	
	override bool CanPutAsAttachment( EntityAI parent )
    {
        if ( !IsRuined() )
            return true;
        return false;
    }
	
	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ActionAttachOnProxy);
		AddAction(ActionAttachOnSelection);
		AddAction(ActionDetach);
	}
}

class DogVestBase: Clothing 
{
	override bool CanPutInCargoClothingConditions(EntityAI parent)
    {
        return true;
    }
	
    override bool CanReceiveItemIntoCargoClothingConditions(EntityAI item)
    {
        return true;
    }

    override bool CanLoadItemIntoCargoClothingConditions(EntityAI item)
    {        
        return true;
    }
	
	
	override bool CanPutInCargo( EntityAI parent )
	{
		if ( !super.CanPutInCargo( parent ) )
			return false;
		
		if ( parent && parent.IsKindOf( "DogVestBase" ) )
			return false;
		
		return true;
	}
	
	override bool CanReceiveItemIntoCargo(EntityAI item)
	{
		if ( !super.CanReceiveItemIntoCargo( item ) )
			return false;

		if ( item.IsKindOf( "DogVestBase" ) )
			return false;

		return true;
	}
	
	override bool CanLoadItemIntoCargo(EntityAI item)
	{
		if ( !super.CanLoadItemIntoCargo( item ) )
			return false;

		if (item.IsKindOf( "DogVestBase" ) )
			return false;

		return true;
	}
	
}

class magic_dog_bone_toy : dog_bone_toy {}

class DogPainting_Base : Inventory_Base
{
	override void SetActions()
	{
		super.SetActions();
		AddAction(ActionTogglePlaceObject);
		AddAction(ActionPlaceObject);

	} 
	
	override bool CanPutIntoHands(EntityAI parent) 
	{
		return true;
	}
	
	override void OnPlacementComplete( Man player, vector position = "0 0 0", vector orientation = "0 0 0" )
	{
		SetIsDeploySound( true );
		super.OnPlacementComplete(player, position, orientation);
	}
	
	override bool IsDeployable()
	{
		return true;
	}

}
class DogNemysPaint : DogPainting_Base {}