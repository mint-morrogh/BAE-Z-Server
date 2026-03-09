class ActionDogWhistleCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(0.5);
	}
};

class ActionDogWhistle: ActionContinuousBase
{
	void ActionDogWhistle()
	{
		m_CallbackClass = ActionDogWhistleCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_TAKETEMPSELF;
		m_CommandUIDProne = DayZPlayerConstants.CMD_ACTIONFB_TAKETEMPSELF;
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_LOW;
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINotPresent;
		m_ConditionTarget = new CCTSelf;
	}
	
	override bool HasProneException()
	{
		return true;
	}

	override bool HasTarget()
	{
		return false;
	}
	
	override bool CanBeUsedInVehicle()
	{
		return false;
	}
		
	override string GetText()
	{
		return "#STR_DD_whistle";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		//PlayerBase player_me = PlayerBase.Cast(GetGame().GetPlayer());
		
		//if(player.MyDog  &&  player.MyDog.IsAlive())
		if (player.HaveDog())
		{
			player.ResetWhistle();
			return true;
		}

		return false;
	}
	
	override void OnStartServer(ActionData action_data )
	{	
		vector Doggo_location = action_data.m_Player.GetPosition();
		//DogWhistle whistle = GetGame().CreateObject("DogWhistle", Doggo_location);
		//whistle.BlowWhistle();
		action_data.m_Player.BlowWhistle();
		
	}

	override void OnFinishProgressServer( ActionData action_data )
	{	
		vector Doggo_location = action_data.m_Player.GetPosition();
		if(action_data.m_Player.GetMyDog())
		{
			//needs to be an rpc
			//if (!action_data.m_Player.MyDog.can_stay())
			//{
				vector targetObjectPos = action_data.m_Player.GetMyDog().GetPosition();
				GetGame().ObjectDelete( action_data.m_Player.GetMyDog() );
				Dayz_Doggo target_dog = Dayz_Doggo.Cast(GetGame().CreateObject("Doggo_Follow"+action_data.m_Player.GetDogIndex(), targetObjectPos, false, true ));

				//needs to be an rpc
				target_dog.SetOwnerId(action_data.m_Player.GetID());
				action_data.m_Player.setMyDog(target_dog);
				//GetGame().RemoteObjectCreate( target_dog );
			//}
			action_data.m_Player.GetMyDog().SetPosition( Doggo_location );
			action_data.m_Player.ResetWhistle();
			
		}
	}
};