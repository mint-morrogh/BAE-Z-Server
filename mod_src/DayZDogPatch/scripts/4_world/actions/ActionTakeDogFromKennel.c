class ActionTakeDogFromKennel: ActionInteractBase
{
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTCursor;
	}
	
	void ActionTakeDogFromKennel()
	{
		m_Text = "#DD_take_dog_from_kennel";
	}
	/*
	override typename GetInputType()
	{
		return ContinuousInteractActionInput;
	}
	*/
	override ActionData CreateActionData()
	{
		DetachActionData action_data = new DetachActionData;
		return action_data;
	}
	
	int FindSlotIdToDetach(PlayerBase player, ActionTarget target, ItemBase item)
	{
		EntityAI target_entity = EntityAI.Cast(target.GetObject());
		
		if ( player && target_entity )
		{
			array<string> selections = new array<string>();
			target_entity.GetActionComponentNameList( target.GetComponentIndex(),selections );
			
			if ( target_entity && target_entity.GetInventory() && target_entity.GetInventory().AttachmentCount() > 0 )
			{
				for(int i = 0; i < selections.Count(); i++ )
				{
					int target_slot_id = InventorySlots.GetSlotIdFromString( "Dog" );
					EntityAI att = target_entity.GetInventory().FindAttachment(target_slot_id);
					
					if ( att )
					{
						if ( att.CanDetachAttachment( target_entity ) && target_entity.CanReleaseAttachment( att ) )
							return target_slot_id;
					}
				}		
			}
		}
		return InventorySlots.INVALID;
	}
	
	override bool SetupAction(PlayerBase player, ActionTarget target, ItemBase item, out ActionData action_data, Param extra_data = NULL)
	{
		int attSlotId = InventorySlots.INVALID;
		if (!GetGame().IsDedicatedServer() )
		{
			attSlotId = FindSlotIdToDetach(player, target, item);
		}
		
		if ( super.SetupAction( player, target, item, action_data, extra_data))
		{
			if (!GetGame().IsDedicatedServer())
			{
				if(attSlotId != InventorySlots.INVALID)
				{
					DetachActionData action_data_a = DetachActionData.Cast(action_data);
					action_data_a.m_AttSlot = attSlotId;
					return true;
				}
				return false;
			}
			return true;
		}
		return false;
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		dog_shed_small dogshed = dog_shed_small.Cast( target.GetObject() );
		if (player.HaveDog())
		{
			return false;
		}
		if (dogshed && dogshed.GetDogOwnerID() != player.GetDogOwnerID())
		{
			return false;
		}	
		return FindSlotIdToDetach(player, target, item) != InventorySlots.INVALID;
	}
	
	override Object GetDisplayInteractObject(PlayerBase player, ActionTarget target)
	{
		int target_slot_id = FindSlotIdToDetach(player, target, null);
		EntityAI target_entity = EntityAI.Cast( target.GetObject() );
		
		if(target_slot_id != InventorySlots.INVALID)
		{
			return target_entity.GetInventory().FindAttachment(target_slot_id);
		}
		return null;
	}
	
	override void OnExecuteClient( ActionData action_data )
	{
		Process(action_data);
	}
	
	override void OnExecuteServer( ActionData action_data )
	{
		if(!GetGame().IsMultiplayer())
			Process(action_data);
	}

	void Process( ActionData action_data )
	{
		ClearInventoryReservationEx(action_data);
		
		DetachActionData action_data_a = DetachActionData.Cast(action_data);
		EntityAI target_entity = EntityAI.Cast( action_data_a.m_Target.GetObject() );
				
		ItemBase attachment = ItemBase.Cast(target_entity.GetInventory().FindAttachment(action_data_a.m_AttSlot));
			
		if(attachment)
		{
			action_data.m_Player.PredictiveTakeEntityAsAttachment( attachment );
		}
	}
}


