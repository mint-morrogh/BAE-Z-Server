class DoggoCloseTrigger extends Trigger
{
	protected Dayz_Doggo m_ParentDog;
	protected int m_NumberOfCreatures;
	
	void SetParentDog(Dayz_Doggo dog)
	{
		m_ParentDog = dog;
	}
	
	override void EOnInit(IEntity other, int extra)
	{
		SetCollisionSphere(2);
		m_NumberOfCreatures = 0;
	}
	
    override bool CanAddObjectAsInsider(Object object)
    {
        DayZCreatureAI creature = DayZCreatureAI.Cast( object );
        if(creature)
        {
            return !creature.ResistContaminatedEffect();
        }
        else
        {
            PlayerBase player = PlayerBase.Cast(object);
            return player != null;
        }
    }
	
	override void OnEnter(Object obj)
    {
		DayZCreature creature;
		Class.CastTo( creature, obj);
		BehaviourGroupInfectedPack bgip;
		//check if dog have gasmask
		//DoggoLogger.Log("DOGGO " + m_ParentDog + " " +  m_ParentDog.GetAIAgent() + " " + m_ParentDog.GetAIAgent().GetGroup() + " " +  m_ParentDog.GetAIAgent().GetGroup().GetBehaviour());
		if (m_ParentDog && m_ParentDog.GetAIAgent() && m_ParentDog.GetAIAgent().GetGroup())
		{
			Class.CastTo( bgip, m_ParentDog.GetAIAgent().GetGroup().GetBehaviour());
		}
		if ((obj.IsMan() || creature )&& obj.IsAlive() && !obj.IsInherited(Dayz_Doggo) && GetGame().IsServer() && m_ParentDog && m_ParentDog.IsAlive() && m_ParentDog.GetDoggoCmd() != DoggoCmd.PLRCONTROL )
		{
			m_NumberOfCreatures++;
			//if not as predator switch to predator
			//DoggoLogger.Log("DOGGO " + obj + " num " + m_NumberOfCreatures + " " + m_ParentDog.GetOwnerId() + " " + obj.GetID());
			if (m_NumberOfCreatures > 0 && bgip && m_ParentDog && m_ParentDog.GetOwnerId() != obj.GetID() )
			{
				//DoggoLogger.Log("predator switch");
				//AIGroup grp0 = GetGame().GetWorld().GetAIWorld().CreateGroup("DZWolfGroupBeh");
				
				/*
				AIGroup grp0 = GetGame().GetWorld().GetAIWorld().CreateGroup("Predator_Doggo");
				m_ParentDog.DestroyAIAgent();
				m_ParentDog.InitAIAgent(grp0);
				*/
				
				///DayZAnimalInputController inputController = m_ParentDog.GetInputController();
				//inputController.OverrideBehaviourAction(true, DayZAnimalBehaviourAction.CHARGING);
			}
			ZombieBase zombie = ZombieBase.Cast(obj);
			if (obj.IsInherited(Animal_GallusGallusDomesticus) || (zombie && zombie.IsCrawling()) )
			{
				m_ParentDog.SetDogAttackLow(true);
			}
		}
		/* //not works for items
		else if ((obj.IsInherited(DeadChicken_ColorBase) || obj.IsInherited(DeadRabbit)) && GetGame().IsServer() && m_ParentDog.GetDoggoCmd() != DoggoCmd.PLRCONTROL && m_NumberOfCreatures == 0)
		{
			DoggoLogger.Log("found something");
			EntityAI mouthslot = m_ParentDog.FindAttachmentBySlotName("DogMouth");
			if (!mouthslot)
			{
				string tlama = EntityAI.Cast(obj).GetType();
				m_ParentDog.GetInventory().CreateAttachment(tlama);
				EntityAI.Cast(obj).DeleteSave();
			}
		}
		*/
	}
	
	override void OnLeave(Object obj)
	{
		DayZCreature creature;
		Class.CastTo( creature, obj);
		if ((obj.IsMan() || creature ) && GetGame().IsServer() && m_ParentDog && m_ParentDog.GetDoggoCmd() != DoggoCmd.PLRCONTROL )
		{
			m_NumberOfCreatures--;
			//DoggoLogger.Log("num leave " + m_NumberOfCreatures );
			if (m_NumberOfCreatures < 1 || GetInsiders().Count() == 0)
			{
				//switch back to patrol
				m_NumberOfCreatures = 0;
				//GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( SwitchBack, 10000, false );
				
			}
		}
	}
	void SwitchBack()
	{
		if (m_NumberOfCreatures < 1 || GetInsiders().Count() == 0)
		{
			//DoggoLogger.Log("switch back ");
			AIGroup grp0 = GetGame().GetWorld().GetAIWorld().CreateGroup("DZDogGroupBeh");
			if (m_ParentDog && m_ParentDog.IsAlive())
			{
				m_ParentDog.DestroyAIAgent();
				m_ParentDog.InitAIAgent(grp0);
				m_ParentDog.SetDogAttackLow(false);
				//DayZAnimalInputController inputController = m_ParentDog.GetInputController();
				//inputController.OverrideBehaviourAction(false, DayZAnimalBehaviourAction.CHARGING);				
			}

		}

	}
	
}

class DoggoFarTrigger extends Trigger
{
	protected Dayz_Doggo m_ParentDog;
	
	void SetParentDog(Dayz_Doggo dog)
	{
		m_ParentDog = dog;
	}
	
	override void EOnInit(IEntity other, int extra)
	{
		SetCollisionSphere(40);
	}
	
	override bool CanAddObjectAsInsider(Object object)
    {
        PlayerBase player = PlayerBase.Cast(object);
        return player != null;
    }
		
	override void OnEnterServerEvent( TriggerInsider insider )
	{
		PlayerBase playerInsider = PlayerBase.Cast(insider.GetObject());
		if (playerInsider && playerInsider.GetIdentity() && playerInsider.GetIdentity().GetName() && m_ParentDog && m_ParentDog.IsAlive() && m_ParentDog.GetOwnerId() != playerInsider.GetID())
		{
			//DoggoLogger.Log("PLAYER NEARBY " + vector.Distance(m_ParentDog.GetPosition(), insider.GetObject().GetPosition()));
			m_ParentDog.StartGrowl();
			GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( ResetGrowl, 1500, false );
		}
		super.OnEnterServerEvent(insider);
	}
	

	void ResetGrowl()
	{
		if (m_ParentDog && m_ParentDog.IsAlive())
		{
			m_ParentDog.ResetGrowl();
		}
	}
}
