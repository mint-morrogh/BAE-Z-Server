class ActionPutDogToKennel: ActionInteractBase
{
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTCursor;
	}
	
	void ActionPutDogToKennel()
	{
		m_Text = "#DD_put_dog_to_kennel";
	}
	
	override ActionData CreateActionData()
	{
		AttachActionData action_data = new AttachActionData;
		return action_data;
	}
	
	int FindSlotIdToAttachOrCombine(PlayerBase player, ActionTarget target, ItemBase item)
	{
		EntityAI target_entity = EntityAI.Cast(target.GetObject());
		
		if ( player && target_entity )
		{
			array<string> selections = new array<string>();
			target_entity.GetActionComponentNameList( target.GetComponentIndex(),selections );
			
			if ( target_entity && target_entity.GetInventory() && target_entity.GetInventory().AttachmentCount() == 0 )
			{
				for(int i = 0; i < selections.Count(); i++ )
				{
					
					int target_slot_id = InventorySlots.GetSlotIdFromString( "Dog" );
					EntityAI att = player.GetInventory().FindAttachment(target_slot_id);
					
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
			attSlotId = FindSlotIdToAttachOrCombine(player, target, item);
		}
		
		if ( super.SetupAction( player, target, item, action_data, extra_data))
		{
			if (!GetGame().IsDedicatedServer())
			{
				if(attSlotId != InventorySlots.INVALID)
				{
					AttachActionData action_data_a = AttachActionData.Cast(action_data);
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
		return FindSlotIdToAttachOrCombine(player, target, item) != InventorySlots.INVALID;
	}

	override void OnExecuteServer( ActionData action_data )
	{	
		if (GetGame().IsMultiplayer())
			return;
		
		EntityAI target_entity = EntityAI.Cast( action_data.m_Target.GetObject() );
		EntityAI item_entity = action_data.m_MainItem;
		
		AttachActionData action_data_a = AttachActionData.Cast(action_data);
		
		action_data.m_Player.PredictiveTakeEntityToTargetAttachmentEx(target_entity, item_entity, action_data_a.m_AttSlot );
		//action_data.m_Player.ServerTakeEntityToTargetAttachmentEx(target_entity, item_entity, action_data_a.m_AttSlot );
		//action_data.m_Player.LocalTakeEntityToTargetAttachmentEx(target_entity, item_entity, action_data_a.m_AttSlot );
	}
	
	override void OnExecuteClient( ActionData action_data )
	{
		ClearInventoryReservationEx(action_data);
		EntityAI target_entity = EntityAI.Cast( action_data.m_Target.GetObject() );
		EntityAI item_entity = action_data.m_Player.GetDogSlot();
		
		AttachActionData action_data_a = AttachActionData.Cast(action_data);
		
		//ItemBase attachment = ItemBase.Cast( target_entity.GetInventory().FindAttachment( action_data_a.m_AttSlot ) );
			
		
		ItemBase item_base	= ItemBase.Cast( item_entity );
		
		action_data.m_Player.PredictiveTakeEntityToTargetAttachmentEx(target_entity, item_entity, action_data_a.m_AttSlot);
		//action_data.m_Player.LocalTakeEntityToTargetAttachmentEx(target_entity, item_entity, action_data_a.m_AttSlot );
		//item_entity.LockToParent();
		//action_data.m_Player.SetDogAttLock(true);	
	}
};