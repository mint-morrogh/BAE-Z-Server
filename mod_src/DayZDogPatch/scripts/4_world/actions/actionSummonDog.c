class ActionSummonDog: ActionInteractBase
{
	void ActionSummonDog()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_HUDCursorIcon = CursorIcons.CloseHood;
	}

	override string GetText()
	{
		return "#DD_summon_dog";
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Doggo_House doghouse;
		Class.CastTo(doghouse, target.GetObject());
		Bone bone;
		Class.CastTo(bone, player.GetItemInHands());
		if ( doghouse && !player.HaveDog() && bone)
		{
			return true;
		}
		return false;
	}

	override void OnExecuteServer( ActionData action_data )
	{	
		Object targetObject = action_data.m_Target.GetObject();				
		vector targetObjectPos = targetObject.GetPosition();
		action_data.m_Player.LocalDestroyEntityInHands();
		//targetObject.SetPosition(vector.Zero);
		//GetGame().ObjectDelete( targetObject );
		// model space x - 2 ModelToWorld
		vector dogspawn = targetObject.ModelToWorld("-2 0 0");

		Doggo_Stay target_dog = Doggo_Stay.Cast(GetGame().CreateObject("Doggo_Stay", dogspawn, true, true ));
		target_dog.SetOwnerId(action_data.m_Player.GetID());
		action_data.m_Player.setMyDog(target_dog);
		GetGame().RemoteObjectCreate( target_dog );
		// musi to pres rpc
		//needs to be an rpc
		//action_data.m_Player.resetDog();
	}
};

