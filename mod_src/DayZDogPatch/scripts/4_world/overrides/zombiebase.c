modded class ZombieBase
{
	//private ref InfectedST m_AnimST;
	
	void ZombieBase()
	{
		//m_AnimST = new InfectedST( this );
	}
	
	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
		if( GetGame().IsServer() && source.IsInherited(Dayz_Doggo) && GetHealth() <= 100 ) //Thx Liven for suggestion
		{
			//float rndnum = Math.RandomFloatInclusive(0, 1);
			//if (rndnum > 0.4)
			SetHealth(0);
			Dayz_Doggo dog = Dayz_Doggo.Cast(source);
			if (dog)
			{
				if (dog.GetOwnerId())
				{
					PlayerBase player = GetPlayerByEntityID(dog.GetOwnerId());
					if (player)
					{
						EntityAI dogslot = player.FindAttachmentBySlotName("Dog");
						if (dogslot)
						{
							DogPreview_Base dogpr = DogPreview_Base.Cast(dogslot);
							dogpr.AddZedKill();
						}
					}
				}
				
			}
		}		
	}
	
	
	override bool CanBeTargetedByAI(EntityAI ai)
	{
		
		Dayz_Doggo dog = Dayz_Doggo.Cast(ai);	
		if (dog && ( dog.GetDoggoCmd() == DoggoCmd.MISSION || dog.GetDoggoCmd() == DoggoCmd.FOLLOW || dog.GetDoggoCmd() == DoggoCmd.STAY || dog.GetDoggoCmd() == DoggoCmd.PLRCONTROL )&& dog.GetDoggoAttackTarget() != this )
		{
			return false;
		}
		return super.CanBeTargetedByAI( ai );
	}
	/*
	override bool ModCommandHandlerInside(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)
	{
		if (super.ModCommandHandlerInside(pDt, pCurrentCommandID, pCurrentCommandFinished))
		{
			return true;
		}
		DayZInfectedInputController ic = GetInputController();
		m_ActualTarget = ic.GetTargetEntity();
		if (m_ActualTarget)
		{
			PlayerBase pb = PlayerBase.Cast(m_ActualTarget);
			Dayz_Doggo dog = Dayz_Doggo.Cast(m_ActualTarget);
			Dayz_Doggo switch_dog;
			if (pb)
			{
				if (pb.HaveDog())
				{
					if(pb.GetMyDog())
					{
						//if(( pb.GetMyDog().IsInherited(Doggo_Follow1) || pb.GetMyDog().IsInherited(Doggo_Follow2) ||pb.GetMyDog().IsInherited(Doggo_Follow3) || pb.GetMyDog().IsInherited(Doggo_Follow4)|| pb.GetMyDog().IsInherited(Doggo_Follow5)|| pb.GetMyDog().IsInherited(Doggo_Follow6)|| pb.GetMyDog().IsInherited(Doggo_Follow7) || pb.GetMyDog().IsInherited(Doggo_Follow8) ) && this.IsAlive() )
						if ((pb.GetMyDog().GetDoggoCmd() == DoggoCmd.FOLLOW || pb.GetMyDog().GetDoggoCmd() == DoggoCmd.STAY ) && this.IsAlive() )
						{
							if (vector.Distance(pb.GetMyDog().GetPosition(), pb.GetPosition()) < 5 && vector.Distance(this.GetPosition(), pb.GetPosition()) < 2.5)
							{
								switch_dog = pb.GetMyDog().zmb_atk_ownr(pb.GetMyDog());
								pb.setMyDog(switch_dog);
								pb.GetMyDog().SetOwnerId(pb.GetDogOwnerID());
								pb.GetMyDog().SetDoggoCmd(DoggoCmd.PATROL);
							}
		
						}
					}
				}
			}
			else if (dog)
			{
				if (dog.GetOwnerId())
				{
					PlayerBase player = GetPlayerByEntityID(dog.GetOwnerId());
					if (player && player.GetMyDog() && player.GetMyDog().GetDoggoCmd() == DoggoCmd.FOLLOW && this.IsAlive() )
					{
						if (vector.Distance(this.GetPosition(), dog.GetPosition()) < 2.5)
						{
							switch_dog = player.GetMyDog().zmb_atk_ownr(player.GetMyDog());
							player.setMyDog(switch_dog);
							player.GetMyDog().SetOwnerId(player.GetDogOwnerID());
							player.GetMyDog().SetDoggoCmd(DoggoCmd.PATROL);
						}
					}
				}
			}
		}	

		return false;
	}
	*/
	/*
	InfectedST GetAnimST()
	{
		return m_AnimST;
	}

	override bool ModCommandHandlerAfter(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)
	{
		if (super.ModCommandHandlerAfter(pDt, pCurrentCommandID, pCurrentCommandFinished))
		{
			return true;
		}
		DayZInfectedInputController ic = GetInputController();
		if (pCurrentCommandID == DayZInfectedConstants.COMMANDID_SCRIPT)
		{
			return true;
		}
		ZmbTestCommandScript tst = new ZmbTestCommandScript( this, m_AnimST );
		StartCommand_Script( tst );
		return false;
		
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
};
